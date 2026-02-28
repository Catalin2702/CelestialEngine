//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#ifndef CE_WINDOW_WINDOWS_WINDOWSVIEWPORT_HPP
#define CE_WINDOW_WINDOWS_WINDOWSVIEWPORT_HPP

#include "Window/I_Viewport.hpp"

#include "Types/Window/WindowDestructor.hpp"
#include "Types/Window/WindowProps.hpp"


struct GLFWwindow;

namespace CeTypeWindow = CE::Types::Window;

namespace CE::Window {

class OpenGLViewport final : public I_Viewport {
public:
	OpenGLViewport(const CeTypeWindow::WindowProps& windowProps);
	~OpenGLViewport() override;

public:
	void OnUpdate() override;

public:
	[[nodiscard]] unsigned int GetWidth() const override { return _data.width; }
	[[nodiscard]] unsigned int GetHeight() const override { return _data.height; }
	[[nodiscard]] bool IsVSync() const override { return _data.VSync; }
	[[nodiscard]] GLFWwindow* GetGLFWwindow() const override { return _glfwWindow.get(); }
	
	void SetEventCallback(const EventCallbackFn& callback) override;
	void SetWindowCallbacks() override;
	void SetWidth(unsigned int width) override;
	void SetHeight(unsigned int height) override;
	void SetVSync(bool enabled) override;

private:
	void _Init();
	void _Shutdown();

private:
	CeTypeWindow::GLFWwindowPtr _glfwWindow = nullptr;
	EventWindowData _data;
};

}

#endif //CE_WINDOW_WINDOWS_WINDOWSVIEWPORT_HPP
