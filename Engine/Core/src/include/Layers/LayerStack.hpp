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

namespace CE::Layers {

class I_Layer;

class CE_API LayerStack {
using StackContainer = std::vector<I_Layer*>;
	
public:
	LayerStack();
	~LayerStack();

public:
	void PushLayer(I_Layer* layer);
	void PushOverlay(I_Layer* overlay);
	void PopLayer(I_Layer* layer);
	void PopOverlay(I_Layer* overlay);

	StackContainer::iterator begin() { return _layers.begin(); }
	StackContainer::iterator end() { return _layers.end(); }
private:
	StackContainer _layers;
	StackContainer::iterator _layerInsert;
};

}

#endif //CE_LAYERS_LAYERSTACK_HPP
