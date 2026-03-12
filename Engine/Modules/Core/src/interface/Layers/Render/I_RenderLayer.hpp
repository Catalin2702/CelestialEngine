//
// Module: CelestialEngine/Engine/Modules/Layers/Render
// File: I_RenderLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-11
// Updated by: Catalin Chirosca
// Updated: 2026-03-12
//

#pragma once

#ifndef CE_LAYERS_I_RENDERLAYER_HPP
#define CE_LAYERS_I_RENDERLAYER_HPP

#include "Layers/I_Layer.hpp"

#include <string>


namespace CE::Layers {

/**
 * @class I_RenderLayer
 * @brief Abstract base class for render layers
 * @details Extends I_Layer to provide an interface for layers that handle rendering logic.
 *			Defines pure virtual methods for rendering and frame management. This allows
 *			for separation of rendering logic from other types of layers.
 */
class I_RenderLayer: public I_Layer {
public:
	I_RenderLayer([[maybe_unused]] const std::string& name = "RenderLayer") : I_Layer(name) {}
	~I_RenderLayer() override = default;

public:
	/**
	 * @brief Pure virtual method for handling rendering logic
	 */
	virtual void OnRender() const = 0;

	/**
	 * @brief Pure virtual method for handling begin frame logic
	 */
	virtual void Begin() = 0;

	/**
	 * @brief Pure virtual method for handling end frame logic
	 */
	virtual void End() = 0;

protected:
	mutable float _time = 0.f;						///< Time accumulator for frame timing
	bool _currentFrameStarted = false;				///< Flag to track if the current frame has started
};

}

#endif //CE_LAYERS_I_RENDERLAYER_HPP
