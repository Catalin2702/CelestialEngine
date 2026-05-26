//
// Module: CelestialEngine/Engine/Modules/Input/Platforms/Common/Glfw
// File: GlfwInput.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-06
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#include "Core/Input/Platforms/Common/Glfw/GlfwInput.hpp"

#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Core/Window/I_Window.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <GLFW/glfw3.h>

namespace CE::Core::Input {

bool GlfwInput::_IsKeyPressedImpl(const KeyCode::KeyboardKeyCode keyCode) {
	if (not _window) {
		_InitWindow();
	}
	const auto state = glfwGetKey(_window, KeyCode::GlfwKeyCodeFromKeyboard(keyCode));
	return state == GLFW_PRESS or state == GLFW_REPEAT;
}

bool GlfwInput::_IsMouseButtonPressedImpl(const KeyCode::MouseButtonCode buttonCode) {
	if (not _window) {
		_InitWindow();
	}
	const auto state = glfwGetMouseButton(_window, KeyCode::GlfwKeyCodeFromMouseButton(buttonCode));
	return state == GLFW_PRESS;
}

float GlfwInput::_GetMouseXImpl() {
	if (not _window) {
		_InitWindow();
	}
	double mouseX, mouseY;
	glfwGetCursorPos(_window, &mouseX, &mouseY);
	return static_cast<float>(mouseX);
}

float GlfwInput::_GetMouseYImpl() {
	if (not _window) {
		_InitWindow();
	}
	double mouseX, mouseY;
	glfwGetCursorPos(_window, &mouseX, &mouseY);
	return static_cast<float>(mouseY);
}

std::pair<float, float> GlfwInput::_GetMouseXYImpl() {
	if (not _window) {
		_InitWindow();
	}
	double mouseX, mouseY;
	glfwGetCursorPos(_window, &mouseX, &mouseY);
	return {static_cast<float>(mouseX), static_cast<float>(mouseY)};
}

void GlfwInput::_InitWindow() {
	const auto& window = Application::GlfwApplication::StGet().GetWindow();
	_window = static_cast<GLFWwindow*>(window.GetNativeWindow());
}

}
