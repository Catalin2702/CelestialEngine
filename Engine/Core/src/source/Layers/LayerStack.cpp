//
// Created by Catalin Chirosca on 2026-02-19.
//

#include "Layers/LayerStack.hpp"
#include "Layers/Layer.hpp"


namespace CE::Layers {


LayerStack::LayerStack() {
	_layerInsert = _layers.begin();
}

LayerStack::~LayerStack() {
	for (const auto layer: _layers)
		delete layer;
}

void LayerStack::PushLayer(Layer* layer) {
	_layerInsert = _layers.emplace(_layerInsert, layer);
}

void LayerStack::PushOverlay(Layer* overlay) {
	_layers.emplace_back(overlay);
}

Layer* LayerStack::PopLayer(Layer* layer) {
	if (const auto it = std::find(_layers.begin(), _layers.end(), layer); it != _layers.end()) {
		_layers.erase(it);
		--_layerInsert;
		return layer;
	}
	return nullptr;
}

Layer* LayerStack::PopOverlay(Layer* overlay) {
	if (const auto it = std::find(_layers.begin(), _layers.end(), overlay); it != _layers.end()) {
		_layers.erase(it);
		return overlay;
	}
	return nullptr;
}

}
