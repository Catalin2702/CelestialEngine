//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Common/Glfw
// File: GlfwWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"

#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Utility/Utility.hpp"

#include <GLFW/glfw3.h>

// On macOS GLFW hosts a real NSWindow, so fullscreen defers to the native (Spaces) toggleFullScreen instead of GLFW's
// exclusive monitor fullscreen (which hides the title-bar controls).
#if CE_PLATFORM_MACOS
	#define GLFW_EXPOSE_NATIVE_COCOA
	#include <GLFW/glfw3native.h>

	#include <AppKit/AppKit.hpp>
#endif

#include <stdexcept>
#include <utility>


namespace CE::Core {

// Ensures glfwInit() is called only once during program execution
static bool _st_GLFWInitialized = false;

// Tracks the number of active windows to determine when to call glfwTerminate() (when count reaches 0)
static int _st_GLFWWindowCount = 0;

void GlfwWindowEventHandler::DispatchResizeEvent(const int width, const int height) const {
	windowStateEvents.onResizeDispatcher.Dispatch(width, height);
}

void GlfwWindowEventHandler::DispatchCloseEvent() const {
	windowStateEvents.onCloseDispatcher.Dispatch();
}

void GlfwWindowEventHandler::DispatchErrorEvent(const int errorCode, const char* description) const {
	windowStateEvents.onErrorDispatcher.Dispatch(errorCode, description);
}

void GlfwWindowEventHandler::DispatchFocusEvent(const int focused) const {
	windowStateEvents.onFocusDispatcher.Dispatch(focused);
}

void GlfwWindowEventHandler::DispatchKeyEvent(const int key, const int action, const int scancode, const int mods) const {
	keyboardEvents.onKeyDispatcher.Dispatch(key, action, scancode, mods);
}

void GlfwWindowEventHandler::DispatchCharEvent(const unsigned int codepoint) const {
	keyboardEvents.onCharDispatcher.Dispatch(codepoint);
}

void GlfwWindowEventHandler::DispatchMouseButtonEvent(const int button, const int action, const int mods) const {
	mouseEvents.onMouseButtonDispatcher.Dispatch(button, action, mods);
}

void GlfwWindowEventHandler::DispatchMousePositionEvent(const f64 xPos, const f64 yPos) const {
	mouseEvents.onMousePositionDispatcher.Dispatch(xPos, yPos);
}

void GlfwWindowEventHandler::DispatchMouseDraggedEvent(const int button, const int action, const int mods, const f64 xPos, const f64 yPos) const {
	mouseEvents.onMouseDraggedDispatcher.Dispatch(button, action, mods, xPos, yPos);
}

void GlfwWindowEventHandler::DispatchMouseWheelScrollEvent(const f64 xOffset, const f64 yOffset) const {
	mouseEvents.onMouseWheelScrollDispatcher.Dispatch(xOffset, yOffset);
}

GlfwWindow::GlfwWindow() {
	Init();
}

GlfwWindow::GlfwWindow(GlfwWindow&& other) noexcept:
	windowEventHandler(std::move(other.windowEventHandler)),
	_glfwWindow(std::move(other._glfwWindow)),
	_pressedMouseButtons(other._pressedMouseButtons),
	_lastMouseMods(other._lastMouseMods) {
#if not CE_PLATFORM_MACOS
	_windowedX = other._windowedX;
	_windowedY = other._windowedY;
	_windowedWidth = other._windowedWidth;
	_windowedHeight = other._windowedHeight;
#endif

	_AdoptNativeWindow();
}

GlfwWindow& GlfwWindow::operator=(GlfwWindow&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	// Release the window this object currently owns before taking over other's, otherwise that handle would leak and the
	// window count would drift.
	_Shutdown();

	windowEventHandler = std::move(other.windowEventHandler);
	_glfwWindow = std::move(other._glfwWindow);
	_pressedMouseButtons = other._pressedMouseButtons;
	_lastMouseMods = other._lastMouseMods;

#if not CE_PLATFORM_MACOS
	_windowedX = other._windowedX;
	_windowedY = other._windowedY;
	_windowedWidth = other._windowedWidth;
	_windowedHeight = other._windowedHeight;
#endif

	_AdoptNativeWindow();

	return *this;
}

GlfwWindow::~GlfwWindow() {
	_Shutdown();
}

void GlfwWindow::_AdoptNativeWindow() {
	if (not _glfwWindow) [[unlikely]]
		return;

	glfwSetWindowUserPointer(_glfwWindow.get(), this);
}

void GlfwWindow::OnUpdate() const {
	glfwPollEvents();
}

std::pair<f32, f32> GlfwWindow::GetWindowSize() const {
	int width = 0, height = 0;
	glfwGetWindowSize(_glfwWindow.get(), &width, &height);
	return {static_cast<f32>(width), static_cast<f32>(height)};
}

unsigned int GlfwWindow::GetRefreshRate() const {
	if (not _glfwWindow) [[unlikely]]
		return 0;

	// When fullscreen the window owns a monitor; otherwise fall back to the primary monitor.
	auto monitor = glfwGetWindowMonitor(_glfwWindow.get());
	if (not monitor) [[unlikely]]
		monitor = glfwGetPrimaryMonitor();
	if (not monitor) [[unlikely]]
		return 0;

	const auto mode = glfwGetVideoMode(monitor);
	return mode ? static_cast<unsigned int>(mode->refreshRate) : 0;
}

std::pair<f32, f32> GlfwWindow::GetFrameSize() const {
	if (not _glfwWindow) [[unlikely]] {
		CE_CORE_WARN("GlfwWindow::GetFrameSize Could not get frame size because window is not initialized.");
		return {0.0f, 0.0f};
	}
	int width = 0, height = 0;
	glfwGetFramebufferSize(_glfwWindow.get(), &width, &height);
	return {static_cast<f32>(width), static_cast<f32>(height)};
}

void GlfwWindow::SetWindowSize(const unsigned int width, const unsigned int height) {
	glfwSetWindowSize(_glfwWindow.get(), static_cast<int>(width), static_cast<int>(height));
}

void GlfwWindow::SetCurrentContext(GLFWwindow* window) const {
	const auto ptr = window ? window : _glfwWindow.get();
	glfwMakeContextCurrent(ptr);
}

void GlfwWindow::_SetIOEventCallbacks() {
	if (not _glfwWindow) [[unlikely]]
		return;

	glfwSetKeyCallback(_glfwWindow.get(), [](GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) [[likely]] {
			_this->windowEventHandler.DispatchKeyEvent(key, action, scancode, mods);
		}
	});

