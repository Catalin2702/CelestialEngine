//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Common
// File: GlfwWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-29
//

#include "Window/Platforms/Common/GlfwWindow.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"
#include "Types/Window/WindowProps.hpp"

#include <GLFW/glfw3.h>

#include <stdexcept>
#include <utility>


namespace CE::Window {

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

GlfwWindow::GlfwWindow(TypeWindow::WindowProps windowProps): _data(std::move(windowProps)){
	_Init();
}

GlfwWindow::~GlfwWindow() {
	_Shutdown();
}

void GlfwWindow::OnUpdate() const {
	glfwPollEvents();
}

float GlfwWindow::GetWindowWidth() const {
	int width = 0, height = 0;
	glfwGetWindowSize(_glfwWindow.get(), &width, &height);
	return static_cast<float>(width);
}

float GlfwWindow::GetWindowHeight() const {
	int width = 0, height = 0;
	glfwGetWindowSize(_glfwWindow.get(), &width, &height);
	return static_cast<float>(height);
}

std::pair<float, float> GlfwWindow::GetWindowSize() const {
	int width = 0, height = 0;
	glfwGetWindowSize(_glfwWindow.get(), &width, &height);
	return {static_cast<float>(width), static_cast<float>(height)};
}

float GlfwWindow::GetMonitorWidth() const {
	if (const auto monitor = glfwGetPrimaryMonitor()) {
		if (const auto videoMode = glfwGetVideoMode(monitor)) {
			return static_cast<float>(videoMode->width);
		}
	}
	return 0.f;
}

float GlfwWindow::GetMonitorHeight() const {
	if (const auto monitor = glfwGetPrimaryMonitor()) {
		if (const auto videoMode = glfwGetVideoMode(monitor)) {
			return static_cast<float>(videoMode->height);
		}
	}
	return 0.f;
}

std::pair<float, float> GlfwWindow::GetMonitorSize() const {
	if (const auto monitor = glfwGetPrimaryMonitor()) {
		if (const auto videoMode = glfwGetVideoMode(monitor)) {
			return {static_cast<float>(videoMode->width), static_cast<float>(videoMode->height)};
		}
	}
	return {0.f, 0.f};
}

std::pair<float, float> GlfwWindow::GetContentScale() const {
	if (not _glfwWindow) {
		CE_CORE_WARN("Could not get content scale because window is not initialized.");
		return {1.0f, 1.0f};
	}
	float xScale = 1.0f, yScale = 1.0f;
	glfwGetWindowContentScale(_glfwWindow.get(), &xScale, &yScale);
	return {xScale, yScale};
}

std::pair<float, float> GlfwWindow::GetContentSize() const {
	if (not _glfwWindow) {
		CE_CORE_WARN("Could not get content size because window is not initialized.");
		return {static_cast<float>(_data.width), static_cast<float>(_data.height)};
	}
	const auto [xScale, yScale] = GetContentScale();
	return {static_cast<float>(_data.width) * xScale, static_cast<float>(_data.height) * yScale};
}

std::pair<int, int> GlfwWindow::GetFrameBufferSize() const {
	if (not _glfwWindow) {
		CE_CORE_WARN("Could not get framebuffer size because window is not initialized.");
		return {static_cast<int>(_data.width), static_cast<int>(_data.height)};
	}
	int width = 0, height = 0;
	glfwGetFramebufferSize(_glfwWindow.get(), &width, &height);
	return {width, height};
}

double GlfwWindow::GetTime() {
	return glfwGetTime();
}

void GlfwWindow::SetEventCallback(const EventCallbackFn& callback) {
	_callbacks.EventCallback = callback;
}

void GlfwWindow::_SetIOEventCallbacks() {
	if (not _glfwWindow)
		return;

	glfwSetKeyCallback(_glfwWindow.get(), [](GLFWwindow* window, const int key, const int, const int action, const int) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
			switch (action) {
				case GLFW_PRESS: {
					Events::KeyPressedEvent keyPressedEvent{KeyCode::KeyboardKeyCodeFromGlfw(key), 0};
					windowCallbacks->EventCallback(keyPressedEvent);
					break;
				}

				case GLFW_RELEASE: {
					Events::KeyReleasedEvent keyReleasedEvent{KeyCode::KeyboardKeyCodeFromGlfw(key)};
					windowCallbacks->EventCallback(keyReleasedEvent);
					break;
				}

				case GLFW_REPEAT: {
					Events::KeyPressedEvent keyPressedEvent{KeyCode::KeyboardKeyCodeFromGlfw(key), 1};
					windowCallbacks->EventCallback(keyPressedEvent);
					break;
				}
				default:;
			}
		}
	});

	glfwSetCharCallback(_glfwWindow.get(), [](GLFWwindow* window, const unsigned int keycode) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
			Events::KeyTypedEvent keyTypedEvent{KeyCode::KeyboardCharsCodeFromGlfw(keycode)};
			windowCallbacks->EventCallback(keyTypedEvent);
		}
	});

	glfwSetMouseButtonCallback(_glfwWindow.get(), [](GLFWwindow* window, const int button, const int action, const int) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
			switch (action) {
				case GLFW_PRESS: {
					Events::MouseButtonPressedEvent mouseButtonPressedEvent{KeyCode::MouseButtonKeyCodeFromGlfw(button)};
					windowCallbacks->EventCallback(mouseButtonPressedEvent);
					break;
				}
				case GLFW_RELEASE: {
					Events::MouseButtonReleasedEvent mouseButtonReleasedEvent{KeyCode::MouseButtonKeyCodeFromGlfw(button)};
					windowCallbacks->EventCallback(mouseButtonReleasedEvent);
					break;
				}
				default:;
			}
		}
	});

	glfwSetScrollCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xOffset, const double yOffset) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
			Events::MouseScrolledEvent mouseScrolledEvent{static_cast<float>(xOffset), static_cast<float>(yOffset)};
			windowCallbacks->EventCallback(mouseScrolledEvent);
		}
	});

	glfwSetCursorPosCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xPos, const double yPos) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
			Events::MouseMovedEvent mouseMovedEvent{static_cast<float>(xPos), static_cast<float>(yPos)};
			windowCallbacks->EventCallback(mouseMovedEvent);
		}
	});
}

