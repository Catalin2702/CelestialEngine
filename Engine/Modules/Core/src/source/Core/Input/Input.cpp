//
// Module: CelestialEngine/Engine/Modules/Core/Input
// File: Input.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-22
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#include "Core/Input/Input.hpp"

#include <cassert>

namespace CE::Core {

Input* Input::_instance = nullptr;

void Input::Init() {
	if (_instance)
		return;

	_instance = new Input();
}

void Input::Shutdown() {
	if (not _instance)
		return;

	delete _instance;
	_instance = nullptr;
}

Input& Input::Get() {
	assert(_instance && "Input::Get: Input system not initialized! Call Input::Init() before using.");
	return *_instance;
}

bool Input::IsKeyPressed(const Types::KeyboardKeyCode keyCode) {
	return _instance->_state.IsKeyDown(keyCode);
}

bool Input::IsKeyJustPressed(const Types::KeyboardKeyCode keyCode) {
	return _instance->_state.IsKeyJustPressed(keyCode);
}

bool Input::IsKeyJustReleased(const Types::KeyboardKeyCode keyCode) {
	return _instance->_state.IsKeyJustReleased(keyCode);
}

bool Input::IsMouseButtonPressed(const Types::MouseButtonCode buttonCode) {
	return _instance->_state.IsMouseButtonDown(buttonCode);
}

bool Input::IsMouseButtonJustPressed(const Types::MouseButtonCode buttonCode) {
	return _instance->_state.IsMouseButtonJustPressed(buttonCode);
}

bool Input::IsMouseButtonJustReleased(const Types::MouseButtonCode buttonCode) {
	return _instance->_state.IsMouseButtonJustReleased(buttonCode);
}

float Input::GetMouseX() {
	return _instance->_state.GetMouseX();
}

float Input::GetMouseY() {
	return _instance->_state.GetMouseY();
}

std::pair<float, float> Input::GetMouseXY() {
	return _instance->_state.GetMouseXY();
}

float Input::GetScrollDeltaX() {
	return _instance->_state.GetScrollDeltaX();
}

float Input::GetScrollDeltaY() {
	return _instance->_state.GetScrollDeltaY();
}

void Input::EndFrame() {
	_instance->_state.EndFrame();
}

}
