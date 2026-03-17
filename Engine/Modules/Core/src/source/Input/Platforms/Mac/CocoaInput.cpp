//
// Module: CelestialEngine/Engine/Input/Platform/Mac
// File: CocoaInput.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-17
//

#include "Input/Platforms/Mac/CocoaInput.hpp"

#include "Core/Application.hpp"
#include "Window/Platforms/Mac/MetalCocoaWindow.hpp"


namespace CE::Input {

bool CocoaInput::_IsKeyPressedImpl([[maybe_unused]] KeyCode::KeyboardKeyCode keyCode) {
	if (not _cocoaWindow) {
		_InitCocoaWindow();
	}
	return false;
}

bool CocoaInput::_IsMouseButtonPressedImpl([[maybe_unused]] KeyCode::MouseButtonCode buttonCode) {
	if (not _cocoaWindow) {
		_InitCocoaWindow();
	}
	return false;
}

float CocoaInput::_GetMouseXImpl() {
	if (not _cocoaWindow) {
		_InitCocoaWindow();
	}
	return 0.0f;
}

float CocoaInput::_GetMouseYImpl() {
	if (not _cocoaWindow) {
		_InitCocoaWindow();
	}
	return 0.0f;
}

std::pair<float, float> CocoaInput::_GetMouseXYImpl() {
	if (not _cocoaWindow) {
		_InitCocoaWindow();
	}
	return {0.0f, 0.0f};
}

void CocoaInput::_InitCocoaWindow() {
	if (const auto window = Core::Application::Get().GetWindow()) {
		if (const auto cocoaWindow = dynamic_cast<Window::MetalCocoaWindow*>(window)) {
			_cocoaWindow = cocoaWindow->GetMetalWindow();
		}
	}
}

}
