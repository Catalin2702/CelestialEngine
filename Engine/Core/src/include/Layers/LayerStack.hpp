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
using StackContainer = std::vector<Layer*>;
	
public:
	LayerStack();
	~LayerStack();

public:
	void PushLayer(Layer* layer);
	void PushOverlay(Layer* overlay);
	Layer* PopLayer(Layer* layer);
	Layer* PopOverlay(Layer* overlay);

	StackContainer::iterator begin() { return _layers.begin(); }
	StackContainer::iterator end() { return _layers.end(); }
private:
	StackContainer _layers;
	StackContainer::iterator _layerInsert;
};

}

#endif //CE_LAYERS_LAYERSTACK_HPP
