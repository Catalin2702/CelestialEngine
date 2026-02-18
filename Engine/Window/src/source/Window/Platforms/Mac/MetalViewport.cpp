//
// Created by Catalin Chirosca on 2026-02-18.
//

#include "Window/Platforms/Mac/MetalViewport.hpp"
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

	auto color = MTL::ClearColor::Make(0, 0, 0, 1);

	const auto autoReleasePool = NS::TransferPtr(NS::AutoreleasePool::alloc()->init());

	color.red = color.red > 1.0 ? 0.0 : color.red + 0.01;

	auto surface = _metalLayer->nextDrawable();

	auto pass = MTL::RenderPassDescriptor::renderPassDescriptor();

	auto passColorAttachment0 = pass->colorAttachments()->object(0);
	passColorAttachment0->setClearColor(color);
	passColorAttachment0->setLoadAction(MTL::LoadActionClear);
	passColorAttachment0->setStoreAction(MTL::StoreActionStore);
	passColorAttachment0->setTexture(surface->texture());

	auto commandBuffer = _commandQueue->commandBuffer();
	auto encoder = commandBuffer->renderCommandEncoder(pass);
	encoder->endEncoding();
	commandBuffer->presentDrawable(surface);
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

	const CGRect frame = {{100.0, 100.0}, {static_cast<double>(_data.width), static_cast<double>(_data.height)}};

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	_glfwWindow.reset(glfwCreateWindow(
		static_cast<int>(_data.width),
		static_cast<int>(_data.height),
		_data.title.c_str(),
		nullptr,
		nullptr
	));
	glfwMakeContextCurrent(_glfwWindow.get());
	glfwSetWindowUserPointer(_glfwWindow.get(), &_data);

	_metalWindow = NS::TransferPtr(reinterpret_cast<NS::Window*>(glfwGetCocoaWindow(_glfwWindow.get())));
	if (not _metalWindow) {
		CE_CORE_ERROR("Could not create MetalWindow!");
		exit(EXIT_FAILURE);
	}

	_metalLayer = NS::TransferPtr(CA::MetalLayer::layer());
	if (not _metalLayer) {
		CE_CORE_ERROR("Could not create MetalLayer!");
		exit(EXIT_FAILURE);
	}

	_metalLayer->setDevice(_metalDevice.get());

	_view = NS::TransferPtr(MTK::View::alloc()->init(frame, _metalDevice.get()));
	if (not _view) {
		CE_CORE_ERROR("Could not create View!");
		exit(EXIT_FAILURE);
	}
	_view->setLayer(_metalLayer.get());
}

void MetalViewport::_Shutdown() {
	_glfwWindow.reset();
}

Window* Window::CreateWindow(const WindowProps &windowProps) {
	return new MetalViewport(windowProps);
}

}
