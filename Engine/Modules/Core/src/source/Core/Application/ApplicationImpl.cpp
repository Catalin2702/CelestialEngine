//
// Module: CelestialEngine/Engine/Modules/Core
// File: ApplicationImpl.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-18
//

#include "Core/Application/I_Application.hpp"

#include <cassert>


namespace CE::Core::Application {

std::atomic<I_Application*> I_Application::_stInstance = nullptr;

void I_Application::PushLayer(Layers::I_Layer* layer) {
	assert(layer != nullptr && "I_Application::PushLayer: Cannot push a null layer");
	_layerStack.PushLayer(layer);
}

void I_Application::PushOverlay(Layers::I_Layer* overlay) {
	assert(overlay != nullptr && "I_Application::PushOverlay: Cannot push a null overlay");
	_layerStack.PushOverlay(overlay);
}

void I_Application::PopLayer(Layers::I_Layer* layer) {
	assert(layer != nullptr && "I_Application::PopLayer: Cannot pop a null layer");
	_layerStack.PopLayer(layer);
}

void I_Application::PopOverlay(Layers::I_Layer* overlay) {
	assert(overlay != nullptr && "I_Application::PopOverlay: Cannot pop a null overlay");
	_layerStack.PopOverlay(overlay);
}

I_Application& I_Application::StGet() {
	assert(_stInstance != nullptr && "I_Application::StGet: _stInstance is null - did you forget to create an Application?");
	return *_stInstance;
}

}
