//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#ifndef CE_WINDOW_WINDOWS_WINDOWSVIEWPORT_HPP
#define CE_WINDOW_WINDOWS_WINDOWSVIEWPORT_HPP

#include "Window/InterfaceWindow.hpp"


struct GLFWwindow;

namespace CE::Window {

class WindowsViewport final : public InterfaceViewport {
public:
	WindowsViewport(const WindowProps& windowProps);
	~WindowsViewport() override;

public:
	void OnUpdate() override;

public:
	[[nodiscard]] inline unsigned int GetWidth() const override { return _data.width; }
	[[nodiscard]] inline unsigned int GetHeight() const override { return _data.height; }
	[[nodiscard]] inline bool IsVSync() const override { return _data.VSync; }
	
	void SetEventCallback(const EventCallbackFn& callback) override;
	void SetWindowCallbacks() override;
	void SetWidth(unsigned int width) override;
	void SetHeight(unsigned int height) override;
	void SetVSync(bool enabled) override;

private:
	void Init(const WindowProps& windowProps);
	void Shutdown();

private:
	GLFWwindow* _window = nullptr;
	WindowData _data;
};

}

#endif //CE_WINDOW_WINDOWS_WINDOWSVIEWPORT_HPP
