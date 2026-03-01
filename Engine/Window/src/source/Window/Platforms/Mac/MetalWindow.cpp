//
// Module: Window/Platforms/Mac
// File: MetalWindow.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-18
// Updated: 2026-02-28
//

#include "Window/Platforms/Mac/MetalWindow.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "MetalBridge/Cocoa/MetalCocoaBridge.h"
#include "Tools/Log/Log.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA

#include <GLFW/glfw3native.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>

namespace CE::Window {

/**
 * @brief Static flag to track if GLFW has already been initialized
 * @details Ensures that glfwInit() is called only once during program execution
 */
static bool s_GLFWInitialized = false;

/**
 * @brief MetalWindow constructor implementation
 * @param windowProps Window configuration properties (title, width, height, VSync)
 * @details Initializes window data with the provided properties and calls _Init()
 *          to set up the Metal device and create the window
 */
MetalWindow::MetalWindow(const TypeWindow::WindowProps& windowProps): _data(windowProps) {
	_Init();
}

/**
 * @brief MetalWindow destructor implementation
 * @details Calls _Shutdown() to clean up GLFW window resources. Metal resources
 *          are automatically released by their smart pointers.
 */
MetalWindow::~MetalWindow() {
	_Shutdown();
}

/**
 * @brief Updates the window state each frame
 * @details Polls all pending GLFW events. Unlike OpenGL window, Metal handles
 *          buffer presentation separately through the Metal command buffer system,
 *          so no swap buffer call is needed here.
 */
void MetalWindow::OnUpdate() {
	glfwPollEvents();
}

/**
 * @brief Sets the event callback function
 * @param callback Function to be called when events occur
 * @details Stores the callback in _data.eventCallback, which will be invoked by
 *          all the GLFW event callbacks registered in SetWindowCallbacks()
 */
void MetalWindow::SetEventCallback(const EventCallbackFn& callback) {
	_data.eventCallback = callback;
}

/**
 * @brief Configures all GLFW window event callbacks
 * @details Sets up lambda callbacks for all GLFW events:
 *          - Window resize: Updates dimensions and generates WindowResizeEvent
 *          - Window close: Generates WindowCloseEvent
 *          - Keyboard input: Handles key press/release/repeat, generates KeyPressedEvent and KeyReleasedEvent
 *          - Character input: Generates KeyTypedEvent for text input
 *          - Mouse buttons: Generates MouseButtonPressedEvent and MouseButtonReleasedEvent
 *          - Mouse scroll: Generates MouseScrolledEvent
 *          - Cursor position: Generates MouseMovedEvent
 *          All callbacks retrieve the EventWindowData from GLFW user pointer and invoke
 *          the stored event callback. Returns early if _glfwWindow is null.
 */
void MetalWindow::SetWindowCallbacks() {
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
 * @param width New width in pixels
 * @details Updates the cached width value in _data and updates the Metal layer's
 *          drawable size to match. The Metal layer must be recreated with the new size.
 */
void MetalWindow::SetWidth(const unsigned int width) {
	_data.width = width;
	if (_metalLayer) {
		_metalLayer->setDrawableSize(CGSizeMake(_data.width, _data.height));
	}
}

/**
 * @brief Sets the window height
 * @param height New height in pixels
 * @details Updates the cached height value in _data and updates the Metal layer's
 *          drawable size to match. The Metal layer must be recreated with the new size.
 */
void MetalWindow::SetHeight(const unsigned int height) {
	_data.height = height;
	if (_metalLayer) {
		_metalLayer->setDrawableSize(CGSizeMake(_data.width, _data.height));
	}
}

/**
 * @brief Enables or disables vertical synchronization (VSync)
 * @param enabled True to enable VSync, false to disable
 * @details When enabled, synchronizes rendering with the display's refresh rate to
 *          prevent screen tearing. Uses Metal's setDisplaySyncEnabled method on the
 *          Metal layer. If GLFW is not initialized or Metal layer is null, prints a
 *          warning and returns without making changes. Logs the VSync state change.
 */
void MetalWindow::SetVSync(const bool enabled) {
	if (not s_GLFWInitialized) {
		CE_CORE_WARN("Could not set VSync because GLFW is not initialized.");
		return;
	}
	_data.VSync = enabled;
	if (_metalLayer) {
		_metalLayer->setDisplaySyncEnabled(_data.VSync);
		CE_CORE_INFO("VSync {0}", _data.VSync ? "enabled" : "disabled");
	}
}

/**
 * @brief Private initialization method for the Metal window
 * @details Performs complete window initialization in the following order:
 *          1. Calls _InitDevice() to create Metal device and command queue
 *          2. Calls _InitWindow() to create GLFW window and configure Metal layer
 *          3. Calls SetVSync() to configure vertical synchronization
 *          4. Calls SetWindowCallbacks() to register all event callbacks
 */
void MetalWindow::_Init() {
	_InitDevice();
	_InitWindow();
	SetVSync(_data.VSync);
	SetWindowCallbacks();
}

/**
 * @brief Initializes the Metal device and command queue
 * @details Creates the Metal resources needed for rendering:
 *          - Metal device: Creates the system default Metal device (GPU)
 *          - Command queue: Creates a command queue for submitting rendering commands
 *          Exits with EXIT_FAILURE if either creation fails. Both resources are
 *          managed by smart pointers (NS::TransferPtr) for automatic cleanup.
 */
void MetalWindow::_InitDevice() {
	_metalDevice = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
	if (not _metalDevice) {
		CE_CORE_ERROR("Could not create MetalDevice!");
		exit(EXIT_FAILURE);
	}
	_commandQueue = NS::TransferPtr(_metalDevice->newCommandQueue());
	if (not _commandQueue) {
		CE_CORE_ERROR("Could not create CommandQueue!");
		exit(EXIT_FAILURE);
	}
}

/**
 * @brief Initializes the GLFW window and configures the Metal layer
 * @details Performs complete window initialization:
 *          1. Initializes GLFW if not already done (with error callback)
 *          2. Sets GLFW window hint to GLFW_NO_API (no OpenGL context needed)
 *          3. Creates GLFW window with specified dimensions and title
 *          4. Associates window data with GLFW user pointer
 *          5. Retrieves the native Cocoa window from GLFW
 *          6. Gets the Cocoa window's content view
 *          7. Creates a CAMetalLayer for Metal rendering
 *          8. Configures Metal layer (device, pixel format, drawable size)
 *          9. Optimizes for triple buffering (3 drawables, no timeout)
 *          10. Sets Metal layer as backing layer for content view
 *          Exits with EXIT_FAILURE if any step fails. Uses Cocoa bridge functions
 *          to interact with Objective-C APIs.
 */
void MetalWindow::_InitWindow() {
	CE_INFO("Creating window {0}, ({1}x{2}), VSync: {3}, Graphics api: {4}", _data.title, _data.width, _data.height, _data.VSync, _data.graphicsApi);

	if (not s_GLFWInitialized) {
		if (const int success = glfwInit(); not success) {
			CE_CORE_ERROR("Could not initialize GLFW!");
			exit(EXIT_FAILURE);
		}
		glfwSetErrorCallback([]([[maybe_unused]] const int error_code, [[maybe_unused]] const char* description) {
			CE_CORE_ERROR("GLFW error: {0}\nDescription: {1}", error_code, description);
		});
		s_GLFWInitialized = true;
	}

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	_glfwWindow.reset(glfwCreateWindow(
		static_cast<int>(_data.width),
		static_cast<int>(_data.height),
		_data.title.c_str(),
		nullptr,
		nullptr
	));
	glfwSetWindowUserPointer(_glfwWindow.get(), &_data);

	// Get native Cocoa window from GLFW
	void* cocoaWindow = glfwGetCocoaWindow(_glfwWindow.get());
	if (not cocoaWindow) {
		CE_CORE_ERROR("Could not get Cocoa window from GLFW!");
		exit(EXIT_FAILURE);
	}

	_metalWindow = NS::TransferPtr(static_cast<NS::Window*>(cocoaWindow));

	// Get GLFW's content view
	void* contentView = Bridge::GetCocoaContentView(cocoaWindow);
	if (not contentView) {
		CE_CORE_ERROR("Could not get content view from GLFW window!");
		exit(EXIT_FAILURE);
	}

	// Create CAMetalLayer
	_metalLayer = NS::TransferPtr(CA::MetalLayer::layer());
	if (not _metalLayer) {
		CE_CORE_ERROR("Could not create CAMetalLayer!");
		exit(EXIT_FAILURE);
	}

	// Configure Metal layer
	_metalLayer->setDevice(_metalDevice.get());
	_metalLayer->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
	_metalLayer->setDrawableSize(CGSizeMake(_data.width, _data.height));

	// Optimize for triple buffering (reduces latency)
	_metalLayer->setMaximumDrawableCount(3);

	// Allow next drawable to be acquired while the previous frame is still rendering
	_metalLayer->setAllowsNextDrawableTimeout(false);

	// Set Metal layer as the layer for GLFW's content view
	// This doesn't replace the view, just sets its backing layer
	Bridge::SetCocoaViewLayer(contentView, _metalLayer.get());
}

/**
 * @brief Cleans up and releases window resources
 * @details Resets the GLFW window smart pointer, which automatically destroys
 *          the GLFW window. Metal resources (_metalDevice, _commandQueue, _metalLayer,
 *          _metalWindow) are automatically released by their NS::SharedPtr destructors.
 */
void MetalWindow::_Shutdown() {
	_glfwWindow.reset();
}

}
