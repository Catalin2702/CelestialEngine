//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Common/Glfw
// File: GlfwWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"

#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Utility/Utility.hpp"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <utility>

namespace CE::Core {

/**
 * @brief Static flag to track if GLFW has already been initialized
 * @details Ensures that glfwInit() is called only once during program execution
 */
static bool _st_GLFWInitialized = false;

/**
 * @brief Static counter to track the number of active windows
 * @details Used to determine when to call glfwTerminate() (when count reaches 0)
 */
static int _st_GLFWWindowCount = 0;

/**
 * @brief Flag to track the current VSync state
 * @details Updated by SetVSync() and returned by IsVSync()
 */
bool _glfwVSync = false;

void GlfwWindowEventHandler::DispatchResizeEvent(const int width, const int height) const {
	windowStateEvents.onResizeDispatcher.Dispatch(width, height);
}

void GlfwWindowEventHandler::DispatchCloseEvent() const {
	windowStateEvents.onCloseDispatcher.Dispatch();
}

void GlfwWindowEventHandler::DispatchErrorEvent(const int errorCode, const char* description) const {
	windowStateEvents.onErrorDispatcher.Dispatch(errorCode, description);
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

void GlfwWindowEventHandler::DispatchMousePositionEvent(const double xPos, const double yPos) const {
	mouseEvents.onMousePositionDispatcher.Dispatch(xPos, yPos);
}

void GlfwWindowEventHandler::DispatchMouseDraggedEvent(const int button, const int action, const int mods, const double xPos, const double yPos) const {
	mouseEvents.onMouseDraggedDispatcher.Dispatch(button, action, mods, xPos, yPos);
}

void GlfwWindowEventHandler::DispatchMouseWheelScrollEvent(const double xOffset, const double yOffset) const {
	mouseEvents.onMouseWheelScrollDispatcher.Dispatch(xOffset, yOffset);
}

GlfwWindow::GlfwWindow() {
	Init();
}

GlfwWindow::~GlfwWindow() {
	_Shutdown();
}

void GlfwWindow::OnUpdate() const {
	glfwPollEvents();
}

void GlfwWindow::GetReady(const bool VSync) {
	if (not _st_GLFWInitialized) {
		CE_CORE_WARN("Could not set VSync because GLFW is not initialized.");
		return;
	}
	SetVSync(VSync);
}

std::pair<float, float> GlfwWindow::GetWindowSize() const {
	int width = 0, height = 0;
	glfwGetWindowSize(_glfwWindow.get(), &width, &height);
	return {static_cast<float>(width), static_cast<float>(height)};
}

std::pair<float, float> GlfwWindow::GetFrameSize() const {
	if (not _glfwWindow) {
		CE_CORE_WARN("GlfwWindow::GetFrameSize Could not get frame size because window is not initialized.");
		return {0, 0};
	}
	int width = 0, height = 0;
	glfwGetFramebufferSize(_glfwWindow.get(), &width, &height);
	return {width, height};
}

bool GlfwWindow::IsVSync() const {
	if (not _glfwWindow) {
		CE_CORE_WARN("GlfwWindow::IsVSync: Could not get VSync state because window is not initialized.");
		return false;
	}

	return _glfwVSync;
}

void GlfwWindow::SetSize(const unsigned int width, const unsigned int height) {
	glfwSetWindowSize(_glfwWindow.get(), static_cast<int>(width), static_cast<int>(height));
}

void GlfwWindow::SetVSync(const bool enabled) {
	if (not _st_GLFWInitialized) {
		CE_CORE_WARN("Could not set VSync because GLFW is not initialized.");
		return;
	}
	_glfwVSync = enabled;
	glfwSwapInterval(_glfwVSync ? 1 : 0);
}

void GlfwWindow::SetCurrentContext(GLFWwindow* window) const {
	const auto ptr = window ? window : _glfwWindow.get();
	glfwMakeContextCurrent(ptr);
}

void GlfwWindow::_SetIOEventCallbacks() {
	if (not _glfwWindow)
		return;

	glfwSetKeyCallback(_glfwWindow.get(), [](GLFWwindow* window, const int key, const int scancode, const int action, const int mods) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) {
			_this->windowEventHandler.DispatchKeyEvent(key, action, scancode, mods);
		}
	});

