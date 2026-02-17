//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Window/Platforms/Mac/Window.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"

#include <GLFW/glfw3.h>


namespace CeEvents = CE::Events;

namespace CE::Window {

static bool s_GLFWInitialized = false;

PlatformWindow::PlatformWindow(const WindowProps &windowProps)
{
	Init(windowProps);
}

PlatformWindow::~PlatformWindow() {
	Shutdown();
}

void PlatformWindow::OnUpdate() {
	glfwPollEvents();
	glfwSwapBuffers(_window);
}

void PlatformWindow::SetEventCallback(const EventCallbackFn &callback) {
	_data.eventCallback = callback;
}

void PlatformWindow::SetWindowCallbacks() {
	if (not _window)
		return;

	glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, const int width, const int height) {
		if (const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window))) {
			data->width = static_cast<unsigned int>(width);
			data->height = static_cast<unsigned int>(height);
			CeEvents::WindowResizeEvent event{data->width, data->height};
			data->eventCallback(event);
		}
	});

	glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
		if (const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window))) {
			CeEvents::WindowCloseEvent event;
			data->eventCallback(event);
		}
	});

	glfwSetKeyCallback(_window, [](GLFWwindow* window, const int key, const int, const int action, const int) {
		if (const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window))) {
			switch (action) {
				case GLFW_PRESS: {
					CeEvents::KeyPressedEvent keyPressedEvent{key, 0};
					data->eventCallback(keyPressedEvent);
					break;
				}

				case GLFW_RELEASE: {
					CeEvents::KeyReleasedEvent keyReleasedEvent{key};
					data->eventCallback(keyReleasedEvent);
					break;
				}

				case GLFW_REPEAT: {
					CeEvents::KeyPressedEvent keyPressedEvent{key, 1};
					data->eventCallback(keyPressedEvent);
					break;
				}
				default:
					return;
			}
		}
	});

	glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, const int button, const int action, const int) {
		if (const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window))) {
			switch (action) {
				case GLFW_PRESS: {
					CeEvents::MouseButtonPressedEvent mouseButtonPressedEvent(button);
					data->eventCallback(mouseButtonPressedEvent);
					break;
				}
				case GLFW_RELEASE: {
					CeEvents::MouseButtonReleasedEvent mouseButtonReleasedEvent(button);
					data->eventCallback(mouseButtonReleasedEvent);
					break;
				}
				default:
					return;
			}
		}
	});

	glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset) {
		if (const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window))) {
			CeEvents::MouseScrolledEvent mouseScrolledEvent{static_cast<float>(xOffset), static_cast<float>(yOffset)};
			data->eventCallback(mouseScrolledEvent);
		}
	});

	glfwSetCursorPosCallback(_window, [](GLFWwindow* window, const double xPos, const double yPos) {
		if (const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window))) {
			CeEvents::MouseMovedEvent mouseMovedEvent{static_cast<float>(xPos), static_cast<float>(yPos)};
			data->eventCallback(mouseMovedEvent);
		}
	});
}

void PlatformWindow::SetWidth(const unsigned int width) {
	_data.width = width;
}

void PlatformWindow::SetHeight(const unsigned int height) {
	_data.height = height;
}

void PlatformWindow::SetVSync(const bool enabled) {
	if (not s_GLFWInitialized) {
		CE_CORE_WARN("Could not set VSync because GLFW is not initialized.");
		return;
	}
	_data.VSync = enabled;
	glfwSwapInterval(enabled ? 1 : 0);
}

void PlatformWindow::Init(const WindowProps &windowProps) {
	_data.title = windowProps.title;
	_data.width = windowProps.width;
	_data.height = windowProps.height;
	_data.VSync = windowProps.VSync;

	CE_INFO("Creating window {0}, ({1}x{2}), VSync: {3}", _data.title, _data.width, _data.height, _data.VSync);

	if (not s_GLFWInitialized) {
		if (const int success = glfwInit(); not success) {
			CE_CORE_ERROR("Could not initialize GLFW!");
		}
		glfwSetErrorCallback([]([[maybe_unused]] const int error_code, [[maybe_unused]] const char* description) {
			CE_CORE_ERROR("GLFW error: {0}\nDescription: {1}", error_code, description);
		});
		s_GLFWInitialized = true;
	}

	_window = glfwCreateWindow(
		static_cast<int>(_data.width),
		static_cast<int>(_data.height),
		_data.title.c_str(),
		nullptr,
		nullptr
	);

	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, &_data);
	SetVSync(_data.VSync);
	SetWindowCallbacks();
}

void PlatformWindow::Shutdown() {
	glfwDestroyWindow(_window);
	_window = nullptr;
}

Window *Window::CreateWindow(const WindowProps &windowProps) {
	return new PlatformWindow(windowProps);
}

}
