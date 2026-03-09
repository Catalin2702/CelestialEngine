//
// Module: CelestialEngine/Engine/Modules/Input/Platforms/Mac
// File: MetalInput.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-06
// Updated by: Catalin Chirosca
// Updated: 2026-03-09
//

#include "Input/Platforms/Mac/MetalInput.hpp"

#include "Core/Application.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"
#include "Window/I_Window.hpp"

#include <GLFW/glfw3.h>

namespace CE::Input {

I_Input* I_Input::_instance = new MetalInput();

bool MetalInput::_IsKeyPressedImpl(const KeyCode::KeyboardKeyCode keyCode) {
	if (not _glfWwindow) {
		_InitGLFWWindow();
	}
	const auto state = glfwGetKey(_glfWwindow, KeyCode::GlfwKeyCodeFromKeyboard(keyCode));
	return state == GLFW_PRESS or state == GLFW_REPEAT;
}

bool MetalInput::_IsMouseButtonPressedImpl(const KeyCode::MouseButtonCode buttonCode) {
	if (not _glfWwindow) {
		_InitGLFWWindow();
	}
	const auto state = glfwGetMouseButton(_glfWwindow, KeyCode::GlfwKeyCodeFromMouseButton(buttonCode));
	return state == GLFW_PRESS;
}

float MetalInput::_GetMouseXImpl() {
	if (not _glfWwindow) {
		_InitGLFWWindow();
	}
	double mouseX, mouseY;
	glfwGetCursorPos(_glfWwindow, &mouseX, &mouseY);
	return static_cast<float>(mouseX);
}

float MetalInput::_GetMouseYImpl() {
	if (not _glfWwindow) {
		_InitGLFWWindow();
	}
	double mouseX, mouseY;
	glfwGetCursorPos(_glfWwindow, &mouseX, &mouseY);
	return static_cast<float>(mouseY);
}

Types::Vec2f MetalInput::_GetMouseXYImpl() {
	if (not _glfWwindow) {
		_InitGLFWWindow();
	}
	double mouseX, mouseY;
	glfwGetCursorPos(_glfWwindow, &mouseX, &mouseY);
	return {static_cast<float>(mouseX), static_cast<float>(mouseY)};
}

void MetalInput::_InitGLFWWindow() {
	if (const auto window = Core::Application::Get().GetWindow()) {
		_glfWwindow = static_cast<GLFWwindow*>(window->GetNativeWindow());
	}
}

void InitInput() {
	if (not I_Input::_instance) {
		I_Input::_instance = new MetalInput();
	}
}

void ShutdownInput() {
	if (I_Input::_instance) {
		delete I_Input::_instance;
		I_Input::_instance = nullptr;
	}
}

}