	glfwSetCharCallback(_glfwWindow.get(), [](GLFWwindow* window, const unsigned int codepoint) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) [[likely]] {
			_this->windowEventHandler.DispatchCharEvent(codepoint);
		}
	});

	glfwSetMouseButtonCallback(_glfwWindow.get(), [](GLFWwindow* window, const int button, const int action, const int mods) {
		// GLFW has no native drag event, so we track button state here to synthesize
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) [[likely]] {
			if (button >= 0 && button < static_cast<int>(_this->_pressedMouseButtons.size()))
				_this->_pressedMouseButtons[button] = (action == GLFW_PRESS);
			_this->_lastMouseMods = mods;

			_this->windowEventHandler.DispatchMouseButtonEvent(button, action, mods);
		}
	});

	glfwSetCursorPosCallback(_glfwWindow.get(), [](GLFWwindow* window, const f64 xPos, const f64 yPos) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) [[likely]] {
			_this->windowEventHandler.DispatchMousePositionEvent(xPos, yPos);

			// GLFW has no native drag event: if the cursor moves while a mouse button is
			// held, synthesize a drag event for each pressed button.
			for (int button = 0; button < static_cast<int>(_this->_pressedMouseButtons.size()); ++button) {
				if (_this->_pressedMouseButtons[button])
					_this->windowEventHandler.DispatchMouseDraggedEvent(button, GLFW_PRESS, _this->_lastMouseMods, xPos, yPos);
			}
		}
	});

	glfwSetScrollCallback(_glfwWindow.get(), [](GLFWwindow* window, const f64 xOffset, const f64 yOffset) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) [[likely]] {
			_this->windowEventHandler.DispatchMouseWheelScrollEvent(xOffset, yOffset);
		}
	});
}

void GlfwWindow::_SetWindowEventCallbacks() {
	if (not _glfwWindow) [[unlikely]]
		return;

	glfwSetWindowSizeCallback(_glfwWindow.get(), [](GLFWwindow* window, const int width, const int height) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) [[likely]] {
			_this->windowEventHandler.DispatchResizeEvent(width, height);
			_this->SetWindowSize(width, height);
		}
	});

	glfwSetWindowCloseCallback(_glfwWindow.get(), [](GLFWwindow* window) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) [[likely]] {
			_this->windowEventHandler.DispatchCloseEvent();
		}
	});

	glfwSetWindowFocusCallback(_glfwWindow.get(), [](GLFWwindow* window, const int focused) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) [[likely]] {
			_this->windowEventHandler.DispatchFocusEvent(focused);
		}
	});
}

void GlfwWindow::Init() {
	_InitWindow();

	_SetIOEventCallbacks();
	_SetWindowEventCallbacks();

	_st_GLFWWindowCount++;
}