	glfwSetCharCallback(_glfwWindow.get(), [](GLFWwindow* window, const unsigned int codepoint) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) {
			_this->windowEventHandler.DispatchCharEvent(codepoint);
		}
	});

	glfwSetMouseButtonCallback(_glfwWindow.get(), [](GLFWwindow* window, const int button, const int action, const int mods) {
		// GLFW has no native drag event, so we track button state here to synthesize
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) {
			if (button >= 0 && button < static_cast<int>(_this->_pressedMouseButtons.size()))
				_this->_pressedMouseButtons[button] = (action == GLFW_PRESS);
			_this->_lastMouseMods = mods;

			_this->windowEventHandler.DispatchMouseButtonEvent(button, action, mods);
		}
	});

	glfwSetCursorPosCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xPos, const double yPos) {
		// if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
		// 	Events::MouseMovedEvent mouseMovedEvent{static_cast<float>(xPos), static_cast<float>(yPos)};
		// 	windowCallbacks->EventCallback(mouseMovedEvent);
		// }
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) {
			_this->windowEventHandler.DispatchMousePositionEvent(xPos, yPos);

			// GLFW has no native drag event: if the cursor moves while a mouse button is
			// held, synthesize a drag event for each pressed button.
			for (int button = 0; button < static_cast<int>(_this->_pressedMouseButtons.size()); ++button) {
				if (_this->_pressedMouseButtons[button])
					_this->windowEventHandler.DispatchMouseDraggedEvent(button, GLFW_PRESS, _this->_lastMouseMods, xPos, yPos);
			}
		}
	});

	glfwSetScrollCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xOffset, const double yOffset) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) {
			_this->windowEventHandler.DispatchMouseWheelScrollEvent(xOffset, yOffset);
		}
	});
}

void GlfwWindow::_SetWindowEventCallbacks() {
	if (not _glfwWindow)
		return;

	glfwSetWindowSizeCallback(_glfwWindow.get(), [](GLFWwindow* window, const int width, const int height) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) {
			_this->windowEventHandler.DispatchResizeEvent(width, height);
			_this->SetSize(width, height);
		}
	});

	glfwSetWindowCloseCallback(_glfwWindow.get(), [](GLFWwindow* window) {
		if (const auto _this = static_cast<GlfwWindow*>(glfwGetWindowUserPointer(window))) {
			_this->windowEventHandler.DispatchCloseEvent();
		}
	});
}

void GlfwWindow::Init() {
	_InitWindow();

	_SetIOEventCallbacks();
	_SetWindowEventCallbacks();

	_st_GLFWWindowCount++;
}

void GlfwWindow::_InitWindow() {
	if (not _st_GLFWInitialized) {
		if (const int success = glfwInit(); not success) {
			const auto error = "GlfwWindow::_InitWindow: Could not initialize GLFW!";
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

	const auto& windowProps = Utility::Config::Config::StGetWindowProps();

	_glfwWindow.reset(glfwCreateWindow(
		static_cast<int>(windowProps.width),
		static_cast<int>(windowProps.height),
		windowProps.title.c_str(),
		nullptr,
		nullptr
	));

	if (not _glfwWindow) {
		const auto error = "GlfwWindow::_InitWindow: Failed to create GLFW window!";
		windowEventHandler.DispatchErrorEvent(-1, error);
		throw std::runtime_error(error);
	}

	glfwSetWindowUserPointer(_glfwWindow.get(), this);
}

void GlfwWindow::_Shutdown() {
	_glfwWindow.reset();

	_st_GLFWWindowCount--;
	if (_st_GLFWWindowCount == 0 && _st_GLFWInitialized) {
		glfwTerminate();
		_st_GLFWInitialized = false;
	}
}

}
