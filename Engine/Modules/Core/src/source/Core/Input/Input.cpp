//
// Module: CelestialEngine/Engine/Modules/Core/Input
// File: Input.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-22
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#include "Core/Input/Input.hpp"

namespace CE::Core {

void Input::Init() {
	if (I_Input::_instance)
		return;

	I_Input::_instance = new Input();
}

void Input::Shutdown() {
	if (not I_Input::_instance)
		return;

	delete I_Input::_instance;
	I_Input::_instance = nullptr;
}

bool Input::_IsKeyPressedImpl(const Types::KeyboardKeyCode keyCode) {
	return _state.IsKeyDown(keyCode);
}

bool Input::_IsMouseButtonPressedImpl(const Types::MouseButtonCode buttonCode) {
	return _state.IsMouseButtonDown(buttonCode);
}

float Input::_GetMouseXImpl() {
	return _state.GetMouseX();
}

float Input::_GetMouseYImpl() {
	return _state.GetMouseY();
}

std::pair<float, float> Input::_GetMouseXYImpl() {
	return _state.GetMouseXY();
}

}
