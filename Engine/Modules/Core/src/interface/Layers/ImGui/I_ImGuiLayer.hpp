//
// Module: CelestialEngine/Engine/Modules/Layers
// File: I_ImGuiLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-03-11
//

#pragma once

#ifndef CE_I_IMGUILAYER_HPP
#define CE_I_IMGUILAYER_HPP

#include "Layers/I_RenderLayer.hpp"

namespace CE::Layers {

/**
 * @class I_ImGuiLayer
 * @brief Abstract base class for ImGui overlay layers
 * @details Extends I_Layer to provide a specialized interface for ImGui-based UI layers.
 *			Defines pure virtual methods for handling various input events that are
 *			relevant for ImGui interactions. Concrete implementations (ImGuiOpenGlLayer,
 *			ImGuiMetalLayer) handle platform-specific rendering.
 */
class I_ImGuiLayer: public I_RenderLayer {
public:
	I_ImGuiLayer([[maybe_unused]] const std::string& name = "ImGuiLayer"): I_RenderLayer(name) {}

	~I_ImGuiLayer() override = default;

public:
	void OnUpdate() override {}
	void OnEvent(Events::I_Event&) override {}
};

}

#endif //CE_I_IMGUILAYER_HPP