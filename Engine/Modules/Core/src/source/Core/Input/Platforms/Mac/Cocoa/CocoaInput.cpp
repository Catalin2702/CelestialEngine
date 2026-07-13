//
// Module: CelestialEngine/Engine/Input/Core/Platforms/Mac/Cocoa
// File: CocoaInput.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Core/Input/Platforms/Mac/Cocoa/CocoaInput.hpp"

#include "Apple/Bridge/AppKit/Window/WindowBridge.h"
#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

namespace CE::Core {

bool CocoaInput::_IsKeyPressedImpl(const Types::KeyboardKeyCode keyCode) {
	// Convert CE keycode to macOS virtual key code and check state
	const auto cocoaKeyCode = Types::CocoaKeyCodeFromKeyboard(keyCode);
	return Native::IsKeyPressed(cocoaKeyCode);
}

bool CocoaInput::_IsMouseButtonPressedImpl(const Types::MouseButtonCode buttonCode) {
	// Convert CE mouse button code to Cocoa button number
	const auto cocoaButton = Types::CocoaButtonNumberFromMouseButton(buttonCode);
	return Native::IsMouseButtonPressed(cocoaButton);
}

float CocoaInput::_GetMouseXImpl() {
	if (not _window) {
		_InitWindow();
	}

	float x = 0.0f, y = 0.0f;
	Native::GetMousePosition(_window, &x, &y);
	return x;
}

float CocoaInput::_GetMouseYImpl() {
	if (not _window) {
		_InitWindow();
	}

	float x = 0.0f, y = 0.0f;
	Native::GetMousePosition(_window, &x, &y);
	return y;
}

std::pair<float, float> CocoaInput::_GetMouseXYImpl() {
	if (not _window) {
		_InitWindow();
	}

	float x = 0.0f, y = 0.0f;
	Native::GetMousePosition(_window, &x, &y);
	return {x, y};
}

void CocoaInput::_InitWindow() {
	const auto& window = CocoaApplication::StGet().GetWindow();
	const auto& cocoaWindow = dynamic_cast<const CocoaWindow&>(window);
	_window = cocoaWindow.GetWindow();
}

}
