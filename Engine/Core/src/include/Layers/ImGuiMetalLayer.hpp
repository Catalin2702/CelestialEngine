//
// Module: Layers
// File: ImGuiMetalLayer.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-24
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_LAYERS_IMGUIMETALLAYER_HPP
#define CE_LAYERS_IMGUIMETALLAYER_HPP

#include "Layers/I_ImGuiLayer.hpp"

struct GLFWwindow;

namespace CA {
class MetalLayer;
}
namespace MTL {
class CommandQueue;
class Device;
}
namespace CE::Window {
class MetalViewport;
}


namespace CE::Layers {

class ImGuiMetalLayer final : public I_ImGuiLayer {
public:
	ImGuiMetalLayer();

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

	// Cached pointers per evitare lookup ripetuti ogni frame
	Window::MetalViewport* _viewport = nullptr;
	GLFWwindow* _glfwWindow = nullptr;
	MTL::Device* _metalDevice = nullptr;
	MTL::CommandQueue* _commandQueue = nullptr;
	CA::MetalLayer* _metalLayer = nullptr;

};

}


#endif //CE_LAYERS_IMGUIMETALLAYER_HPP
