//
// Created by Catalin Chirosca on 2026-02-15.
//

#include "Core/Application.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/Event.hpp"
#include "Layers/Layer.hpp"
#include "Tools/Log/Log.hpp"
#include "Window/InterfaceWindow.hpp"
#include "Window/Platforms/Universal/OpenGLViewport.hpp"

#ifdef CE_PLATFORM_MACOS
#include "Window/Platforms/Mac/MetalViewport.hpp"
#endif

#ifdef CE_PLATFORM_MACOS
using Viewport = CE::Window::MetalViewport;
#else
using Viewport = CE::Window::OpenGLViewport;
#endif


namespace CE::Core {

Application::Application() {
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
		_window->OnUpdate();
	}
}

void Application::OnEvent(Events::Event& event) {
	Events::EventDispatcher eventDispatcher(event);
	eventDispatcher.Dispatch<Events::WindowCloseEvent>(BIND_EVENT_FN_ONE_PARAM(Application::OnWindowClose));

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

void Application::PushLayer(Layers::Layer *layer) {
	_layerStack.PushLayer(layer);
}

void Application::PushOverlay(Layers::Layer *overlay) {
	_layerStack.PushOverlay(overlay);
}

void Application::_Init(const CeTypeWindow::WindowProps& windowProps) {
	_window = std::unique_ptr<Window::InterfaceViewport>(
		Window::InterfaceViewport::CreateWindow<Viewport>(windowProps)
	);
	if (not _window) {
		CE_CORE_ERROR("Can't initialize the window");
		exit(EXIT_FAILURE);
	}
	_window->SetEventCallback(BIND_EVENT_FN_ONE_PARAM(Application::OnEvent));
	_running = true;
}

}
