//
// Module: CelestialEngine/Engine/Modules/Core
// File: Application.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-03-16
//

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Input/I_Input.hpp"
#include "Layers/I_Layer.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Window/I_Window.hpp"

#ifdef CE_PLATFORM_MACOS
#include "Layers/ImGui/Platforms/Mac/ImGuiMetalLayer.hpp"
#include "Window/Platforms/Mac/MetalGlfwWindow.hpp"

#include "Layers/ImGui/Platforms/Universal/ImGuiOpenGlLayer.hpp"
#include "Window/Platforms/Common/CommonGlfwWindow.hpp"
#else
#include "Layers/ImGui/Platforms/Universal/ImGuiOpenGlLayer.hpp"
#include "Window/Platforms/Common/CommonGlfwWindow.hpp"
#endif

#include <memory>
#include <stdexcept>


namespace CE::Core {

Application* Application::_instance = nullptr;

Application::Application() {
	assert(_instance == nullptr && "Application already exists!");
	_Init({"CelestialEngine", 1280, 720, true, TypeWindow::GraphicsApi::OpenGL, TypeWindow::WindowApi::GLFW});
}

Application::Application(const TypeWindow::WindowProps& windowProps) {
	assert(_instance == nullptr && "Application already exists!");
	_Init(windowProps);
}

Application::Application(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const TypeWindow::GraphicsApi graphicsApi, const TypeWindow::WindowApi windowApi) {
	assert(_instance == nullptr && "Application already exists!");
	_Init({title, width, height, VSync, graphicsApi, windowApi});
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
	CE_CORE_INFO("Application::OnWindowClose: Window closed");
	return true;
}

void Application::PushLayer(Layers::I_Layer *layer) {
	_layerStack.PushLayer(layer);
}

void Application::PushOverlay(Layers::I_Layer *overlay) {
	_layerStack.PushOverlay(overlay);
}

void Application::PopLayer(Layers::I_Layer* layer) {
	_layerStack.PopLayer(layer);
}

void Application::PopOverlay(Layers::I_Layer* overlay) {
	_layerStack.PopOverlay(overlay);
}

void Application::_Init(const TypeWindow::WindowProps& windowProps) {
	if (not TypeWindow::IsGraphicsApiCompatibleWithWindowApi(windowProps.graphicsApi, windowProps.windowApi)) {
		CE_CORE_ERROR("Application::_Init: Incompatible graphics API and window API specified in window properties. Graphics API: {0}, Window API: {1}", windowProps.graphicsApi, windowProps.windowApi);
		throw std::runtime_error("Incompatible graphics API and window API specified in window properties");
	}

	_instance = this;
	switch (windowProps.windowApi) {
		case TypeWindow::WindowApi::GLFW:
			_window = std::unique_ptr<Window::I_Window>(
				Window::I_Window::CreateWindow<Window::CommonGlfwWindow>(windowProps)
			);
			break;
#ifdef CE_PLATFORM_MACOS
		case TypeWindow::WindowApi::Cocoa:
			_window = std::unique_ptr<Window::I_Window>(
				Window::I_Window::CreateWindow<Window::MetalGlfwWindow>(windowProps)
			);
			break;
#endif
		default:
			CE_CORE_ERROR("Application::_Init: Unsupported window API specified in window properties. Window API: {0}", windowProps.windowApi);
			throw std::runtime_error("Unsupported window API specified in window properties");
	}

	if (not _window) {
		CE_CORE_ERROR("Application::_Init: Can't initialize the window");
		throw std::runtime_error("Can't initialize the window");
	}

	_window->SetEventCallback(BIND_FN_ONE_PARAM(Application::OnEvent));

	std::unique_ptr<Layers::I_ImGuiLayer> overlay;
	switch (windowProps.graphicsApi) {
		case TypeWindow::GraphicsApi::OpenGL: {
			overlay = std::make_unique<Layers::ImGuiOpenGlLayer>();
			break;
		}
		case TypeWindow::GraphicsApi::Metal: {
			overlay = std::make_unique<Layers::ImGuiMetalLayer>();
			break;
		}
		default: {
			CE_CORE_ERROR("Unsupported graphics API specified in window properties for ImGui layer. Graphics API: {0}", windowProps.graphicsApi);
			throw std::runtime_error("Unsupported graphics API specified in window properties for ImGui layer");
		}
	}
	if (not overlay) {
		CE_CORE_ERROR("Application::_Init: Can't initialize the ImGui layer");
		throw std::runtime_error("Can't initialize the ImGui layer");
	}
	_renderLayer = overlay.release();
	PushOverlay(_renderLayer);

	_running = true;
	Input::InitInput();
}

}
