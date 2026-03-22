//
// Module: CelestialEngine/Engine/Modules/Core
// File: Application.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
//

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Input/I_Input.hpp"
#include "Layers/I_Layer.hpp"
#include "Layers/ImGui/Platforms/Common/ImGuiOpenGlLayer.hpp"
#include "Render/Context/Platforms/Common/OpenGlContext.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Window/I_Window.hpp"
#include "Window/Platforms/Common/GlfwWindow.hpp"


#ifdef CE_PLATFORM_MACOS
#include "Layers/ImGui/Platforms/Mac/ImGuiMetalLayer.hpp"

#include "Render/Context/Platforms/Mac/MetalContext.hpp"

#include "Window/Platforms/Mac/CocoaWindow.hpp"
#endif

#include <cassert>
#include <memory>
#include <stdexcept>


namespace CE::Core {

Application* Application::_instance = nullptr;

Application::Application() {
	assert(_instance == nullptr && "Application already exists!");
	_Init();
}

Application::~Application() {
	// Detach all layers (they may use GLFW in their cleanup)
	_layerStack.Clear();

	// Shutdown input system (deletes the singleton instance)
	Input::ShutdownInput();

	// Destroy the window (calls glfwDestroyWindow)
	_window.reset();

	// Reset the singleton instance pointer
	_instance = nullptr;
}

void Application::Update() {
	for (const auto layer: _layerStack)
		layer->OnUpdate();

	_renderLayer->Begin();
	for (const auto layer: _layerStack)
		if (const auto renderLayer = dynamic_cast<Layers::I_RenderLayer*>(layer))
			renderLayer->OnRender();
	_renderLayer->End();

	_window->OnUpdate();
	_context->SwapBuffers();
}

void Application::Run() {
	while (_running) {
		Update();
	}
}

void Application::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher eventDispatcher(event);
	eventDispatcher.Dispatch<Events::WindowCloseEvent>(BIND_FN_ONE_PARAM(Application::OnWindowClose));

	for (auto it = _layerStack.end(); it != _layerStack.begin(); ) {
		(*--it)->OnEvent(event);
		if (event.IsHandled())
			break;
	}
}

bool Application::OnWindowClose(const Events::WindowCloseEvent&) {
	_running = false;
	return true;
}

void Application::PushLayer(Layers::I_Layer *layer) {
	_layerStack.PushLayer(layer);
}

void Application::PushOverlay(Layers::I_Layer *overlay) {
	_layerStack.PushOverlay(overlay);
}

void Application::PopLayer(Layers::I_Layer* layer) {
	if (layer == _renderLayer)
		_renderLayer = nullptr;

	_layerStack.PopLayer(layer);
}

void Application::PopOverlay(Layers::I_Layer* overlay) {
	if (overlay == _renderLayer)
		_renderLayer = nullptr;

	_layerStack.PopOverlay(overlay);
}

Application& Application::Get() {
	assert(_instance && "Application::Get: No application instance exists!");
	return *_instance;
}

void Application::InitWindow(const TypeWindow::WindowProps& windowProps) {
	if (not TypeWindow::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		CE_CORE_ERROR("Application::_Init: Incompatible graphics API and window API specified in window properties. Graphics API: {0}, Window API: {1}", windowProps.graphicsApi, windowProps.windowApi);
		throw std::runtime_error("Incompatible graphics API and window API specified in window properties");
	}

	std::unique_ptr<Window::I_Window> window;

	switch (windowProps.windowApi) {
		case TypeWindow::WindowApi::GLFW: {
			window = std::unique_ptr<Window::I_Window>(
				Window::I_Window::NewWindow<Window::GlfwWindow>(windowProps)
			);
			break;
		}
#ifdef CE_PLATFORM_MACOS
		case TypeWindow::WindowApi::Cocoa: {
			window = std::unique_ptr<Window::I_Window>(
				Window::I_Window::NewWindow<Window::CocoaWindow>(windowProps)
			);
			break;
		}
#endif
		case TypeWindow::WindowApi::None:
		default: {
			CE_CORE_ERROR("Application::_Init: Unsupported window API specified in window properties. Window API: {0}", windowProps.windowApi);
			throw std::runtime_error("Unsupported window API specified in window properties");
		}
	}

	assert(window && "Application::_Init: Failed to create window instance");

	window->SetEventCallback(BIND_FN_ONE_PARAM(Application::OnEvent));

	_window = std::move(window);

	Input::InitInput(windowProps.windowApi);
}

