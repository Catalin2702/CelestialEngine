//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Common/Glfw
// File: GlfwWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"

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
#include <string>
#include <utility>


namespace CE::Core {

void GlfwWindowEventHandler::DispatchResizeEvent(const int width, const int height) const {
	windowStateEvents.onResizeDispatcher.Dispatch(width, height);
}

void GlfwWindowEventHandler::DispatchCloseEvent() const {
	windowStateEvents.onCloseDispatcher.Dispatch();
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

namespace {

/**
 * @brief Recovers the GlfwWindow that owns a native handle
 * @details Every GLFW C callback receives the native handle and nothing else; the owning object is found through the
 *			user pointer installed by _AdoptNativeWindow. Null once that owner has been destroyed.
 */
GlfwWindow* WindowFrom(GLFWwindow* window) {
	return static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window));
}

}

GlfwWindow::GlfwWindow(const GlfwPlatform& platform) {
	_InitWindow(platform);

	_SetIOEventCallbacks();
	_SetWindowEventCallbacks();
}

GlfwWindow::GlfwWindow(GlfwWindow&& other) noexcept:
	windowEventHandler(other.windowEventHandler),
	_glfwWindow(std::move(other._glfwWindow)),
	_width(other._width),
	_height(other._height),
	_frameWidth(other._frameWidth),
	_frameHeight(other._frameHeight),
	_pressedMouseButtons(other._pressedMouseButtons) {
#if not CE_PLATFORM_MACOS
	_windowedX = other._windowedX;
	_windowedY = other._windowedY;
	_windowedWidth = other._windowedWidth;
	_windowedHeight = other._windowedHeight;
#endif

	_AdoptNativeWindow();
}

GlfwWindow::~GlfwWindow() {
	_Shutdown();
}

GlfwWindow& GlfwWindow::operator = (GlfwWindow&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	// Release the window this object currently owns before taking over other's, otherwise that handle would leak.
	_Shutdown();

	windowEventHandler = other.windowEventHandler;
	_glfwWindow = std::move(other._glfwWindow);
	_width = other._width;
	_height = other._height;
	_frameWidth = other._frameWidth;
	_frameHeight = other._frameHeight;
	_pressedMouseButtons = other._pressedMouseButtons;

#if not CE_PLATFORM_MACOS
	_windowedX = other._windowedX;
	_windowedY = other._windowedY;
	_windowedWidth = other._windowedWidth;
	_windowedHeight = other._windowedHeight;
#endif

	_AdoptNativeWindow();

	return *this;
}

void GlfwWindow::Show() {
	if (const auto window = _Native()) [[likely]]
		glfwShowWindow(window);
}

void GlfwWindow::Miniaturize() {
	if (const auto window = _Native()) [[likely]]
		glfwIconifyWindow(window);
}

void GlfwWindow::Deminiaturize() {
	if (const auto window = _Native()) [[likely]]
		glfwRestoreWindow(window);
}

