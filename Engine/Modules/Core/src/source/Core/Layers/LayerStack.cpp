//
// Module: CelestialEngine/Engine/Modules/Core/Layers
// File: LayerStack.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-19
// Updated by: Catalin Chirosca
// Updated: 2026-08-28
//

#include "Core/Layers/LayerStack.hpp"
#include "Core/Layers/I_Layer.hpp"

#include <algorithm>


namespace CE::Core {

static LayerStack::StackContainer::iterator FindLayer(const LayerStack::StackContainer::iterator& begin, const LayerStack::StackContainer::iterator& end, const std::shared_ptr<I_Layer>& layer) {
	return std::ranges::find_if(begin, end, [layer](const std::shared_ptr<I_Layer>& entry) {
		return entry == layer;
	});
}

LayerStack::LayerStack(): _lastLayerIndex(0) {}

LayerStack::~LayerStack() {
	Clear();
}

void LayerStack::Clear() {
	for (const auto& layer: _layers) {
		layer->OnDetach();
	}
	_layers.clear();
	_lastLayerIndex = 0;
}

void LayerStack::PushLayer(const std::shared_ptr<I_Layer>& layer) {
	_layers.emplace(_layers.begin() + _lastLayerIndex, layer);
	_lastLayerIndex++;
	layer->OnAttach();
}

void LayerStack::PushOverlay(const std::shared_ptr<I_Layer>& overlay) {
	_layers.emplace_back(overlay);
	overlay->OnAttach();
}

void LayerStack::PopLayer(const std::shared_ptr<I_Layer>& layer) {
	const auto it = FindLayer(_layers.begin(), _layers.begin() + _lastLayerIndex, layer);
	if (it == _layers.end()) [[unlikely]]
		return;

	layer->OnDetach();
	_layers.erase(it);
	--_lastLayerIndex;
}

void LayerStack::PopOverlay(const std::shared_ptr<I_Layer>& overlay) {
	const auto it = FindLayer(_layers.begin() + _lastLayerIndex, _layers.end(), overlay);
	if (it == _layers.end()) [[unlikely]]
		return;

	overlay->OnDetach();
	_layers.erase(it);
}

// Replacing in place (rather than PopOverlay + PushOverlay) keeps the entry at its original index, so the layer/overlay
// split tracked by _layerInsert stays valid whichever half the replaced entry belonged to.
void LayerStack::ReplaceLayer(const std::shared_ptr<I_Layer>& oldLayer, const std::shared_ptr<I_Layer>& newLayer) {
	const auto it = FindLayer(_layers.begin(), _layers.end(), oldLayer);
	if (it == _layers.end()) [[unlikely]]
		return;

	// Copied, not moved out of the slot: OnDetach runs arbitrary layer code that may walk the stack, and a moved-from
	// entry would leave a null shared_ptr in it. The copy also keeps the old layer alive until this function returns,
	// so it is destroyed (and unsubscribes) only after the new one is attached.
	const auto& detached = *it;
	detached->OnDetach();

	*it = newLayer;
	newLayer->OnAttach();
}

}
