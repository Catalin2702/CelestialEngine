//
// Created by Catalin Chirosca on 2026-02-18.
//

#include "Window/Platforms/Mac/MetalViewport.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"
#include "MetalBridge/Cocoa/MetalCocoaBridge.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA

#include <GLFW/glfw3native.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/CAMetalLayer.hpp>


namespace CeEvents = CE::Events;

namespace CE::Window {

static bool s_GLFWInitialized = false;

MetalViewport::MetalViewport(const CeTypeWindow::WindowProps &windowProps): _data(windowProps.title, windowProps.width, windowProps.height, windowProps.VSync) {
	_Init();
}

MetalViewport::~MetalViewport() {
	_Shutdown();
}

void MetalViewport::OnUpdate() {
	glfwPollEvents();
}

void MetalViewport::SetEventCallback(const EventCallbackFn& callback) {
	_data.eventCallback = callback;
}

void MetalViewport::SetWindowCallbacks() {
	if (not _glfwWindow)
		return;

	glfwSetWindowSizeCallback(_glfwWindow.get(), [](GLFWwindow* window, const int width, const int height) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			data->width = static_cast<unsigned int>(width);
			data->height = static_cast<unsigned int>(height);
			CeEvents::WindowResizeEvent event{data->width, data->height};
			data->eventCallback(event);
		}
	});

	glfwSetWindowCloseCallback(_glfwWindow.get(), [](GLFWwindow* window) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			CeEvents::WindowCloseEvent event;
			data->eventCallback(event);
		}
	});

	glfwSetKeyCallback(_glfwWindow.get(), [](GLFWwindow* window, const int key, const int, const int action, const int) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
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
				default:;
			}
		}
	});

	glfwSetMouseButtonCallback(_glfwWindow.get(), [](GLFWwindow* window, const int button, const int action, const int) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
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
				default:;
			}
		}
	});

	glfwSetScrollCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xOffset, const double yOffset) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			CeEvents::MouseScrolledEvent mouseScrolledEvent{static_cast<float>(xOffset), static_cast<float>(yOffset)};
			data->eventCallback(mouseScrolledEvent);
		}
	});

	glfwSetCursorPosCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xPos, const double yPos) {
		if (const auto data = static_cast<EventWindowData*>(glfwGetWindowUserPointer(window))) {
			CeEvents::MouseMovedEvent mouseMovedEvent{static_cast<float>(xPos), static_cast<float>(yPos)};
			data->eventCallback(mouseMovedEvent);
		}
	});
}

void MetalViewport::SetWidth(const unsigned int width) {
	_data.width = width;
	if (_metalLayer) {
		_metalLayer->setDrawableSize(CGSizeMake(_data.width, _data.height));
	}
}

void MetalViewport::SetHeight(const unsigned int height) {
	_data.height = height;
	if (_metalLayer) {
		_metalLayer->setDrawableSize(CGSizeMake(_data.width, _data.height));
	}
}

void MetalViewport::SetVSync(const bool enabled) {
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

void MetalViewport::_Init() {
	_InitDevice();
	_InitWindow();
	SetVSync(_data.VSync);
	SetWindowCallbacks();
}

void MetalViewport::_InitDevice() {
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

void MetalViewport::_InitWindow() {
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

void MetalViewport::_Shutdown() {
	_glfwWindow.reset();
}

}
