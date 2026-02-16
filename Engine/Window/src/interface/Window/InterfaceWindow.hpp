//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#include "Core/Core.hpp"

#include <functional>
#include <string>


namespace CE::Events {

class Event;

}

namespace CE::Window {

struct WindowProps {
public:
	std::string title;
	unsigned int width;
	unsigned int height;
	bool VSync;

public:
	WindowProps(std::string title, unsigned int width, unsigned int height, bool VSync);
};

class CE_API Window {
public:
	using EventCallbackFn = std::function<void(CE::Events::Event&)>;

public:
	virtual ~Window() = default;

public:
	virtual void OnUpdate() = 0;

public:
	[[nodiscard]] virtual unsigned int GetWidth() const = 0;
	[[nodiscard]] virtual unsigned int GetHeight() const = 0;
	[[nodiscard]] virtual bool IsVSync() const = 0;

	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
	virtual void SetWidth(unsigned int width) = 0;
	virtual void SetHeight(unsigned int height) = 0;
	virtual void SetVSync(bool enabled) = 0;

	static Window* CreateWindow(const WindowProps& windowProps);
};

}