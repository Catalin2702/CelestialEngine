//
// Created by Catalin Chirosca on 2026-02-24.
//

#pragma once

#ifndef CE_LAYERS_IMGUIOPENGLLAYER_HPP
#define CE_LAYERS_IMGUIOPENGLLAYER_HPP

#include "Layers/I_ImGuiLayer.hpp"

struct GLFWwindow;

namespace CE::Window {
class OpenGLViewport;
}


namespace CE::Layers {

class ImGuiOpenGlLayer final : public I_ImGuiLayer {
public:
	ImGuiOpenGlLayer();

public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;

	bool OnEvent(Events::I_Event& event) override;

protected:
	bool OnKeyPressed(Events::KeyPressedEvent& event) override;
	bool OnKeyReleased(Events::KeyReleasedEvent& event) override;
	bool OnKeyTyped(Events::KeyTypedEvent& event) override;

	bool OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) override;
	bool OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) override;
	bool OnMouseMoved(Events::MouseMovedEvent& event) override;
	bool OnMouseScrolled(Events::MouseScrolledEvent& event) override;

	bool OnWindowResized(Events::WindowResizeEvent& event) override;

private:
	float _time = 0.0f;
	GLFWwindow* _glfwWindow = nullptr;
	Window::OpenGLViewport* _viewport = nullptr;
};

}

#endif //CE_LAYERS_IMGUIOPENGLLAYER_HPP
