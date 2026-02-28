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

LayerStack::LayerStack() {
	_layerInsert = _layers.begin();
}

LayerStack::~LayerStack() {
	for (const auto layer: _layers) {
		layer->OnDetach();
		delete layer;
	}
}

void LayerStack::PushLayer(I_Layer* layer) {
	_layerInsert = _layers.emplace(_layerInsert, layer);
	layer->OnAttach();
	++_layerInsert;
}

void LayerStack::PushOverlay(I_Layer* overlay) {
	_layers.emplace_back(overlay);
	overlay->OnAttach();
}

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

void LayerStack::PopOverlay(I_Layer* overlay) {
	if (const auto it = std::find(_layers.begin(), _layers.end(), overlay); it != _layers.end()) {
		overlay->OnDetach();
		_layers.erase(it);
	}
}

}
