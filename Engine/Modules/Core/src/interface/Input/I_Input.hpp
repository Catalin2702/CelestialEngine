//
// Module: CelestialEngine/Engine/Modules/Input
// File: I_Input.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-04
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#pragma once

#ifndef CE_INPUT_I_INPUT_HPP
#define CE_INPUT_I_INPUT_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"
#include "Types/Window/WindowProps.hpp"

#include <utility>


/**
 * @namespace CE::Core::Input
 * @brief Input handling module for processing user input events
 * @details
 */
namespace CE::Core::Input {

/**
 * @class I_Input
 * @brief Interface for input handling
 * @details Provides a platform-agnostic interface for querying input states such as keyboard keys and mouse buttons. This class is designed to be inherited by platform-specific implementations (e.g., MetalInput for macOS). It uses the singleton pattern to allow static access to input queries throughout the application without needing to pass around an instance.
 */
class CE_API I_Input {
	friend void InitInput(Types::Window::WindowApi windowApi);
	friend void ShutdownInput();
public:
	/**
	 * @brief Virtual destructor
	 * @details Ensures proper cleanup of derived input handling classes
	 */
	virtual ~I_Input() = default;

public:
	/**
	 * @brief Checks if a specific key is currently pressed
	 * @param keyCode The keycode of the key to check
	 * @return bool True if the key is pressed, false otherwise
	 * @details Queries the current state of the specified key. This method is static and can be called
	 *			from anywhere in the application without needing an instance of I_Input. It relies on
	 *			the singleton instance to perform the actual check. If the instance is not initialized,
	 *			it crashes.
	 */
	static bool IsKeyPressed(KeyCode::KeyboardKeyCode keyCode);
	/**
	 * @brief Checks if a specific mouse button is currently pressed
	 * @param buttonCode The button code of the mouse button to check
	 * @return bool True if the mouse button is pressed, false otherwise
	 * @details Queries the current state of the specified mouse button. Similar to IsKeyPressed, this
	*			method is static and relies on the singleton instance to perform the check. If the instance is not initialized,
	 *			it crashes.
	 */
	static bool IsMouseButtonPressed(KeyCode::MouseButtonCode buttonCode);
	/**
	 * @brief Gets the current X position of the mouse cursor
	 * @return float The X coordinate of the mouse cursor
	 * @details Retrieves the current X position of the mouse cursor. This method is static and relies
	*			on the singleton instance to perform the query. If the instance is not initialized,
	 *			it crashes.
	 */
	static float GetMouseX();
	/**
	 * @brief Gets the current Y position of the mouse cursor
	 * @return float The Y coordinate of the mouse cursor
	 * @details Retrieves the current Y position of the mouse cursor. Similar to GetMouseX, this method
	*			is static and relies on the singleton instance. If the instance is not initialized,
	 *			it crashes.
	 */
	static float GetMouseY();
	/**
	 * @brief Gets the current X and Y position of the mouse cursor as a vector
	 * @return std::pair<float, float> A vector containing the X and Y coordinates of the mouse cursor
	 * @details Retrieves both the X and Y positions of the mouse cursor in a single call. This method
	*			is static and relies on the singleton instance to perform the query. If the instance is not initialized,
	 *			it crashes.
	 */
	static std::pair<float, float> GetMouseXY();

public:
	/**
	 * @brief Gets the singleton instance of the Input class
	 * @return Input* Pointer to the Input instance
	 * @details Provides access to the global Input instance for handling input events throughout the application
	 */
	static I_Input* Get();

protected:
	/**
	 * @brief Implementation of key press check
	 * @param keyCode The keycode of the key to check
	 * @return bool True if the key is pressed, false otherwise
	 * @details Pure virtual method that must be implemented by derived classes to provide platform-specific key press checking logic.
	 */
	[[nodiscard]] virtual bool _IsKeyPressedImpl(KeyCode::KeyboardKeyCode keyCode) = 0;
	/**
	 * @brief Implementation of mouse button press check
	 * @param buttonCode The button code of the mouse button to check
	 * @return bool True if the mouse button is pressed, false otherwise
	 * @details Pure virtual method that must be implemented by derived classes to provide platform-specific mouse button press checking logic.
	 */
	[[nodiscard]] virtual bool _IsMouseButtonPressedImpl(KeyCode::MouseButtonCode buttonCode) = 0;
	/**
	 * @brief Implementation of mouse X position retrieval
	 * @return float The X coordinate of the mouse cursor
	 * @details Pure virtual method that must be implemented by derived classes to provide platform-specific logic for retrieving the mouse cursor's X position.
	 */
	[[nodiscard]] virtual float _GetMouseXImpl() = 0;
	/**
	 * @brief Implementation of mouse Y position retrieval
	 * @return float The Y coordinate of the mouse cursor
	 * @details Pure virtual method that must be implemented by derived classes to provide platform-specific logic for retrieving the mouse cursor's Y position.
	 */
	[[nodiscard]] virtual float _GetMouseYImpl() = 0;
	/**
	 * @brief Implementation of mouse XY position retrieval
	 * @return Types::Vec2f A vector containing the X and Y coordinates of the mouse cursor
	 * @details Pure virtual method that must be implemented by derived classes to provide platform-specific logic for retrieving both the X and Y positions of the mouse cursor in a single call.
	 */
	[[nodiscard]] virtual std::pair<float, float> _GetMouseXYImpl() = 0;

private:
	static I_Input* _instance;						///< Singleton instance of the I_Input class
};

void InitInput(Types::Window::WindowApi windowApi);
void ShutdownInput();

}

#endif //CE_INPUT_I_INPUT_HPP
