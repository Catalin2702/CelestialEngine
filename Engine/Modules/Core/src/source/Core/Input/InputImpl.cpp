//
// Module: CelestialEngine/Engine/Core/Input/
// File: InputImpl.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#include "Core/Input/I_Input.hpp"

#include <cassert>

namespace CE::Core {

I_Input* I_Input::_instance = nullptr;

bool I_Input::IsKeyPressed(const Types::KeyboardKeyCode keyCode) {
	return _instance->_IsKeyPressedImpl(keyCode);
}

bool I_Input::IsMouseButtonPressed(const Types::MouseButtonCode buttonCode) {
	return _instance->_IsMouseButtonPressedImpl(buttonCode);
}

float I_Input::GetMouseX() {
	return _instance->_GetMouseXImpl();
}

float I_Input::GetMouseY() {
	return _instance->_GetMouseYImpl();
}

std::pair<float, float> I_Input::GetMouseXY() {
	return _instance->_GetMouseXYImpl();
}

I_Input* I_Input::Get() {
	assert(_instance &&  "I_Input::Get: Input system not initialized! Call Input::Init() before using.");
	return _instance;
}

}
