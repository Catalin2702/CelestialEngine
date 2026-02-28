//
// Module: Core
// File: Application.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-15
// Updated: 2026-02-28
//

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Layers/I_Layer.hpp"
#include "Tools/Log/Log.hpp"
#include "Window/I_Viewport.hpp"

#ifdef CE_PLATFORM_MACOS
#include "Window/Platforms/Mac/MetalViewport.hpp"
#include "Window/Platforms/Universal/OpenGLViewport.hpp"
#else
#include "Window/Platforms/Universal/OpenGLViewport.hpp"
#endif

#ifdef CE_PLATFORM_MACOS
using Viewport = CE::Window::MetalViewport;
#else
using Viewport = CE::Window::OpenGLViewport;
#endif


namespace CE::Core {

Application* Application::_instance = nullptr;

Application::Application() {
	assert(_instance == nullptr && "Application already exists!");
	_Init({"CelestialEngine", 1280, 720, true});
}

Application::Application(const CeTypeWindow::WindowProps& windowProps) {
	_Init(windowProps);
}

Application::Application(const std::string &title, const unsigned int width, const unsigned int height, const bool VSync) {
	_Init({title, width, height, VSync});
}

Application::~Application() = default;

void Application::Run() {
	while (_running) {
		for (const auto layer: _layerStack) {
			layer->OnUpdate();
		}
		_viewport->OnUpdate();
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
	CE_CORE_INFO("Window closed");
	return true;
}

void Application::PushLayer(Layers::I_Layer *layer) {
	_layerStack.PushLayer(layer);
}

void Application::PushOverlay(Layers::I_Layer *overlay) {
	_layerStack.PushOverlay(overlay);
}

void Application::_Init(const CeTypeWindow::WindowProps& windowProps) {
	_instance = this;
	_viewport = std::unique_ptr<Window::I_Viewport>(
		Window::I_Viewport::CreateWindow<Viewport>(windowProps)
	);
	if (not _viewport) {
		CE_CORE_ERROR("Can't initialize the window");
		exit(EXIT_FAILURE);
	}
	_viewport->SetEventCallback(BIND_FN_ONE_PARAM(Application::OnEvent));
	_running = true;
}

}
