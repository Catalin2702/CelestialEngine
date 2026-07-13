//
// Module: CelestialEngine/Engine/Modules/Core/Input/Platforms/Common/Glfw
// File: GlfwInput.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-06
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Core/Input/Platforms/Common/Glfw/GlfwInput.hpp"

#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Core/Window/I_Window.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <GLFW/glfw3.h>

namespace CE::Core {

bool GlfwInput::_IsKeyPressedImpl(const Types::KeyboardKeyCode keyCode) {
	if (not _window) {
		_InitWindow();
	}
	const auto state = glfwGetKey(_window, Types::GlfwKeyCodeFromKeyboard(keyCode));
	return state == GLFW_PRESS or state == GLFW_REPEAT;
}

bool GlfwInput::_IsMouseButtonPressedImpl(const Types::MouseButtonCode buttonCode) {
	if (not _window) {
		_InitWindow();
	}
	const auto state = glfwGetMouseButton(_window, Types::GlfwKeyCodeFromMouseButton(buttonCode));
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
	const auto& window = GlfwApplication::StGet().GetWindow();
	_window = static_cast<GLFWwindow*>(window.GetNativeWindow());
}

}
