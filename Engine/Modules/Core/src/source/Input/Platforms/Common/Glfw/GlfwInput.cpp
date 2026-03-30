//
// Module: CelestialEngine/Engine/Modules/Input/Platforms/Mac
// File: GlfwInput.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-06
// Updated by: Catalin Chirosca
// Updated: 2026-03-18
//

#include "Input/Platforms/Common/Glfw/GlfwInput.hpp"

#include "Core/Application.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"
#include "Window/I_Window.hpp"

#include <GLFW/glfw3.h>

namespace CE::Input {

bool GlfwInput::_IsKeyPressedImpl(const KeyCode::KeyboardKeyCode keyCode) {
	if (not _glfWwindow) {
		_InitGLFWWindow();
	}
	const auto state = glfwGetKey(_glfWwindow, KeyCode::GlfwKeyCodeFromKeyboard(keyCode));
	return state == GLFW_PRESS or state == GLFW_REPEAT;
}

bool GlfwInput::_IsMouseButtonPressedImpl(const KeyCode::MouseButtonCode buttonCode) {
	if (not _glfWwindow) {
		_InitGLFWWindow();
	}
	const auto state = glfwGetMouseButton(_glfWwindow, KeyCode::GlfwKeyCodeFromMouseButton(buttonCode));
	return state == GLFW_PRESS;
}

float GlfwInput::_GetMouseXImpl() {
	if (not _glfWwindow) {
		_InitGLFWWindow();
	}
	double mouseX, mouseY;
	glfwGetCursorPos(_glfWwindow, &mouseX, &mouseY);
	return static_cast<float>(mouseX);
}

float GlfwInput::_GetMouseYImpl() {
	if (not _glfWwindow) {
		_InitGLFWWindow();
	}
	double mouseX, mouseY;
	glfwGetCursorPos(_glfWwindow, &mouseX, &mouseY);
	return static_cast<float>(mouseY);
}

std::pair<float, float> GlfwInput::_GetMouseXYImpl() {
	if (not _glfWwindow) {
		_InitGLFWWindow();
	}
	double mouseX, mouseY;
	glfwGetCursorPos(_glfWwindow, &mouseX, &mouseY);
	return {static_cast<float>(mouseX), static_cast<float>(mouseY)};
}

void GlfwInput::_InitGLFWWindow() {
	if (const auto window = Core::Application::Get().GetWindow()) {
		_glfWwindow = static_cast<GLFWwindow*>(window->GetNativeWindow());
	}
}

}
