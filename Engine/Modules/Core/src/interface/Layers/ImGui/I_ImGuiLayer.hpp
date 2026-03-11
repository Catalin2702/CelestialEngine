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

	void OnAttach() override { _Init(); }
	void OnDetach() override { _Shutdown(); }

protected:
	/**
	 * @brief Platform-specific initialization logic for ImGui layer
	 * @details Initializes ImGui context, sets up rendering backend, and configures input handling.
	 *			Caches necessary pointers for use in rendering. Should be called when the layer is attached.
	 */
	virtual void _Init() = 0;
	/**
	 * @brief Platform-specific shutdown logic for ImGui layer
	 * @details Shuts down ImGui backend and destroys ImGui context. Should be called when the layer is detached.
	 */
	virtual void _Shutdown() = 0;

public:
	void OnUpdate() override {}
	void OnEvent(Events::I_Event&) override {}

protected:
	bool _initialized = false;						///< Flag to track if ImGui Metal backend was successfully initialized
};

}

#endif //CE_I_IMGUILAYER_HPP