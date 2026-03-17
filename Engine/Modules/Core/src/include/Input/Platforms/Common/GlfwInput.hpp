//
// Module: CelestialEngine/Engine/Modules/Input/Platforms/Common
// File: GlfwInput.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-06
// Updated by: Catalin Chirosca
// Updated: 2026-03-17
//

#pragma once

#ifndef CE_INPUT_COMMON_GLFWINPUT_HPP
#define CE_INPUT_COMMON_GLFWINPUT_HPP

#include "Input/I_Input.hpp"

#include <utility>

struct GLFWwindow;

namespace CE::Input {

/**
 * @class GlfwInput
 * @brief Generic input handling implementation using GLFW
 * @details Implements the I_Input interface.
 *			Uses GLFW functions to query the state of keyboard keys and mouse buttons, as well as to retrieve the mouse cursor position.
 *			Caches a pointer to the GLFW window for efficient input state queries.
 *			This class is instantiated as the singleton instance of I_Input.
 */
class GlfwInput final: public I_Input {
protected:
	[[nodiscard]] bool _IsKeyPressedImpl(KeyCode::KeyboardKeyCode keyCode) override;
	[[nodiscard]] bool _IsMouseButtonPressedImpl(KeyCode::MouseButtonCode buttonCode) override;
	[[nodiscard]] float _GetMouseXImpl() override;
	[[nodiscard]] float _GetMouseYImpl() override;
	[[nodiscard]] std::pair<float, float> _GetMouseXYImpl() override;

private:
	void _InitGLFWWindow();

private:
	GLFWwindow* _glfWwindow = nullptr;				///< Cached GLFW window pointer for input state queries
};

}

#endif //CE_INPUT_COMMON_GLFWINPUT_HPP
