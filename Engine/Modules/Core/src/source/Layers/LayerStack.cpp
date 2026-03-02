//
// Module: Layers
// File: LayerStack.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-19
// Updated: 2026-02-28
//

#include "Layers/LayerStack.hpp"
#include "Layers/I_Layer.hpp"


namespace CE::Layers {

/**
 * @brief LayerStack constructor implementation
 * @details Initializes the layer insert iterator to the beginning of the vector.
 *			This ensures layers are inserted before overlays.
 */
LayerStack::LayerStack() {
	_layerInsert = _layers.begin();
}

/**
 * @brief LayerStack destructor implementation
 * @details Detaches and deletes all layers in the stack by calling Clear()
 */
LayerStack::~LayerStack() {
	Clear();
}

/**
 * @brief Clears all layers from the stack
 * @details Detaches and deletes all layers, then clears the vector and resets
 *			the insert iterator. This can be called explicitly for cleanup before
 *			the destructor, or will be called automatically by the destructor.
 */
void LayerStack::Clear() {
	for (const auto layer: _layers) {
		layer->OnDetach();
		delete layer;
	}
	_layers.clear();
	_layerInsert = _layers.begin();
}

/**
 * @brief Pushes a layer onto the stack
 * @param layer Pointer to the layer to add
 * @details Inserts the layer at the current insert position (before overlays),
 *			calls OnAttach(), and updates the insert iterator
 */
void LayerStack::PushLayer(I_Layer* layer) {
	_layerInsert = _layers.emplace(_layerInsert, layer);
	layer->OnAttach();
	++_layerInsert;
}

/**
 * @brief Pushes an overlay onto the stack
 * @param overlay Pointer to the overlay to add
 * @details Inserts the overlay at the end of the stack (after all layers)
 *			and calls OnAttach()
 */
void LayerStack::PushOverlay(I_Layer* overlay) {
	_layers.emplace_back(overlay);
	overlay->OnAttach();
}

/**
 * @brief Removes a layer from the stack
 * @param layer Pointer to the layer to remove
 * @details Finds the layer in the stack, calls OnDetach(), removes it from
 *			the vector, and adjusts the insert iterator if necessary
 */
void LayerStack::PopLayer(I_Layer* layer) {
	if (const auto it = std::find(_layers.begin(), _layers.end(), layer); it != _layers.end()) {
		layer->OnDetach();
		_layers.erase(it);
		const bool erasedBeforeInsert = (it < _layerInsert);
		layer->OnDetach();
		_layers.erase(it);
		if (erasedBeforeInsert) {
			--_layerInsert;
		}
	}
}

/**
 * @brief Removes an overlay from the stack
 * @param overlay Pointer to the overlay to remove
 * @details Finds the overlay in the stack, calls OnDetach(), and removes it
 */
void LayerStack::PopOverlay(I_Layer* overlay) {
	if (const auto it = std::find(_layers.begin(), _layers.end(), overlay); it != _layers.end()) {
		overlay->OnDetach();
		_layers.erase(it);
	}
}

}
