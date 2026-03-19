//
// Module: CelestialEngine/Engine/Modules/Core
// File: Application.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
//

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Input/I_Input.hpp"
#include "Layers/I_Layer.hpp"
#include "Layers/ImGui/Platforms/Common/ImGuiOpenGlGlfwLayer.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Window/I_Window.hpp"
#include "Window/Platforms/Common/GlfwWindow.hpp"


#ifdef CE_PLATFORM_MACOS
#include "Layers/ImGui/Platforms/Mac/ImGuiMetalCocoaLayer.hpp"
#include "Layers/ImGui/Platforms/Mac/ImGuiMetalGlfwLayer.hpp"

#include "Window/Platforms/Mac/MetalCocoaWindow.hpp"
#endif

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

void Application::InitWindow(const TypeWindow::WindowProps& windowProps) {
	if (not TypeWindow::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		CE_CORE_ERROR("Application::_Init: Incompatible graphics API and window API specified in window properties. Graphics API: {0}, Window API: {1}", windowProps.graphicsApi, windowProps.windowApi);
		throw std::runtime_error("Incompatible graphics API and window API specified in window properties");
	}

	std::unique_ptr<Window::I_Window> window;

	switch (windowProps.windowApi) {
		case TypeWindow::WindowApi::GLFW: {
			window = std::unique_ptr<Window::I_Window>(
				Window::I_Window::CreateWindow<Window::GlfwWindow>(windowProps)
			);
			break;
		}
#ifdef CE_PLATFORM_MACOS
		case TypeWindow::WindowApi::Cocoa: {
			window = std::unique_ptr<Window::I_Window>(
				Window::I_Window::CreateWindow<Window::MetalCocoaWindow>(windowProps)
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

	if (not window) {
		CE_CORE_ERROR("Application::_Init: Can't initialize the window");
		throw std::runtime_error("Can't initialize the window");
	}

	window->SetEventCallback(BIND_FN_ONE_PARAM(Application::OnEvent));

	_window = std::move(window);

	Input::InitInput(windowProps.windowApi);
}

void Application::InitImGuiLayer(const TypeWindow::WindowProps& windowProps) {
	if (not _window) {
		CE_CORE_ERROR("Application::InitImGuiLayer: Window must be initialized before initializing ImGui layer");
		throw std::runtime_error("Window must be initialized before initializing ImGui layer");
	}

	std::unique_ptr<Layers::I_ImGuiLayer> overlay;

	switch (windowProps.graphicsApi) {
		case TypeWindow::GraphicsApi::OpenGL: {
			overlay = std::make_unique<Layers::ImGuiOpenGlGlfwLayer>();
			break;
		}
#ifdef CE_PLATFORM_MACOS
		case TypeWindow::GraphicsApi::Metal: {
			switch (windowProps.windowApi) {
				case TypeWindow::WindowApi::GLFW: {
					overlay = std::make_unique<Layers::ImGuiMetalGlfwLayer>();
					break;
				}
				case TypeWindow::WindowApi::Cocoa: {
					overlay = std::make_unique<Layers::ImGuiMetalCocoaLayer>();
					break;
				}
				default: {
					CE_CORE_ERROR("Application::InitImGuiLayer: Unsupported window API specified in window properties for Metal graphics API on macOS. Window API: {0}", windowProps.windowApi);
					throw std::runtime_error("Unsupported window API specified in window properties for Metal graphics API on macOS");
				}
			}
			break;
		}
#endif
		default: {
			CE_CORE_ERROR("Application::InitImGuiLayer: Unsupported graphics API specified in window properties for ImGui layer. Graphics API: {0}", windowProps.graphicsApi);
			throw std::runtime_error("Unsupported graphics API specified in window properties for ImGui layer");
		}
	}

	if (not overlay) {
		CE_CORE_ERROR("Application::InitImGuiLayer: Can't initialize the ImGui layer");
		throw std::runtime_error("Can't initialize the ImGui layer");
	}

	_renderLayer = overlay.release();
	PushOverlay(_renderLayer);
}

void Application::_Init() {
	_instance = this;
	_running = true;
}

}
