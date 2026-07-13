//
// Module: CelestialEngine/Engine/Modules/Core/Input/Platforms/Common/Glfw
// File: GlfwInput.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-06
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_CORE_INPUT_COMMON_GLFWINPUT_HPP
#define CE_CORE_INPUT_COMMON_GLFWINPUT_HPP

#include "Core/Input/I_Input.hpp"

#include "Define/DynamicLinker.hpp"

#include <utility>

struct GLFWwindow;

namespace CE::Core {

/**
 * @class GlfwInput
 * @brief Generic input handling implementation using GLFW
 * @details Implements the I_Input interface.
 *			Uses GLFW functions to query the state of keyboard keys and mouse buttons, as well as to retrieve the mouse cursor position.
 *			Caches a pointer to the GLFW window for efficient input state queries.
 *			This class is instantiated as the singleton instance of I_Input.
 */
class CE_API GlfwInput final: public I_Input {
protected:
	[[nodiscard]] bool _IsKeyPressedImpl(Types::KeyboardKeyCode keyCode) override;
	[[nodiscard]] bool _IsMouseButtonPressedImpl(Types::MouseButtonCode buttonCode) override;
	[[nodiscard]] float _GetMouseXImpl() override;
	[[nodiscard]] float _GetMouseYImpl() override;
	[[nodiscard]] std::pair<float, float> _GetMouseXYImpl() override;

private:
	void _InitWindow();

private:
	GLFWwindow* _window = nullptr;				///< Cached GLFW window pointer for input state queries
};

}

#endif //CE_CORE_INPUT_COMMON_GLFWINPUT_HPP
