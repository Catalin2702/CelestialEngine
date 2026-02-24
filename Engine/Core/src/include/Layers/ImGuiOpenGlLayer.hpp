//
// Created by Catalin Chirosca on 2026-02-24.
//

#pragma once

#ifndef CE_LAYERS_IMGUIOPENGLLAYER_HPP
#define CE_LAYERS_IMGUIOPENGLLAYER_HPP

#include "Layers/Layer.hpp"


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
};

}

#endif //CE_LAYERS_IMGUIOPENGLLAYER_HPP
