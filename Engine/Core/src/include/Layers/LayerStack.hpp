//
// Created by Catalin Chirosca on 2026-02-19.
//
// RAII ownership
//

#pragma once

#ifndef CE_LAYERS_LAYERSTACK_HPP
#define CE_LAYERS_LAYERSTACK_HPP

#include <Define/DynamicLinker.hpp>

#include <vector>

namespace CE::Layers {

class Layer;

class CE_API LayerStack {
using Container = std::vector<Layer*>;
	
public:
	LayerStack();
	~LayerStack();

public:
	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	Layer* PopLayer(Layer* layer);
	Layer* PopOverlay(Layer* overlay);

	inline Container::iterator begin() { return _layers.begin(); }
	inline Container::iterator end() { return _layers.end(); }
private:
	Container _layers;
	Container::iterator _layerInsert;
};

}

#endif //CE_LAYERS_LAYERSTACK_HPP
