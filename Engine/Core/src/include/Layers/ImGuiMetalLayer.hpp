//
// Created by Catalin Chirosca on 2026-02-24.
//

#pragma once

#ifndef CE_LAYERS_IMGUIMETALLAYER_HPP
#define CE_LAYERS_IMGUIMETALLAYER_HPP

#include "Layers/Layer.hpp"

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

class ImGuiMetalLayer final : public Layer {
public:
	ImGuiMetalLayer();

public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(Events::Event& event) override;

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
