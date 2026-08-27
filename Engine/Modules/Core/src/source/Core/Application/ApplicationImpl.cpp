//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: ApplicationImpl.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-08-27
//

#include "Core/Application/I_Application.hpp"

#include <cassert>


namespace CE::Core {

std::atomic<I_Application*> I_Application::_stInstance = nullptr;

I_Application::~I_Application() {
	_stInstance = nullptr;
}

void I_Application::PushLayer(const std::shared_ptr<I_Layer>& layer) {
	_layerStack.PushLayer(layer);
}

void I_Application::PushOverlay(const std::shared_ptr<I_Layer>& overlay) {
	_layerStack.PushOverlay(overlay);
}

void I_Application::PopLayer(const std::shared_ptr<I_Layer>& layer) {
	_layerStack.PopLayer(layer);
}

void I_Application::PopOverlay(const std::shared_ptr<I_Layer>& overlay) {
	_layerStack.PopOverlay(overlay);
}

void I_Application::ReplaceLayer(const std::shared_ptr<I_Layer>& oldLayer, const std::shared_ptr<I_Layer>& newLayer) {
	_layerStack.ReplaceLayer(oldLayer, newLayer);
}

I_Application& I_Application::StGet() {
	assert(_stInstance != nullptr && "I_Application::StGet: _stInstance is null - did you forget to create an Application?");
	return *_stInstance;
}

float I_Application::GetDeltaTime() const {
	const auto currentTime = Clock::now();
	const auto deltaTime = std::chrono::duration<float>(currentTime - _lastFrameTime.load()).count();
	_lastFrameTime.store(currentTime);
	return deltaTime;
}

void I_Application::ResetDeltaTime() const {
	_lastFrameTime.store(Clock::now());
}

}
