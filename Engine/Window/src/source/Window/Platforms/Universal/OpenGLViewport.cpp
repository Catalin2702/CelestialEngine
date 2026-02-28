//
// Module: Window/Platforms/Universal
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

/**
 * @brief Static flag to track if GLFW has already been initialized
 * @details Ensures that glfwInit() is called only once during program execution
 */
static bool s_GLFWInitialized = false;

/**
 * @brief Constructor of the OpenGLViewport class
 * @param windowProps Constant reference to window properties (title, width, height, VSync)
 * @details Initializes window data with the provided properties and calls _Init() to create
 *          and configure the GLFW window with OpenGL 4.1 Core Profile context
 */
OpenGLViewport::OpenGLViewport(const TypeWindow::WindowProps& windowProps): _data(windowProps.title, windowProps.width, windowProps.height, windowProps.VSync){
	_Init();
}

/**
 * @brief Destructor of the OpenGLViewport class
 * @details Calls _Shutdown() to release GLFW window resources and clean up state
 *          before the object is destroyed
 */
OpenGLViewport::~OpenGLViewport() {
	_Shutdown();
}

/**
 * @brief Updates the window state every frame
 * @details Processes all queued events via glfwPollEvents() and swaps front/back buffers
 *          with glfwSwapBuffers() to display the rendered content
 */
void OpenGLViewport::OnUpdate() {
	glfwPollEvents();
	glfwSwapBuffers(_glfwWindow.get());
}

/**
 * @brief Sets the callback function for event handling
 * @param callback Constant reference to a callback function that will be invoked when events occur
 * @details This function will be called whenever an event occurs (resize, close,
 *          keyboard input, mouse, etc.)
 */
void OpenGLViewport::SetEventCallback(const EventCallbackFn& callback) {
	_data.eventCallback = callback;
}

/**
 * @brief Configures all GLFW callbacks for window event handling
 * @details Registers the following callbacks:
 *          - Window resize: generates WindowResizeEvent
 *          - Window close: generates WindowCloseEvent
 *          - Keyboard input (press/release/repeat): generates KeyPressedEvent and KeyReleasedEvent
 *          - Typed characters: generates KeyTypedEvent for text input
 *          - Mouse buttons (press/release): generates MouseButtonPressedEvent and MouseButtonReleasedEvent
 *          - Mouse scroll: generates MouseScrolledEvent
 *          - Mouse movement: generates MouseMovedEvent
 *          Verifies that _glfwWindow is valid before registering callbacks
 */
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

/**
 * @brief Sets the window width
 * @param width New window width in pixels
 * @details Updates the internal value in the _data structure. Note: this method only updates
 *          the stored value, it does not actually resize the window
 */
void OpenGLViewport::SetWidth(const unsigned int width) {
	_data.width = width;
}

/**
 * @brief Sets the window height
 * @param height New window height in pixels
 * @details Updates the internal value in the _data structure. Note: this method only updates
 *          the stored value, it does not actually resize the window
 */
void OpenGLViewport::SetHeight(const unsigned int height) {
	_data.height = height;
}

/**
 * @brief Enables or disables vertical synchronization (VSync)
 * @param enabled true to enable VSync, false to disable it
 * @details When enabled, limits the frame rate to the monitor's refresh rate to prevent tearing.
 *          If GLFW has not been initialized, prints a warning and returns without making changes.
 *          Internally uses glfwSwapInterval(1) to enable and glfwSwapInterval(0) to disable
 */
void OpenGLViewport::SetVSync(const bool enabled) {
	if (not s_GLFWInitialized) {
		CE_CORE_WARN("Could not set VSync because GLFW is not initialized.");
		return;
	}
	_data.VSync = enabled;
	glfwSwapInterval(enabled ? 1 : 0);
}

/**
 * @brief Private method for OpenGL window initialization
 * @details Performs the following operations:
 *          1. Logs window information
 *          2. Initializes GLFW if not already done (also sets up error callback)
 *          3. Configures OpenGL context parameters (version 4.1 Core Profile with forward compatibility)
 *          4. Creates the GLFW window with the specified dimensions
 *          5. Makes the window's OpenGL context current
 *          6. Initializes GLAD to load OpenGL functions
 *          7. Associates window data with GLFW's user pointer
 *          8. Sets VSync according to configuration
 *          9. Registers all event callbacks
 *          In case of error during GLAD initialization, terminates the program with exit(EXIT_FAILURE)
 */
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

/**
 * @brief Private method for cleanup and resource release
 * @details Releases GLFW window resources by resetting the _glfwWindow smart pointer,
 *          which automatically destroys the GLFW window when there are no more references to it
 */
void OpenGLViewport::_Shutdown() {
	_glfwWindow.reset();
}

}
