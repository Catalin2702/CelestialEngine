//
// Module: CelestialEngine/Engine/Input/
// File: InputImpl.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-18
//

#include "Input/I_Input.hpp"

#include "Types/Window/WindowProps.hpp"

#include "Input/Platforms/Common/GlfwInput.hpp"

#ifdef CE_PLATFORM_MACOS
#include "Input/Platforms/Mac/CocoaInput.hpp"
#endif

namespace CE::Input {

I_Input* I_Input::_instance = nullptr;

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