//
// Created by Catalin Chirosca on 2026-02-24.
//

#pragma once

#ifndef CE_LAYERS_IMGUIOPENGLLAYER_HPP
#define CE_LAYERS_IMGUIOPENGLLAYER_HPP

#include "Layers/Layer.hpp"

struct GLFWwindow;

namespace CE::Window {
class OpenGLViewport;
}


namespace CE::Layers {

class ImGuiOpenGlLayer final : public Layer {
public:
	ImGuiOpenGlLayer();

public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(Events::Event& event) override;

private:
	float _time = 0.0f;
	GLFWwindow* _glfwWindow = nullptr;
	Window::OpenGLViewport* _viewport = nullptr;
};

}

#endif //CE_LAYERS_IMGUIOPENGLLAYER_HPP
