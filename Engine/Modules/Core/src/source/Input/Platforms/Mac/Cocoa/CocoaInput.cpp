//
// Module: CelestialEngine/Engine/Input/Platforms/Mac/Cocoa
// File: CocoaInput.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-04-19
//

#include "Input/Platforms/Mac/Cocoa/CocoaInput.hpp"

#include "Bridge/AppKit/Window/WindowBridge.h"
#include "Core/Application/Platforms/Mac/Cocoa/CocoaApplication.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"
#include "Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"


namespace CE::Core::Input {

bool CocoaInput::_IsKeyPressedImpl(const KeyCode::KeyboardKeyCode keyCode) {
	// Convert CE keycode to macOS virtual key code and check state
	const auto cocoaKeyCode = KeyCode::CocoaKeyCodeFromKeyboard(keyCode);
	return Apple::Bridge::IsKeyPressed(cocoaKeyCode);
}

bool CocoaInput::_IsMouseButtonPressedImpl(const KeyCode::MouseButtonCode buttonCode) {
	// Convert CE mouse button code to Cocoa button number
	const auto cocoaButton = KeyCode::CocoaButtonNumberFromMouseButton(buttonCode);
	return Apple::Bridge::IsMouseButtonPressed(cocoaButton);
}

float CocoaInput::_GetMouseXImpl() {
	if (not _cocoaWindow) {
		_InitCocoaWindow();
	}

	float x = 0.0f, y = 0.0f;
	Apple::Bridge::GetMousePosition(_cocoaWindow, &x, &y);
	return x;
}

float CocoaInput::_GetMouseYImpl() {
	if (not _cocoaWindow) {
		_InitCocoaWindow();
	}

	float x = 0.0f, y = 0.0f;
	Apple::Bridge::GetMousePosition(_cocoaWindow, &x, &y);
	return y;
}

std::pair<float, float> CocoaInput::_GetMouseXYImpl() {
	if (not _cocoaWindow) {
		_InitCocoaWindow();
	}

	float x = 0.0f, y = 0.0f;
	Apple::Bridge::GetMousePosition(_cocoaWindow, &x, &y);
	return {x, y};
}

void CocoaInput::_InitCocoaWindow() {
	const auto& window = Core::Application::CocoaApplication::StGet().GetWindow();
	const auto& cocoaWindow = dynamic_cast<const Window::CocoaWindow&>(window);
	_cocoaWindow = cocoaWindow.GetCocoaWindow();
}

}
