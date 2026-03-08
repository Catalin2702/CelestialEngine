//
// Module: CelestialEngine/Engine/Modules/Input/Platforms/Mac
// File: MetalInput.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-06
// Updated by: Catalin Chirosca
// Updated: 2026-03-08
//

#pragma once

#ifndef CE_INPUT_METALINPUT_HPP
#define CE_INPUT_METALINPUT_HPP

#include "Input/I_Input.hpp"
#include "Types/Container/Vec.hpp"

struct GLFWwindow;

namespace CE::Input {

class MetalInputManager;

/**
 * @class MetalInput
 * @brief macOS-specific input handling implementation using GLFW
 * @details Implements the I_Input interface for macOS platforms. Uses GLFW functions to query the state of keyboard keys and mouse buttons, as well as to retrieve the mouse cursor position. Caches a pointer to the GLFW window for efficient input state queries. This class is instantiated as the singleton instance of I_Input on macOS.
 */
class MetalInput final: public I_Input {
protected:
	[[nodiscard]] bool _IsKeyPressedImpl(int keyCode) override;
	[[nodiscard]] bool _IsMouseButtonPressedImpl(int buttonCode) override;
	[[nodiscard]] float _GetMouseXImpl() override;
	[[nodiscard]] float _GetMouseYImpl() override;
	[[nodiscard]] Types::Vec2f _GetMouseXYImpl() override;

private:
	void _InitGLFWWindow();

private:
	GLFWwindow* _glfWwindow = nullptr;				///< Cached GLFW window pointer for input state queries
};

/**
 * @class MetalInputManager
 * @brief macOS-specific input manager implementation
 * @details Implements the I_InputManager interface for macOS platforms. Responsible for initializing and shutting down the MetalInput instance as the singleton input handler. This class may perform any necessary setup or cleanup related to the input handling system on macOS.
 */
class MetalInputManager final: public I_InputManager {
public:
	/**
	 * @brief Initializes the input handling system
	 * @details Creates an instance of MetalInput and sets it as the singleton instance of I_Input. This allows the static input query methods (e.g., IsKeyPressed) to function correctly by delegating to the MetalInput implementation.
	 */
	void Init() override;
	/**
	 * @brief Shuts down the input handling system
	 * @details Deletes the MetalInput instance and resets the singleton instance pointer in I_Input to nullptr. This ensures that any subsequent calls to input query methods will not access a deleted instance and can log an appropriate error message.
	 */
	void Shutdown() override;
};

}

#endif //CE_INPUT_METALINPUT_HPP