void GlfwWindow::ToggleFullScreen() {
	const auto window = _Native();
	if (not window) [[unlikely]]
		return;

#if CE_PLATFORM_MACOS
	// GLFW hosts a real NSWindow on macOS: use the native (Spaces) fullscreen so the transition animates and keeps the
	// title-bar controls, instead of the exclusive glfwSetWindowMonitor fullscreen that covers them and hides the traffic lights.
	const auto nsWindow = reinterpret_cast<NS::Window*>(glfwGetCocoaWindow(window));
	if (not nsWindow) [[unlikely]] {
		CE_CORE_WARN("GlfwWindow::ToggleFullScreen: Cannot toggle fullscreen because the native Cocoa window is unavailable");
		return;
	}

	// Opt the window into native fullscreen so toggleFullScreen animates into its own Space.
	nsWindow->setCollectionBehavior(nsWindow->collectionBehavior() | NS::WindowCollectionBehaviorFullScreenPrimary);
	nsWindow->toggleFullScreen(nullptr);
#else
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

void GlfwWindow::MakeContextCurrent() {
	if (const auto window = _Native()) [[likely]]
		glfwMakeContextCurrent(window);
}

void GlfwWindow::SwapBuffers() {
	if (const auto window = _Native()) [[likely]]
		glfwSwapBuffers(window);
}

void GlfwWindow::SetWindowSize(const u32 width, const u32 height) {
	if (const auto window = _Native()) [[likely]]
		glfwSetWindowSize(window, static_cast<int>(width), static_cast<int>(height));
}

void GlfwWindow::SetTitle(const std::string_view title) {
	const auto window = _Native();
	if (not window) [[unlikely]]
		return;

	// glfwSetWindowTitle takes a C string, and a string_view carries no terminator of its own.
	const std::string terminated(title);
	glfwSetWindowTitle(window, terminated.c_str());
}

void GlfwWindow::SetSwapInterval(const i32 interval) {
	const auto window = _Native();
	if (not window) [[unlikely]]
		return;

	glfwMakeContextCurrent(window);
	glfwSwapInterval(interval);
}

f32 GlfwWindow::GetContentScale() const {
	const auto window = _Native();
	if (not window) [[unlikely]]
		return 1.0f;

	f32 xScale = 1.0f;
	f32 yScale = 1.0f;
	glfwGetWindowContentScale(window, &xScale, &yScale);

	// The two axes only differ on configurations the engine does not target, and every caller wants a single factor.
	return xScale;
}

u32 GlfwWindow::GetRefreshRate() const {
	const auto window = _Native();
	if (not window) [[unlikely]]
		return 0;

	// When fullscreen the window owns a monitor; otherwise fall back to the primary monitor.
	auto monitor = glfwGetWindowMonitor(window);
	if (not monitor) [[unlikely]]
		monitor = glfwGetPrimaryMonitor();
	if (not monitor) [[unlikely]]
		return 0;

	const auto mode = glfwGetVideoMode(monitor);
	return mode ? static_cast<u32>(mode->refreshRate) : 0;
}

void GlfwWindow::_InitWindow(const GlfwPlatform& platform) {
	const auto& windowProps = Utility::Config::StGetWindowProps();

	platform.ApplyWindowHints(windowProps.graphicsApi);

	_glfwWindow.reset(glfwCreateWindow(
		static_cast<int>(windowProps.width),
		static_cast<int>(windowProps.height),
		windowProps.title.c_str(),
		nullptr,
		nullptr
	));

	if (not _glfwWindow) [[unlikely]] {
		constexpr auto error = "GlfwWindow::_InitWindow: Failed to create GLFW window!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_AdoptNativeWindow();
	_CacheSizes();
}

void GlfwWindow::_Shutdown() {
	_glfwWindow.reset();
}

void GlfwWindow::_AdoptNativeWindow() {
	if (not _glfwWindow) [[unlikely]]
		return;

	glfwSetWindowUserPointer(_glfwWindow.get(), this);
}

GLFWwindow* GlfwWindow::_Native(const std::source_location& location) const {
	if (not _glfwWindow) [[unlikely]] {
		CE_CORE_WARN("GlfwWindow::{}: the window is not initialized.", location.function_name());
		return nullptr;
	}

	return _glfwWindow.get();
}

int GlfwWindow::_CurrentModifiers() const {
	const auto window = _Native();
	if (not window) [[unlikely]]
		return 0;

	const auto held = [window](const int leftKey, const int rightKey) {
		return glfwGetKey(window, leftKey) == GLFW_PRESS or glfwGetKey(window, rightKey) == GLFW_PRESS;
	};

	int modifiers = 0;
	if (held(GLFW_KEY_LEFT_SHIFT, GLFW_KEY_RIGHT_SHIFT))
		modifiers |= GLFW_MOD_SHIFT;
	if (held(GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL))
		modifiers |= GLFW_MOD_CONTROL;
	if (held(GLFW_KEY_LEFT_ALT, GLFW_KEY_RIGHT_ALT))
		modifiers |= GLFW_MOD_ALT;
	if (held(GLFW_KEY_LEFT_SUPER, GLFW_KEY_RIGHT_SUPER))
		modifiers |= GLFW_MOD_SUPER;

	// GLFW only reports the lock keys when GLFW_LOCK_KEY_MODS is enabled, which it is not, so they are left out here too.
	return modifiers;
}

void GlfwWindow::_SetIOEventCallbacks() const {
	const auto window = _Native();
	if (not window) [[unlikely]]
		return;

	glfwSetKeyCallback(window, [](GLFWwindow* nativeWindow, const int key, const int scancode, const int action, const int mods) {
		if (const auto self = WindowFrom(nativeWindow)) [[likely]]
			self->windowEventHandler.DispatchKeyEvent(key, action, scancode, mods);
	});

	glfwSetCharCallback(window, [](GLFWwindow* nativeWindow, const unsigned int codepoint) {
		if (const auto self = WindowFrom(nativeWindow)) [[likely]]
			self->windowEventHandler.DispatchCharEvent(codepoint);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* nativeWindow, const int button, const int action, const int mods) {
		const auto self = WindowFrom(nativeWindow);
		if (not self) [[unlikely]]
			return;

		// GLFW has no native drag event, so the held buttons are tracked here to synthesize one on cursor motion.
		if (button >= 0 and button < 8) {
			const auto mask = static_cast<u8>(1u << button);
			self->_pressedMouseButtons = action == GLFW_PRESS
				? static_cast<u8>(self->_pressedMouseButtons | mask)
				: static_cast<u8>(self->_pressedMouseButtons & ~mask);
		}

		self->windowEventHandler.DispatchMouseButtonEvent(button, action, mods);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* nativeWindow, const f64 xPos, const f64 yPos) {
		const auto self = WindowFrom(nativeWindow);
		if (not self) [[unlikely]]
			return;

		self->windowEventHandler.DispatchMousePositionEvent(xPos, yPos);

		// Nothing held: no drag to synthesize, which is every plain cursor move.
		if (self->_pressedMouseButtons == 0) [[likely]]
			return;

		const auto modifiers = self->_CurrentModifiers();
		for (int button = 0; button < 8; ++button) {
			if (self->_pressedMouseButtons & static_cast<u8>(1u << button))
				self->windowEventHandler.DispatchMouseDraggedEvent(button, GLFW_PRESS, modifiers, xPos, yPos);
		}
	});

	glfwSetScrollCallback(window, [](GLFWwindow* nativeWindow, const f64 xOffset, const f64 yOffset) {
		if (const auto self = WindowFrom(nativeWindow)) [[likely]]
			self->windowEventHandler.DispatchMouseWheelScrollEvent(xOffset, yOffset);
	});
}

void GlfwWindow::_SetWindowEventCallbacks() const {
	const auto window = _Native();
	if (not window) [[unlikely]]
		return;

	glfwSetWindowSizeCallback(window, [](GLFWwindow* nativeWindow, const int width, const int height) {
		const auto self = WindowFrom(nativeWindow);
		if (not self) [[unlikely]]
			return;

		// Refresh the cache rather than calling back into GLFW: the window already has this size, it is why the
		// callback fired.
		self->_width = static_cast<u32>(width);
		self->_height = static_cast<u32>(height);

		self->windowEventHandler.DispatchResizeEvent(width, height);
	});

	// The framebuffer has a callback of its own because it can change without the window size changing - dragging the
	// window onto a display with a different scale factor resizes the drawable and nothing else.
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* nativeWindow, const int width, const int height) {
		if (const auto self = WindowFrom(nativeWindow)) [[likely]] {
			self->_frameWidth = static_cast<u32>(width);
			self->_frameHeight = static_cast<u32>(height);
		}
	});

	glfwSetWindowCloseCallback(window, [](GLFWwindow* nativeWindow) {
		if (const auto self = WindowFrom(nativeWindow)) [[likely]]
			self->windowEventHandler.DispatchCloseEvent();
	});

	glfwSetWindowFocusCallback(window, [](GLFWwindow* nativeWindow, const int focused) {
		if (const auto self = WindowFrom(nativeWindow)) [[likely]]
			self->windowEventHandler.DispatchFocusEvent(focused);
	});
}

void GlfwWindow::_CacheSizes() {
	const auto window = _Native();
	if (not window) [[unlikely]]
		return;

	int width = 0;
	int height = 0;

	glfwGetWindowSize(window, &width, &height);
	_width = static_cast<u32>(width);
	_height = static_cast<u32>(height);

	glfwGetFramebufferSize(window, &width, &height);
	_frameWidth = static_cast<u32>(width);
	_frameHeight = static_cast<u32>(height);
}

}
