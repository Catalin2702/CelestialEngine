//
// Module: Window
// File: I_Viewport.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_WINDOW_I_VIEWPORT_HPP
#define CE_WINDOW_I_VIEWPORT_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"

#include <memory>

struct GLFWwindow;


namespace CE::Events {
class I_Event;
}

namespace CE::Window {

using EventWindowData = Types::Window::WindowData<Events::I_Event>;
using EventCallbackFn = Types::Window::CallbackFn<Events::I_Event>;

class CE_API I_Viewport {
public:
	virtual ~I_Viewport() = default;

public:
	virtual void OnUpdate() = 0;

public:
	[[nodiscard]] virtual unsigned int GetWidth() const = 0;
	[[nodiscard]] virtual unsigned int GetHeight() const = 0;
	[[nodiscard]] virtual bool IsVSync() const = 0;
	[[nodiscard]] virtual GLFWwindow* GetGLFWwindow() const = 0;

	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetWindowCallbacks() = 0;
	virtual void SetWidth(unsigned int width) = 0;
	virtual void SetHeight(unsigned int height) = 0;
	virtual void SetVSync(bool enabled) = 0;

	template<std::derived_from<I_Viewport> T>
	static I_Viewport* CreateWindow(const Types::Window::WindowProps& windowProps) {
		auto window = std::make_unique<T>(windowProps);
		return window.release();
	}
};

}

#endif // CE_WINDOW_I_VIEWPORT_HPP
