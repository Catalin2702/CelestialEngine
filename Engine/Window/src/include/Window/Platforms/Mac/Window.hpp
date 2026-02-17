//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#ifndef CE_MAC_WINDOW_HPP
#define CE_MAC_WINDOW_HPP

#include "../interface/Window/InterfaceWindow.hpp"

#include <string>
#include <GLFW/glfw3.h>


namespace CE::Window {

class PlatformWindow final : public Window {

	struct WindowData {
		std::string title;
		unsigned int width, height;
		bool VSync;
		EventCallbackFn eventCallback;
	};

public:
	PlatformWindow(const WindowProps& windowProps);
	~PlatformWindow() override;

public:
	void OnUpdate() override;

public:
	[[nodiscard]] inline unsigned int GetWidth() const override { return _data.width; }
	[[nodiscard]] inline unsigned int GetHeight() const override { return _data.height; }
	[[nodiscard]] inline bool IsVSync() const override { return _data.VSync; }
	
	void SetEventCallback(const EventCallbackFn& callback) override;
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

#endif //CE_MAC_WINDOW_HPP
