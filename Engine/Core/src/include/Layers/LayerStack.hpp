//
// Module: Layers
// File: LayerStack.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-19
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_LAYERS_LAYERSTACK_HPP
#define CE_LAYERS_LAYERSTACK_HPP

#include <Define/DynamicLinker.hpp>

#include <vector>

/**
 * @namespace CE::Layers
 * @brief Layer system for organizing application logic into stackable components
 * @details Provides the LayerStack class for managing a collection of layers and overlays.
 *          The LayerStack allows adding and removing layers dynamically, and ensures that
 *          layers are updated and rendered in the correct order. Regular layers are inserted
 *          before overlays, allowing overlays to receive events first.
 */
namespace CE::Layers {

class I_Layer;

/**
 * @class LayerStack
 * @brief Container for managing application layers and overlays
 * @details Maintains an ordered collection of layers where regular layers are inserted
 *          before overlays. Layers are updated/rendered in order, while events propagate
 *          in reverse order (overlays receive events first). This allows overlays like
 *          UI or debug tools to intercept events before they reach game layers.
 */
class CE_API LayerStack {
using StackContainer = std::vector<I_Layer*>;
	
public:
	/**
	 * @brief Constructor
	 * @details Initializes an empty layer stack
	 */
	LayerStack();

	/**
	 * @brief Destructor
	 * @details Cleans up all layers in the stack
	 */
	~LayerStack();

public:
	/**
	 * @brief Adds a layer to the stack
	 * @param layer Pointer to the layer to add
	 * @details Layers are inserted before overlays in the stack. The layer's
	 *          OnAttach() method is called after insertion.
	 */
	void PushLayer(I_Layer* layer);

	/**
	 * @brief Adds an overlay to the stack
	 * @param overlay Pointer to the overlay layer to add
	 * @details Overlays are always inserted at the end of the stack, after all
	 *          regular layers. The overlay's OnAttach() method is called after insertion.
	 */
	void PushOverlay(I_Layer* overlay);

	/**
	 * @brief Removes a layer from the stack
	 * @param layer Pointer to the layer to remove
	 * @details The layer's OnDetach() method is called before removal
	 */
	void PopLayer(I_Layer* layer);

	/**
	 * @brief Removes an overlay from the stack
	 * @param overlay Pointer to the overlay to remove
	 * @details The overlay's OnDetach() method is called before removal
	 */
	void PopOverlay(I_Layer* overlay);

	/**
	 * @brief Gets an iterator to the beginning of the layer stack
	 * @return StackContainer::iterator Iterator pointing to the first layer
	 */
	StackContainer::iterator begin() { return _layers.begin(); }

	/**
	 * @brief Gets an iterator to the end of the layer stack
	 * @return StackContainer::iterator Iterator pointing past the last layer
	 */
	StackContainer::iterator end() { return _layers.end(); }

private:
	StackContainer _layers;							///< Vector containing all layers and overlays
	StackContainer::iterator _layerInsert;			///< Iterator marking the position to insert layers (before overlays)
};

}

#endif //CE_LAYERS_LAYERSTACK_HPP
