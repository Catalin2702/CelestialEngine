//
// Module: CelestialEngine/Engine/Modules/Core/Layers
// File: LayerStack.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-19
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include "Core/Layers/LayerStack.hpp"
#include "Core/Layers/I_Layer.hpp"

#include <algorithm>


namespace CE::Core {

LayerStack::LayerStack(): _layerInsert(0) {}

LayerStack::~LayerStack() {
	Clear();
}

void LayerStack::Clear() {
	for (const auto& layer: _layers) {
		layer->OnDetach();
	}
	_layers.clear();
	_layerInsert = 0;
}

void LayerStack::PushLayer(I_Layer* layer) {
	_layers.emplace(_layers.begin() + _layerInsert, layer);
	_layerInsert++;
	layer->OnAttach();
}

void LayerStack::PushOverlay(I_Layer* overlay) {
	_layers.emplace_back(overlay);
	overlay->OnAttach();
}

// The stack owns its layers through shared_ptr, so a layer is looked up by comparing the raw addresses. Wrapping the
// argument in a shared_ptr to use std::find instead would build a *second*, independent control block over the same
// pointer, and deleting that temporary at the end of the function would destroy a layer the stack still owns.
static LayerStack::StackContainer::iterator FindLayer(LayerStack::StackContainer& layers, const I_Layer* layer) {
	return std::ranges::find_if(layers, [layer](const std::shared_ptr<I_Layer>& entry) {
		return entry.get() == layer;
	});
}

void LayerStack::PopLayer(I_Layer* layer) {
	const auto it = FindLayer(_layers, layer);
	if (it == _layers.end()) [[unlikely]]
		return;

	layer->OnDetach();
	_layers.erase(it);
	--_layerInsert;
}

void LayerStack::PopOverlay(I_Layer* overlay) {
	const auto it = FindLayer(_layers, overlay);
	if (it == _layers.end()) [[unlikely]]
		return;

	overlay->OnDetach();
	_layers.erase(it);
}

}