void Application::InitRenderer(const TypeWindow::WindowProps& windowProps) {
	assert(_window && "Application::InitRenderer: Window must be initialized before initializing renderer");

	std::unique_ptr<Render::Context::I_Context> context;

	switch (windowProps.graphicsApi) {
		case Types::Render::GraphicsApi::OpenGL: {
			if (_window->GetWindowApi() != TypeWindow::WindowApi::GLFW) {
				CE_CORE_ERROR("Application::InitRenderer: OpenGL graphics API is only supported with GLFW window API. Current window API: {0}", _window->GetWindowApi());
				throw std::runtime_error("OpenGL graphics API is only supported with GLFW window API");
			}
			context = std::make_unique<Render::Context::OpenGlContext>(static_cast<GLFWwindow*>(_window->GetNativeWindow()));
			break;
		}
#ifdef CE_PLATFORM_MACOS
		case Types::Render::GraphicsApi::Metal: {
			if (_window->GetWindowApi() != TypeWindow::WindowApi::Cocoa) {
				CE_CORE_ERROR("Application::InitRenderer: Metal graphics API on macOS is only supported with Cocoa window API. Current window API: {0}", _window->GetWindowApi());
				throw std::runtime_error("Metal graphics API on macOS is only supported with Cocoa window API");
			}
			const Render::Context::MetalContextProps props {
				.window = dynamic_cast<Window::CocoaWindow*>(_window.get()),
				.pixelFormat = MTL::PixelFormat::PixelFormatBGRA8Unorm
			};
			context = std::make_unique<Render::Context::MetalContext>(props);
			break;
		}
#endif
		default: {
			CE_CORE_ERROR("Application::InitRenderer: Unsupported graphics API specified in window properties for renderer initialization. Graphics API: {0}", windowProps.graphicsApi);
			throw std::runtime_error("Unsupported graphics API specified in window properties for renderer initialization");
		}
	}

	assert(context && "Application::InitRenderer: Failed to create graphics context instance");

	_context = std::move(context);

	_context->Init();
}

void Application::InitImGuiLayer(const TypeWindow::WindowProps& windowProps) {
	assert(_window && "Application::InitImGuiLayer: Window must be initialized before initializing ImGui layer");

	std::unique_ptr<Layers::I_ImGuiLayer> overlay;

	switch (windowProps.graphicsApi) {
		case Types::Render::GraphicsApi::OpenGL: {
			overlay = std::make_unique<Layers::ImGuiOpenGlLayer>();
			break;
		}
#ifdef CE_PLATFORM_MACOS
		case Types::Render::GraphicsApi::Metal: {
			overlay = std::make_unique<Layers::ImGuiMetalLayer>();
			break;
		}
#endif
		default: {
			CE_CORE_ERROR("Application::InitImGuiLayer: Unsupported graphics API specified in window properties for ImGui layer. Graphics API: {0}", windowProps.graphicsApi);
			throw std::runtime_error("Unsupported graphics API specified in window properties for ImGui layer");
		}
	}

	assert(overlay && "Application::InitImGuiLayer: Failed to create ImGui layer instance");

	_renderLayer = overlay.release();
	PushOverlay(_renderLayer);
}

void Application::InitAll(const TypeWindow::WindowProps& windowProps) {
	InitWindow(windowProps);
	InitRenderer(windowProps);
	InitImGuiLayer(windowProps);

	_SetWindowCallbacks();
	_window->GetReady();
}

void Application::_Init() {
	_instance = this;
	_running = true;
}

void Application::_SetWindowCallbacks() const {
	assert(_window && "Application::_SetWindowCallbacks: Window must be initialized before setting window callbacks");

	if (_context) {
		_window->SetContentScaleCallback(BIND_FN_ONE_PARAM_ON(_context.get(), &Render::Context::I_Context::HandleContentSizeChange));
		_window->SetVSyncCallback(BIND_FN_ONE_PARAM_ON(_context.get(), &Render::Context::I_Context::HandleVSyncChange));
	}
}

}
