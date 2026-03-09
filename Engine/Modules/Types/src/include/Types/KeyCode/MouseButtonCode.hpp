//
// Module: CelestialEngine/Engine/Modules/Types/KeyboardKeyCode
// File: MouseButtonCode.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-08
// Updated by: Catalin Chirosca
// Updated: 2026-03-09
//

#pragma once

#ifndef CE_TYPES_KEYCODE_MOUSEKEYS_HPP
#define CE_TYPES_KEYCODE_MOUSEKEYS_HPP

#include "Define/DynamicLinker.hpp"

#include <string>


/**
 * @namespace CE::KeyCode
 * @brief Contains definitions and utilities for mouse button codes
 * @details This namespace encapsulates the MouseButtonCode enum and related functions for handling mouse input within the engine.
 *			It provides a standardized set of mouse button codes that can be used across different modules, ensuring consistency in input handling.
 *			The namespace also includes functions to convert between MouseButtonCode values and their string representations, as well as mappings to and from GLFW mouse button codes for seamless integration with GLFW's input system.
 */
namespace CE::KeyCode {

/**
 * @brief Enumeration of mouse button codes
 * @details This enum defines the various mouse buttons and wheel actions that can be used within the engine.
 *			Each button is assigned a unique integer value corresponding to its GLFW code, allowing for easy integration with GLFW's input handling.
 *			The enum includes standard buttons (Left, Right, Middle) as well as additional buttons (Button4-Button8) and wheel actions (WheelX, WheelY).
 *			An Unknown value is also provided to represent unrecognized or unsupported mouse button codes.
 */
enum class CE_API MouseButtonCode : uint8_t {
	Left = 1,
	Right = 2,
	Middle = 3,
	Button4 = 4,
	Button5 = 5,
	Button6 = 6,
	Button7 = 7,
	Button8 = 8,

	WheelX = 9,
	WheelY = 10,

	Unknown = 0
};

/**
 * @brief Converts a MouseButton enum value to its underlying integer representation
 * @param buttonCode The MouseButton enum value to convert
 * @return int The integer representation of the MouseButton
 * @details This function uses static_cast to convert the MouseButton enum to an int. It allows
 *			for easy interoperability with APIs that expect mouse button codes as integers (e.g., GLFW).
 */
inline int ToInt(const MouseButtonCode buttonCode) { return static_cast<int>(buttonCode); }
/**
 * @brief Converts a MouseButton enum value to its underlying unsigned integer representation
 * @param buttonCode The MouseButton enum value to convert
 * @return unsigned int The unsigned integer representation of the MouseButton
 * @details This function uses static_cast to convert the MouseButton enum to an unsigned int.
 *			It can be useful in contexts where mouse button codes are expected as unsigned integers, such as certain input handling systems or when interfacing with APIs that use unsigned types for button codes.
 */
inline unsigned int ToUInt(const MouseButtonCode buttonCode) { return static_cast<unsigned int>(buttonCode); }

/**
 * @brief Converts a MouseButton enum value to a human-readable string
 * @param buttonCode The MouseButton enum value to convert
 * @return const char* A string representation of the MouseButton
 * @details This function provides a way to get a human-readable name for each MouseButton enum value.
 *			It uses a switch statement to return the corresponding string for each defined button.
 *			If the button value is not recognized, it returns "Unknown".
 */
const char* ToString(MouseButtonCode buttonCode);

/**
 * @brief Converts a GLFW mouse button code to a MouseButton enum value
 * @param buttonCode The integer mouse button code from GLFW to convert
 * @return MouseButton The corresponding MouseButton enum value
 * @details This function maps GLFW mouse button codes to the corresponding MouseButton enum values defined in this module.
 *			It allows for easy integration with GLFW's input handling while maintaining a consistent set of mouse button codes within the engine.
 *			The function uses a switch statement to match known GLFW mouse button codes to their MouseButton equivalents.
 *			If the provided buttonCode does not match any known GLFW mouse button code, it returns MouseButton::Unknown.
 */
MouseButtonCode MouseButtonKeyCodeFromGlfw(int buttonCode);

/**
 * @brief Converts a MouseButton enum value to a GLFW mouse button code
 * @param buttonCode The MouseButton enum value to convert
 * @return int The corresponding integer mouse button code for GLFW
 * @details This function maps MouseButton enum values to the corresponding integer codes used by GLFW.
 *			It allows for easy integration with GLFW's input handling when generating events or querying mouse button states.
 *			The function uses a switch statement to match known MouseButton enum values to their GLFW integer equivalents.
 *			If the provided buttonCode does not match any known MouseButton value, it returns -1 to indicate an invalid button code for GLFW.
 */
int GlfwKeyCodeFromMouseButton(MouseButtonCode buttonCode);

/**
 * @brief Converts a MouseButton enum value to an ImGuiKey
 * @param buttonCode The MouseButton enum value to convert
 * @return ImGuiKey The corresponding ImGuiKey for the given MouseButton, or ImGuiKey_None if no mapping exists
 * @details This function maps MouseButton enum values to the corresponding ImGuiKey values used by ImGui for input handling.
 *			This allows for seamless integration with ImGui's input system when using the engine's MouseButton definitions.
 *			The function uses a switch statement to match known MouseButton enum values to their ImGuiKey equivalents.
 *			If the provided buttonCode does not match any known MouseButton value, it returns ImGuiKey_None to indicate that there is no valid mapping for that button.
 */
int ImGuiKeyFromMouseButton(MouseButtonCode buttonCode);

inline std::ostream& operator<<(std::ostream& os, const MouseButtonCode buttonCode) {
	return os << std::string(ToString(buttonCode));
}

inline std::string format_as(const MouseButtonCode buttonCode) {
	return ToString(buttonCode);
}

}

#endif //CE_TYPES_KEYCODE_MOUSEKEYS_HPP
