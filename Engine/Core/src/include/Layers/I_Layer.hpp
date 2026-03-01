//
// Module: Layers
// File: I_Layer.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-19
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_LAYERS_I_LAYER_HPP
#define CE_LAYERS_I_LAYER_HPP

#include <Define/DynamicLinker.hpp>
#include <Events/I_Event.hpp>


/**
 * @namespace CE::Layers
 * @brief Layer system for organizing application logic into stackable components
 * @details Provides the I_Layer interface for defining application layers that can handle events,
 *          update logic, and rendering. Layers are managed in a stack and can be added or removed
 *          dynamically during the application's lifecycle.
 */
namespace CE::Layers {

/**
 * @class I_Layer
 * @brief Abstract base class for application layers
 * @details Layers are stackable components that can handle events, update logic, and rendering.
 *          The layer system allows for organizing application logic into separate, manageable units.
 *          Layers are updated and rendered in order, and events propagate through the layer stack.
 */
class CE_API I_Layer {
public:
	/**
	 * @brief Constructor
	 * @param name Debug name for the layer (only used in debug builds)
	 * @details Creates a layer with an optional debug name for identification
	 */
	I_Layer([[maybe_unused]] const std::string& name = "Layer");

	/**
	 * @brief Virtual destructor
	 * @details Ensures proper cleanup of derived layer classes
	 */
	virtual ~I_Layer() = default;

public:
	/**
	 * @brief Called when the layer is attached to the layer stack
	 * @details Pure virtual method for layer initialization. Called once when the layer
	 *          is added to the application. Use this for setting up resources.
	 */
	virtual void OnAttach() = 0;

	/**
	 * @brief Called when the layer is detached from the layer stack
	 * @details Pure virtual method for layer cleanup. Called once when the layer
	 *          is removed from the application. Use this for releasing resources.
	 */
	virtual void OnDetach() = 0;

	/**
	 * @brief Called every frame to update the layer
	 * @details Pure virtual method for per-frame update logic. Called once per frame
	 *          for all active layers. Use this for game logic, animations, etc.
	 */
	virtual void OnUpdate() = 0;

	/**
	 * @brief Called when an event occurs
	 * @param event Reference to the event to be processed
	 * @return bool True if the event was handled and should stop propagating
	 * @details Pure virtual method for event handling. Events propagate through layers
	 *          until one handles them. Return true to stop event propagation.
	 */
	virtual bool OnEvent(Events::I_Event& event) = 0;

#ifdef CE_DEBUG
public:
	/**
	 * @brief Gets the debug name of the layer
	 * @return const std::string& Reference to the layer's debug name
	 * @details Only available in debug builds. Used for debugging and logging.
	 */
	[[nodiscard]] const std::string& GetDebugName() const { return _debugName; }

protected:
	std::string _debugName;							///< Debug name for the layer (debug builds only)
#endif

};

}

#endif //CE_LAYERS_I_LAYER_HPP
