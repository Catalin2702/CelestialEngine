//
// Created by Catalin Chirosca on 2026-02-24.
//

#pragma once

#ifndef CE_LAYERS_IMGUIMETALLAYER_HPP
#define CE_LAYERS_IMGUIMETALLAYER_HPP

#include "Layers/Layer.hpp"


namespace CE::Layers {

class ImGuiMetalLayer final : public Layer {
public:
	ImGuiMetalLayer();

public:
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(Events::Event& event) override;
};

}


#endif //CE_LAYERS_IMGUIMETALLAYER_HPP
