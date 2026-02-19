//
// Created by Catalin Chirosca on 2026-02-18.
//

#include "Window/Platforms/Mac/MetalViewport.hpp"
#include "Window/Platforms/Mac/MetalViewportHelper.h"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Tools/Log/Log.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_COCOA

#include <GLFW/glfw3native.h>

#include <AppKit/AppKit.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>
#include <QuartzCore/CAMetalLayer.h>
#include <QuartzCore/CAMetalLayer.hpp>


namespace CeEvents = CE::Events;

namespace CE::Window {

static bool s_GLFWInitialized = false;

MetalViewport::MetalViewport(const WindowProps &windowProps) {
	_Init(windowProps);
}

MetalViewport::~MetalViewport() {
	_Shutdown();
}

void MetalViewport::OnUpdate() {
	glfwPollEvents();

	if (not _view or not _commandQueue)
		return;

	const auto pool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

	// Get the current drawable from MTKView
	const auto drawable = _view->currentDrawable();
	if (not drawable)
		return;

	// Get render pass descriptor from MTK::View
	const auto renderPassDescriptor = _view->currentRenderPassDescriptor();
	if (not renderPassDescriptor)
		return;

	const auto commandBuffer = _commandQueue->commandBuffer();

	// Create render command encoder with render pass descriptor
	const auto encoder = commandBuffer->renderCommandEncoder(renderPassDescriptor);

	encoder->endEncoding();

	commandBuffer->presentDrawable(drawable);

	commandBuffer->commit();
}

void MetalViewport::SetEventCallback(const EventCallbackFn &callback) {
	_data.eventCallback = callback;
}

void MetalViewport::SetWindowCallbacks() {
	if (not _glfwWindow)
		return;

	glfwSetWindowSizeCallback(_glfwWindow.get(), [](GLFWwindow* window, const int width, const int height) {
		if (const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window))) {
			data->width = static_cast<unsigned int>(width);
			data->height = static_cast<unsigned int>(height);
			CeEvents::WindowResizeEvent event{data->width, data->height};
			data->eventCallback(event);
		}
	});

	glfwSetWindowCloseCallback(_glfwWindow.get(), [](GLFWwindow* window) {
		if (const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window))) {
			CeEvents::WindowCloseEvent event;
			data->eventCallback(event);
		}
	});

	glfwSetKeyCallback(_glfwWindow.get(), [](GLFWwindow* window, const int key, const int, const int action, const int) {
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

	glfwSetMouseButtonCallback(_glfwWindow.get(), [](GLFWwindow* window, const int button, const int action, const int) {
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

	glfwSetScrollCallback(_glfwWindow.get(), [](GLFWwindow* window, double xOffset, double yOffset) {
		if (const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window))) {
			CeEvents::MouseScrolledEvent mouseScrolledEvent{static_cast<float>(xOffset), static_cast<float>(yOffset)};
			data->eventCallback(mouseScrolledEvent);
		}
	});

	glfwSetCursorPosCallback(_glfwWindow.get(), [](GLFWwindow* window, const double xPos, const double yPos) {
		if (const auto data = static_cast<WindowData*>(glfwGetWindowUserPointer(window))) {
			CeEvents::MouseMovedEvent mouseMovedEvent{static_cast<float>(xPos), static_cast<float>(yPos)};
			data->eventCallback(mouseMovedEvent);
		}
	});
}

void MetalViewport::SetWidth(const unsigned int width) {
	_data.width = width;
}

void MetalViewport::SetHeight(const unsigned int height) {
	_data.height = height;
}

void MetalViewport::SetVSync(const bool enabled) {
	if (not s_GLFWInitialized) {
		CE_CORE_WARN("Could not set VSync because GLFW is not initialized.");
		return;
	}
	_data.VSync = enabled;
	if (_data.VSync and _view and _metalWindow) {
		// Ottieni il refresh rate del monitor
		const int refreshRate = GetDisplayRefreshRate(_metalWindow.get());
		_view->setPreferredFramesPerSecond(refreshRate);
		CE_CORE_INFO("VSync enabled with {0} FPS (monitor refresh rate)", refreshRate);
	}
}

void MetalViewport::_Init(const WindowProps &windowProps) {
	_data.title = windowProps.title;
	_data.width = windowProps.width;
	_data.height = windowProps.height;
	_data.VSync = windowProps.VSync;

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

	void* contentView = GetCocoaContentView(cocoaWindow);
	if (not contentView) {
		CE_CORE_ERROR("Could not get content view from GLFW window!");
		exit(EXIT_FAILURE);
	}

	_metalLayer = NS::TransferPtr(CA::MetalLayer::layer());
	if (not _metalLayer) {
		CE_CORE_ERROR("Could not create MetalLayer!");
		exit(EXIT_FAILURE);
	}

	_metalLayer->setDevice(_metalDevice.get());
	_metalLayer->setPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
	_metalLayer->setDrawableSize(CGSizeMake(_data.width, _data.height));

	// Set Metal layer as content layer for GLFW
	SetCocoaViewLayer(contentView, _metalLayer.get());


	const CGRect frame = CGRectMake(0, 0, _data.width, _data.height);
	_view = NS::TransferPtr(MTK::View::alloc()->init(frame, _metalDevice.get()));
	if (not _view) {
		CE_CORE_ERROR("Could not create MTKView!");
		exit(EXIT_FAILURE);
	}

	_view->setColorPixelFormat(MTL::PixelFormat::PixelFormatBGRA8Unorm_sRGB);
	_view->setClearColor(MTL::ClearColor::Make(1., 0., .6, 1.));

	// Set MTK::View as window's content view
	SetCocoaWindowContentView(cocoaWindow, _view.get());
}

void MetalViewport::_Shutdown() {
	_glfwWindow.reset();
}

InterfaceViewport* InterfaceViewport::CreateWindow(const WindowProps &windowProps) {
	return new MetalViewport(windowProps);
}

}
