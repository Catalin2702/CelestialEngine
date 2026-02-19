//
// Created by Catalin Chirosca on 2026-02-15.
//

#include "Core/Application.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/Event.hpp"
#include "Layers/Layer.hpp"
#include "Tools/Log/Log.hpp"
#include "Window/InterfaceWindow.hpp"


namespace CeEvents = CE::Events;
namespace CeWindow = CE::Window;

namespace CE::Core {

Application::Application() {
	_window = std::unique_ptr<CeWindow::InterfaceViewport>(CeWindow::InterfaceViewport::CreateWindow(CeWindow::WindowProps("CelestialEngine", 1280, 720, true)));
	_window->SetEventCallback(BIND_EVENT_FN_ONE_PARAM(Application::OnEvent));
	_running = true;
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
	CeEvents::EventDispatcher eventDispatcher(event);
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
	_layerStack.PopOverlay(overlay);
}

}
