//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Common/Glfw
// File: GlfwWindow.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"
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

	// glfwSetKeyCallback(_glfwWindow.get(), [](GLFWwindow* window, const int key, const int, const int action, const int) {
	// 	if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
	// 		switch (action) {
	// 			case GLFW_PRESS: {
	// 				Events::KeyPressedEvent keyPressedEvent{KeyCode::KeyboardKeyCodeFromGlfw(key), 0};
	// 				windowCallbacks->EventCallback(keyPressedEvent);
	// 				break;
	// 			}
	//
	// 			case GLFW_RELEASE: {
	// 				Events::KeyReleasedEvent keyReleasedEvent{KeyCode::KeyboardKeyCodeFromGlfw(key)};
	// 				windowCallbacks->EventCallback(keyReleasedEvent);
	// 				break;
	// 			}
	//
	// 			case GLFW_REPEAT: {
	// 				Events::KeyPressedEvent keyPressedEvent{KeyCode::KeyboardKeyCodeFromGlfw(key), 1};
	// 				windowCallbacks->EventCallback(keyPressedEvent);
	// 				break;
	// 			}
	// 			default:;
	// 		}
	// 	}
	// });
	//
	// glfwSetCharCallback(_glfwWindow.get(), [](GLFWwindow* window, const unsigned int codepoint) {
	// 	if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
	// 		Events::KeyTypedEvent keyTypedEvent{codepoint};
	// 		windowCallbacks->EventCallback(keyTypedEvent);
	// 	}
	// });
	//
	// glfwSetMouseButtonCallback(_glfwWindow.get(), [](GLFWwindow* window, const int button, const int action, const int) {
	// 	if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
	// 		switch (action) {
	// 			case GLFW_PRESS: {
	// 				Events::MouseButtonPressedEvent mouseButtonPressedEvent{KeyCode::MouseButtonKeyCodeFromGlfw(button)};
	// 				windowCallbacks->EventCallback(mouseButtonPressedEvent);
	// 				break;
	// 			}
	// 			case GLFW_RELEASE: {
	// 				Events::MouseButtonReleasedEvent mouseButtonReleasedEvent{KeyCode::MouseButtonKeyCodeFromGlfw(button)};
	// 				windowCallbacks->EventCallback(mouseButtonReleasedEvent);
	// 				break;
	// 			}
	// 			default:;
	// 		}
	// 	}
	// });
	//
	// glfwSetScrollCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xOffset, const double yOffset) {
	// 	if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
	// 		Events::MouseScrolledEvent mouseScrolledEvent{static_cast<float>(xOffset), static_cast<float>(yOffset)};
	// 		windowCallbacks->EventCallback(mouseScrolledEvent);
	// 	}
	// });
	//
	// glfwSetCursorPosCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xPos, const double yPos) {
	// 	if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
	// 		Events::MouseMovedEvent mouseMovedEvent{static_cast<float>(xPos), static_cast<float>(yPos)};
	// 		windowCallbacks->EventCallback(mouseMovedEvent);
	// 	}
	// });
}

void GlfwWindow::_SetWindowEventCallbacks() {
	if (not _glfwWindow)
		return;

	// glfwSetWindowSizeCallback(_glfwWindow.get(), [](GLFWwindow* window, const int width, const int height) {
	// 	if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
	// 		Events::WindowResizeEvent event{static_cast<unsigned int>(width), static_cast<unsigned int>(height)};
	// 		windowCallbacks->EventCallback(event);
	// 		windowCallbacks->_internalCallbacks.ResizeEventCallback(event);
	// 	}
	// });
	//
	// glfwSetWindowCloseCallback(_glfwWindow.get(), [](GLFWwindow* window) {
	// 	if (const auto windowCallbacks = static_cast<WindowCallbacks*>(glfwGetWindowUserPointer(window)); windowCallbacks and windowCallbacks->EventCallback) {
	// 		Events::WindowCloseEvent event;
	// 		windowCallbacks->EventCallback(event);
	// 	}
	// });
}

void GlfwWindow::_SetInternalCallbacks() {
	// _callbacks._internalCallbacks.ResizeEventCallback = [this](const Events::WindowResizeEvent& event) {
	// 	SetSize(event.GetWidth(), event.GetHeight());
	// };
}

void GlfwWindow::Init() {
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

	const auto& windowProps = Utility::Config::Config::StGetWindowProps();

	_glfwWindow.reset(glfwCreateWindow(
		static_cast<int>(windowProps.width),
		static_cast<int>(windowProps.height),
		windowProps.title.c_str(),
		nullptr,
		nullptr
	));

	if (not _glfwWindow) {
		CE_CORE_ERROR("Failed to create GLFW window!");
		throw std::runtime_error("Failed to create GLFW window!");
	}

	// glfwSetWindowUserPointer(_glfwWindow.get(), &_callbacks);
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
