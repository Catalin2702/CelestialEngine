//
// Created by Catalin Chirosca on 2026-02-15.
//

#include "Core/Application.hpp"
#include "Window/InterfaceWindow.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace CeEvents = CE::Events;
namespace CeWindow = CE::Window;

namespace CE::Core {

Application::Application() {
	_window = std::unique_ptr<CeWindow::Window>(CeWindow::Window::CreateWindow(CeWindow::WindowProps("CelestialEngine", 1280, 720, true)));
	_running = true;
}

Application::~Application() {
}

void Application::Run() {
	while (_running) {
		glClearColor(1, 0, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		_window->OnUpdate();
	}
}

}