void GlfwWindow::Miniaturize() const {
	if (not _glfwWindow) [[unlikely]] {
		CE_CORE_WARN("GlfwWindow::Miniaturize: Cannot miniaturize because window is not initialized");
		return;
	}

	glfwIconifyWindow(_glfwWindow.get());
}

void GlfwWindow::Deminiaturize() const {
	if (not _glfwWindow) [[unlikely]] {
		CE_CORE_WARN("GlfwWindow::Deminiaturize: Cannot deminiaturize because window is not initialized");
		return;
	}

	glfwRestoreWindow(_glfwWindow.get());
}

void GlfwWindow::ToggleFullScreen() const {
	if (not _glfwWindow) [[unlikely]] {
		CE_CORE_WARN("GlfwWindow::ToggleFullScreen: Cannot toggle fullscreen because window is not initialized");
		return;
	}

#if CE_PLATFORM_MACOS
	// GLFW hosts a real NSWindow on macOS: use the native (Spaces) fullscreen so the transition animates and keeps the
	// title-bar controls, instead of the exclusive glfwSetWindowMonitor fullscreen that covers them and hides the traffic lights.
	const auto nsWindow = reinterpret_cast<NS::Window*>(glfwGetCocoaWindow(_glfwWindow.get()));
	if (not nsWindow) [[unlikely]] {
		CE_CORE_WARN("GlfwWindow::ToggleFullScreen: Cannot toggle fullscreen because the native Cocoa window is unavailable");
		return;
	}

	// Opt the window into native fullscreen so toggleFullScreen animates into its own Space.
	nsWindow->setCollectionBehavior(nsWindow->collectionBehavior() | NS::WindowCollectionBehaviorFullScreenPrimary);
	nsWindow->toggleFullScreen(nullptr);
#else
	const auto window = _glfwWindow.get();

	// Already fullscreen (the window is attached to a monitor): restore the saved windowed placement.
	if (glfwGetWindowMonitor(window)) {
		glfwSetWindowMonitor(window, nullptr, _windowedX, _windowedY, _windowedWidth, _windowedHeight, GLFW_DONT_CARE);
		return;
	}

	// Going fullscreen: remember the current windowed placement so it can be restored on the next toggle.
	glfwGetWindowPos(window, &_windowedX, &_windowedY);
	glfwGetWindowSize(window, &_windowedWidth, &_windowedHeight);

	const auto monitor = glfwGetPrimaryMonitor();
	if (not monitor) [[unlikely]] {
		CE_CORE_WARN("GlfwWindow::ToggleFullScreen: Cannot enter fullscreen because no monitor is available");
		return;
	}

	const auto mode = glfwGetVideoMode(monitor);
	if (not mode) [[unlikely]] {
		CE_CORE_WARN("GlfwWindow::ToggleFullScreen: Cannot enter fullscreen because the monitor video mode is unavailable");
		return;
	}

	glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
#endif
}

void GlfwWindow::_InitWindow() {
	if (not _st_GLFWInitialized) [[likely]] {
		if (const int success = glfwInit(); not success) [[unlikely]] {
			constexpr auto error = "GlfwWindow::_InitWindow: Could not initialize GLFW!";
			windowEventHandler.DispatchErrorEvent(-1, error);
			throw std::runtime_error(error);
		}
		glfwSetErrorCallback([](const int error_code, const char* description) {
			const auto& app = dynamic_cast<GlfwApplication&>(I_Application::StGet());
			app.GetGlfwWindow().windowEventHandler.DispatchErrorEvent(error_code, description);
		});
		_st_GLFWInitialized = true;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	const auto& windowProps = Utility::Config::StGetWindowProps();

	_glfwWindow.reset(glfwCreateWindow(
		static_cast<int>(windowProps.width),
		static_cast<int>(windowProps.height),
		windowProps.title.c_str(),
		nullptr,
		nullptr
	));

	if (not _glfwWindow) [[unlikely]] {
		constexpr auto error = "GlfwWindow::_InitWindow: Failed to create GLFW window!";
		windowEventHandler.DispatchErrorEvent(-1, error);
		throw std::runtime_error(error);
	}

	_AdoptNativeWindow();
}

void GlfwWindow::_Shutdown() {
	// A moved-from window holds no handle and never ran Init, so it must not decrement the window count: doing so would
	// terminate GLFW while the window that took its handle over is still alive.
	if (not _glfwWindow)
		return;

	_glfwWindow.reset();

	_st_GLFWWindowCount--;
	if (_st_GLFWWindowCount == 0 && _st_GLFWInitialized) [[likely]] {
		glfwTerminate();
		_st_GLFWInitialized = false;
	}
}

}
