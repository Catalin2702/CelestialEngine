//
// Created by Catalin Chirosca on 2026-02-15.
//

#include "Core/Application.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/Event.hpp"
#include "Tools/Log/Log.hpp"
#include "Window/InterfaceWindow.hpp"

#include <GL/glew.h>


namespace CeEvents = CE::Events;
namespace CeWindow = CE::Window;

namespace CE::Core {

Application::Application() {
	_window = std::unique_ptr<CeWindow::Window>(CeWindow::Window::CreateWindow(CeWindow::WindowProps("CelestialEngine", 1280, 720, true)));
	_window->SetEventCallback(BIND_EVENT_FN_ONE_PARAM(Application::OnEvent));
	_running = true;
}

Application::~Application() {}

void Application::Run() {
	while (_running) {
		// glClearColor(1, 0, 1, 1);
		// glClear(GL_COLOR_BUFFER_BIT);
		_window->OnUpdate();
	}
}

void Application::OnEvent(Events::Event& event) {
	if (event.GetEventType() != CeEvents::EventType::MouseMoved) {
		CE_TRACE(event);
	}
	CeEvents::EventDispatcher eventDispatcher(event);
	eventDispatcher.Dispatch<Events::WindowCloseEvent>(BIND_EVENT_FN_ONE_PARAM(Application::OnWindowClose));
}

bool Application::OnWindowClose(const Events::WindowCloseEvent&) {
	_running = false;
	CE_INFO("Window closed");
	return true;
}

}