void GlfwWindow::_SetWindowEventCallbacks() {
	if (not _glfwWindow)
		return;

	glfwSetWindowSizeCallback(_glfwWindow.get(), [](GLFWwindow* window, const int width, const int height) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
			Events::WindowResizeEvent event{static_cast<unsigned int>(width), static_cast<unsigned int>(height)};
			windowCallbacks->EventCallback(event);
			windowCallbacks->_internalCallbacks.ResizeEventCallback(event);
		}
	});

	glfwSetWindowCloseCallback(_glfwWindow.get(), [](GLFWwindow* window) {
		if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
			Events::WindowCloseEvent event;
			windowCallbacks->EventCallback(event);
		}
	});
}

void GlfwWindow::_SetInternalCallbacks() {
	auto& [ResizeEventCallback] = _callbacks._internalCallbacks;

	ResizeEventCallback = [this](const Events::WindowResizeEvent& event) {
		SetSize(event.GetWidth(), event.GetHeight());
	};
}

void GlfwWindow::SetWidth(const unsigned int width) {
	_data.width = width;
	glfwSetWindowSize(_glfwWindow.get(), static_cast<int>(_data.width), static_cast<int>(_data.height));
}

void GlfwWindow::SetHeight(const unsigned int height) {
	_data.height = height;
	glfwSetWindowSize(_glfwWindow.get(), static_cast<int>(_data.width), static_cast<int>(_data.height));
}

void GlfwWindow::SetSize(const unsigned int width, const unsigned int height) {
	_data.width = width;
	_data.height = height;
	glfwSetWindowSize(_glfwWindow.get(), static_cast<int>(_data.width), static_cast<int>(_data.height));
}

void GlfwWindow::SetVSync(const bool enabled) {
	if (not _st_GLFWInitialized) {
		CE_CORE_WARN("Could not set VSync because GLFW is not initialized.");
		return;
	}
	_data.VSync = enabled;
	glfwSwapInterval(enabled ? 1 : 0);
}

void GlfwWindow::SetCurrentContext(GLFWwindow* window) const {
	const auto ptr = window ? window : _glfwWindow.get();
	glfwMakeContextCurrent(ptr);
}

void GlfwWindow::GetReady() {
	if (not _st_GLFWInitialized) {
		CE_CORE_WARN("Could not set VSync because GLFW is not initialized.");
		return;
	}
	SetVSync(_data.VSync);
}

void GlfwWindow::_Init() {
	_InitWindow();

	_SetIOEventCallbacks();
	_SetWindowEventCallbacks();
	_SetInternalCallbacks();

	_st_GLFWWindowCount++;
}

void GlfwWindow::_InitWindow() {
	if (not _st_GLFWInitialized) {
		if (const int success = glfwInit(); not success) {
			CE_CORE_ERROR("Could not initialize GLFW!");
			throw std::runtime_error("Could not initialize GLFW!");
		}
		glfwSetErrorCallback([]([[maybe_unused]] const int error_code, [[maybe_unused]] const char* description) {
			CE_CORE_ERROR("GLFW error: {0}\nDescription: {1}", error_code, description);
		});
		_st_GLFWInitialized = true;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	_glfwWindow.reset(glfwCreateWindow(
		static_cast<int>(_data.width),
		static_cast<int>(_data.height),
		_data.title.c_str(),
		nullptr,
		nullptr
	));

	if (not _glfwWindow) {
		CE_CORE_ERROR("Failed to create GLFW window!");
		throw std::runtime_error("Failed to create GLFW window!");
	}

	glfwSetWindowUserPointer(_glfwWindow.get(), &_callbacks);
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
