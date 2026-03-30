//
// Module: CelestialEngine/Engine/Input/
// File: InputImpl.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-30
//

#include "Input/I_Input.hpp"

#include "Types/Window/WindowProps.hpp"

#include "Input/Platforms/Common/Glfw/GlfwInput.hpp"

#ifdef CE_PLATFORM_MACOS
#include "Input/Platforms/Mac/Cocoa/CocoaInput.hpp"
#endif

#include <cassert>

namespace CE::Input {

I_Input* I_Input::_instance = nullptr;

bool I_Input::IsKeyPressed(const KeyCode::KeyboardKeyCode keyCode) {
	return _instance->_IsKeyPressedImpl(keyCode);
}

bool I_Input::IsMouseButtonPressed(const KeyCode::MouseButtonCode buttonCode) {
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
	assert(_instance &&  "I_Input::Get: Input system not initialized! Call InitInput() before using.");
	return _instance;
}

void InitInput(const Types::Window::WindowApi windowApi) {
	if (I_Input::_instance) {
		return;
	}
	if (windowApi == Types::Window::WindowApi::GLFW)
		I_Input::_instance = new GlfwInput();
#ifdef CE_PLATFORM_MACOS
	if (windowApi == Types::Window::WindowApi::Cocoa)
		I_Input::_instance = new CocoaInput();
#endif
}

void ShutdownInput() {
	if (I_Input::_instance) {
		delete I_Input::_instance;
		I_Input::_instance = nullptr;
	}
}

}
