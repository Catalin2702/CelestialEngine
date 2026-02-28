//
// Module: Window//Platforms/Universal
// File: OpenGLViewport.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#include "Window/Platforms/Universal/OpenGLViewport.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Window/WindowProps.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace CE::Window {

static bool s_GLFWInitialized = false;

OpenGLViewport::OpenGLViewport(const CeTypeWindow::WindowProps &windowProps): _data(windowProps.title, windowProps.width, windowProps.height, windowProps.VSync)
{
	_Init();
}

OpenGLViewport::~OpenGLViewport() {
	_Shutdown();
}

void OpenGLViewport::OnUpdate() {
	glfwPollEvents();
	glfwSwapBuffers(_glfwWindow.get());
}

void OpenGLViewport::SetEventCallback(const EventCallbackFn &callback) {
	_data.eventCallback = callback;
}

void OpenGLViewport::SetWindowCallbacks() {
	if (not _glfwWindow)
		return;

	glfwSetWindowSizeCallback(_glfwWindow.get(), [](GLFWwindow* window, const int width, const int height) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			data->width = static_cast<unsigned int>(width);
			data->height = static_cast<unsigned int>(height);
			Events::WindowResizeEvent event{data->width, data->height};
			data->eventCallback(event);
		}
	});

	glfwSetWindowCloseCallback(_glfwWindow.get(), [](GLFWwindow* window) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			Events::WindowCloseEvent event;
			data->eventCallback(event);
		}
	});

	glfwSetKeyCallback(_glfwWindow.get(), [](GLFWwindow* window, const int key, const int, const int action, const int) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			switch (action) {
				case GLFW_PRESS: {
					Events::KeyPressedEvent keyPressedEvent{key, 0};
					data->eventCallback(keyPressedEvent);
					break;
				}

				case GLFW_RELEASE: {
					Events::KeyReleasedEvent keyReleasedEvent{key};
					data->eventCallback(keyReleasedEvent);
					break;
				}

				case GLFW_REPEAT: {
					Events::KeyPressedEvent keyPressedEvent{key, 1};
					data->eventCallback(keyPressedEvent);
					break;
				}
				default:;
			}
		}
	});

	glfwSetCharCallback(_glfwWindow.get(), [](GLFWwindow* window, const unsigned int keycode) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			Events::KeyTypedEvent keyTypedEvent{keycode};
			data->eventCallback(keyTypedEvent);
		}
	});

	glfwSetMouseButtonCallback(_glfwWindow.get(), [](GLFWwindow* window, const int button, const int action, const int) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			switch (action) {
				case GLFW_PRESS: {
					Events::MouseButtonPressedEvent mouseButtonPressedEvent(button);
					data->eventCallback(mouseButtonPressedEvent);
					break;
				}
				case GLFW_RELEASE: {
					Events::MouseButtonReleasedEvent mouseButtonReleasedEvent(button);
					data->eventCallback(mouseButtonReleasedEvent);
					break;
				}
				default:;
			}
		}
	});

	glfwSetScrollCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xOffset, const double yOffset) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			Events::MouseScrolledEvent mouseScrolledEvent{static_cast<float>(xOffset), static_cast<float>(yOffset)};
			data->eventCallback(mouseScrolledEvent);
		}
	});

	glfwSetCursorPosCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xPos, const double yPos) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			Events::MouseMovedEvent mouseMovedEvent{static_cast<float>(xPos), static_cast<float>(yPos)};
			data->eventCallback(mouseMovedEvent);
		}
	});
}

void OpenGLViewport::SetWidth(const unsigned int width) {
	_data.width = width;
}

void OpenGLViewport::SetHeight(const unsigned int height) {
	_data.height = height;
}

void OpenGLViewport::SetVSync(const bool enabled) {
	if (not s_GLFWInitialized) {
		CE_CORE_WARN("Could not set VSync because GLFW is not initialized.");
		return;
	}
	_data.VSync = enabled;
	glfwSwapInterval(enabled ? 1 : 0);
}

void OpenGLViewport::_Init() {

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

	glfwMakeContextCurrent(_glfwWindow.get());
	if (const int gladStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)); not gladStatus) {
		CE_CORE_ERROR("Error to initialize GLAD");
		exit(EXIT_FAILURE);
	}
	glfwSetWindowUserPointer(_glfwWindow.get(), &_data);
	SetVSync(_data.VSync);
	SetWindowCallbacks();
}

void OpenGLViewport::_Shutdown() {
	_glfwWindow.reset();
}

}
