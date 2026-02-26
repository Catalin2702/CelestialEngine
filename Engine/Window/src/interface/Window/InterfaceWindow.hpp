//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"

#include <memory>

struct GLFWwindow;


namespace CE::Events {
class Event;
}

namespace CE::Window {

using EventWindowData = Types::Window::WindowData<Events::Event>;
using EventCallbackFn = Types::Window::CallbackFn<Events::Event>;

class CE_API InterfaceViewport {
public:
	virtual ~InterfaceViewport() = default;

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

	template<std::derived_from<InterfaceViewport> T>
	static InterfaceViewport* CreateWindow(const Types::Window::WindowProps& windowProps) {
		auto window = std::make_unique<T>(windowProps);
		return window.release();
	}
};

}
