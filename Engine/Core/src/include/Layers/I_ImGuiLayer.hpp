//
// Created by Catalin Chirosca on 2026-02-28.
//

#pragma once

#ifndef CE_I_IMGUILAYER_HPP
#define CE_I_IMGUILAYER_HPP

#include "I_Layer.hpp"

namespace CE::Events {

class MouseMovedEvent;
class MouseScrolledEvent;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;

class KeyPressedEvent;
class KeyReleasedEvent;
class KeyTypedEvent;

class WindowResizeEvent;
class WindowCloseEvent;
class AppTickEvent;
class AppUpdateEvent;
class AppRenderEvent;

}

namespace CE::Layers {

class I_ImGuiLayer: public I_Layer {
public:
	I_ImGuiLayer([[maybe_unused]] const std::string& name = "ImGuiLayer"): I_Layer(name) {}
	~I_ImGuiLayer() override = default;
protected:
	virtual bool OnMouseMoved(Events::MouseMovedEvent& event) = 0;
	virtual bool OnMouseScrolled(Events::MouseScrolledEvent& event) = 0;
	virtual bool OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) = 0;
	virtual bool OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) = 0;

	virtual bool OnKeyPressed(Events::KeyPressedEvent& event) = 0;
	virtual bool OnKeyReleased(Events::KeyReleasedEvent& event) = 0;
	virtual bool OnKeyTyped(Events::KeyTypedEvent& event) = 0;

	virtual bool OnWindowResized(Events::WindowResizeEvent& event) = 0;
	virtual bool OnWindowClosed(Events::WindowCloseEvent& event) = 0;
	virtual bool OnAppTick(Events::AppTickEvent& event) = 0;
	virtual bool OnAppUpdate(Events::AppUpdateEvent& event) = 0;
	virtual bool OnAppRender(Events::AppRenderEvent& event) = 0;
};

}

#endif //CE_I_IMGUILAYER_HPP