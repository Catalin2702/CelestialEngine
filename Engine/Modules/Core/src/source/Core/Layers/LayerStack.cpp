//
// Module: CelestialEngine/Engine/Modules/Core/Layers
// File: LayerStack.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-19
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#include "Core/Layers/LayerStack.hpp"
#include "Core/Layers/I_Layer.hpp"

namespace CE::Core {

LayerStack::LayerStack(): _layerInsert(0) {}

LayerStack::~LayerStack() {
	Clear();
}

void LayerStack::Clear() {
	for (const auto layer: _layers) {
		layer->OnDetach();
		delete layer;
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

void LayerStack::PopLayer(I_Layer* layer) {
	if (const auto it = std::find(_layers.begin(), _layers.end(), layer); it != _layers.end()) {
		layer->OnDetach();
		_layers.erase(it);
		--_layerInsert;
	}
}

void LayerStack::PopOverlay(I_Layer* overlay) {
	if (const auto it = std::find(_layers.begin(), _layers.end(), overlay); it != _layers.end()) {
		overlay->OnDetach();
		_layers.erase(it);
	}
}

}
