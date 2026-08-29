//
// Module: CelestialEngine/Engine/Modules/Core/Layers
// File: I_Layer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-19
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_LAYERS_I_LAYER_HPP
#define CE_CORE_LAYERS_I_LAYER_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <string>
#include <utility>


namespace CE::Core {

/**
 * @class I_Layer
 * @brief Abstract base class for application layers
 * @details Layers are stackable components that can handle events, update logic, and rendering.
 *			The layer system allows for organizing application logic into separate, manageable units.
 *			Layers are updated and rendered in order, and events propagate through the layer stack.
 */
class CE_CORE_API I_Layer {
protected:
	/**
	 * @brief Constructor
	 * @param name Name for the layer
	 * @details Creates a layer with an optional name for identification
	 */
	I_Layer(std::string name = "Layer"): _name(std::move(name)) {}

public:
	/**
	 * @brief Virtual destructor
	 * @details Ensures proper cleanup of derived layer classes
	 */
	virtual ~I_Layer() = default;

public:
	/**
	 * @brief Called when the layer is attached to the layer stack
	 * @details Pure virtual method for layer initialization. Called once when the layer
	 *			is added to the application. Use this for setting up resources.
	 */
	virtual void OnAttach() = 0;

	/**
	 * @brief Called when the layer is detached from the layer stack
	 * @details Pure virtual method for layer cleanup. Called once when the layer
	 *			is removed from the application. Use this for releasing resources.
	 */
	virtual void OnDetach() = 0;

	/**
	 * @brief Called every frame to update the layer
	 * @details Pure virtual method for per-frame update logic. Called once per frame
	 *			for all active layers. Use this for game logic, animations, etc.
	 */
	virtual void OnUpdate() = 0;

	/**
	 * @brief Pure virtual method for handling rendering logic
	 */
	virtual void OnRender() const = 0;

	/**
	 * @brief Pure virtual method for handling begin frame logic
	 * @param deltaTime Time elapsed since the last frame, used for time-based updates
	 */
	virtual void Begin(f32 deltaTime) = 0;

	/**
	 * @brief Pure virtual method for handling end frame logic
	 */
	virtual void End() = 0;

	/**
	 * @brief Subscribes the layer's handlers to the event hub (no-op by default)
	 */
	virtual void SubscribeToEventHub() {}
	/**
	 * @brief Removes the layer's handlers from the event hub (no-op by default)
	 */
	virtual void UnsubscribeFromEventHub() {}

public:
	/**
	 * @brief Gets the name of the layer
	 * @return const std::string& Reference to the layer's name
	 */
	[[nodiscard]] const std::string& GetName() const { return _name; }

protected:
	std::string _name;							///< Name for the layer

protected:
	mutable f32 _deltaTime = 0.f;					///< Time accumulator for frame timing
	bool _currentFrameStarted = false;				///< Flag to track if the current frame has started
};

}

#endif //CE_CORE_LAYERS_I_LAYER_HPP
