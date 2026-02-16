//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Window/Platforms/Mac/Window.hpp"
#include "Tools/Log/Log.hpp"


namespace CE::Window {

static bool s_GLFWInitialized = false;

PlatformWindow::PlatformWindow(const WindowProps &windowProps)
{
	Init(windowProps);
}

PlatformWindow::~PlatformWindow() {
	Shutdown();
}

void PlatformWindow::OnUpdate() {
	glfwPollEvents();
	glfwSwapBuffers(_window);
}

void PlatformWindow::SetEventCallback(const EventCallbackFn &callback) {
	_data.eventCallback = callback;
}

void PlatformWindow::SetWidth(const unsigned int width) {
	_data.width = width;
}

void PlatformWindow::SetHeight(const unsigned int height) {
	_data.height = height;
}

void PlatformWindow::SetVSync(const bool enabled) {
	if (not s_GLFWInitialized) {
		CE_CORE_WARN("Could not set VSync because GLFW is not initialized.");
		return;
	}
	_data.VSync = enabled;
	glfwSwapInterval(enabled ? 1 : 0);
}

void PlatformWindow::Init(const WindowProps &windowProps) {
	_data.title = windowProps.title;
	_data.width = windowProps.width;
	_data.height = windowProps.height;
	_data.VSync = windowProps.VSync;

	CE_INFO("Creating window {0}, ({1}x{2}), VSync: {3}", _data.title, _data.width, _data.height, _data.VSync);

	if (not s_GLFWInitialized) {
		if (const int success = glfwInit(); not success) {
			CE_CORE_ERROR("Could not initialize GLFW!");
		}
		s_GLFWInitialized = true;
	}

	_window = glfwCreateWindow(
		static_cast<int>(_data.width),
		static_cast<int>(_data.height),
		_data.title.c_str(),
		nullptr,
		nullptr
	);

	glfwMakeContextCurrent(_window);
	glfwSetWindowUserPointer(_window, &_data);
	SetVSync(_data.VSync);
}

void PlatformWindow::Shutdown() {
	glfwDestroyWindow(_window);
	_window = nullptr;
}

Window *Window::CreateWindow(const WindowProps &windowProps) {
	return new PlatformWindow(windowProps);
}

}
