//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode
// File: KeyboardKeyCode.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-08
// Updated by: Catalin Chirosca
// Updated: 2026-03-18
//

#pragma once

#ifndef CE_TYPES_KEYCODE_KEYBOARDKEYCODE_HPP
#define CE_TYPES_KEYCODE_KEYBOARDKEYCODE_HPP

#include "Define/DynamicLinker.hpp"

// ReSharper disable once CppUnusedIncludeDirective
#include <cstdint>
#include <ostream>
#include <string>


enum ImGuiKey: int;

/**
 * @namespace CE::KeyCode
 * @brief Contains definitions and utilities for keyboard key codes
 * @details This namespace encapsulates the KeyboardKeyCode enum and related functions for handling keyboard input within the engine.
 *			It provides a standardized set of key codes that can be used across different modules, ensuring consistency in input handling.
 *			The namespace also includes functions to convert between KeyCode values and their string representations, as well as mappings to and from GLFW key codes for seamless integration with GLFW's input system.
 */
namespace CE::KeyCode {

/**
 * @brief Enumeration of keyboard key codes
 * @details This enum defines the various keyboard keys that can be used within the engine.
 *			Each key is assigned a unique integer value corresponding to its GLFW code, allowing for easy integration with GLFW's input handling.
 *			The enum includes standard keys (A-Z, 0-9), function keys (F1-F12), arrow keys, and special keys (Space, Enter, Escape, etc.).
 *			An Unknown value is also provided to represent unrecognized or unsupported key codes.
 */
enum class CE_API KeyboardKeyCode : uint16_t {
	// Alphabet
	A = 65, B = 66, C = 67, D = 68, E = 69, F = 70,
	G = 71, H = 72, I = 73, J = 74, K = 75, L = 76,
	M = 77, N = 78, O = 79, P = 80, Q = 81, R = 82,
	S = 83, T = 84, U = 85, V = 86, W = 87, X = 88,
	Y = 89, Z = 90,

	// Numbers
	Num0 = 48, Num1 = 49, Num2 = 50, Num3 = 51, Num4 = 52,
	Num5 = 53, Num6 = 54, Num7 = 55, Num8 = 56, Num9 = 57,

	// Function
	F1 = 290, F2 = 291, F3 = 292, F4 = 293, F5 = 294,
	F6 = 295, F7 = 296, F8 = 297, F9 = 298, F10 = 299,
	F11 = 300, F12 = 301,

	// Arrow
	Left = 263, Right = 262, Up = 265, Down = 264,

	// Special
	Space = 32,
	Enter = 257,
	Escape = 256,
	Tab = 258,
	Backspace = 259,
	LeftShift = 340,
	RightShift = 344,
	LeftControl = 341,
	RightControl = 345,
	LeftAlt = 342,
	RightAlt = 346,
	LeftSuper = 343,
	RightSuper = 347,

	Unknown = 0
};

enum class CE_API KeyboardCharsCode: uint32_t {
	A = 65, B = 66, C = 67, D = 68, E = 69, F = 70,
	G = 71, H = 72, I = 73, J = 74, K = 75, L = 76,
	M = 77, N = 78, O = 79, P = 80, Q = 81, R = 82,
	S = 83, T = 84, U = 85, V = 86, W = 87, X = 88,
	Y = 89, Z = 90,

	a = 97, b = 98, c = 99, d = 100, e = 101, f = 102,
	g = 103, h = 104, i = 105, j = 106, k = 107, l = 108,
	m = 109, n = 110, o = 111, p = 112, q = 113, r = 114,
	s = 115, t = 116, u = 117, v = 118, w = 119, x = 120,
	y = 121, z = 122,

	Unknown = 0
};


/**
 *
 * @param key The KeyboardCharsCode to convert to an integer
 * @return int The integer representation of the KeyCode
 * @details Converts the KeyCode enum value to its underlying integer representation.
 *			This is useful for interfacing with APIs that expect key codes as integers (e.g., GLFW).
 *			The function uses static_cast to ensure a safe conversion from the enum class to its underlying type (uint16_t) and then to int for compatibility with typical key code representations.
 */
inline int ToInt(KeyboardKeyCode key) { return static_cast<int>(key); }

/**
 *
 * @param key The KeyCode to convert to an unsigned integer
 * @return unsigned int The unsigned integer representation of the KeyCode
 * @details Converts the KeyCode enum value to its underlying unsigned integer representation.
 *			This can be useful in contexts where key codes are expected as unsigned integers, such as certain input handling systems or when interfacing with APIs that use unsigned types for key codes.
 *			The function uses static_cast to ensure a safe conversion from the enum class to its underlying type (uint16_t) and then to unsigned int for compatibility with such systems.
 */
inline unsigned int ToUInt(KeyboardKeyCode key) { return static_cast<unsigned int>(key); }

/**
 * @brief Converts a KeyCode enum value to its string representation
 * @param key The KeyCode to convert to a string
 * @return const char* A string representation of the KeyCode
 * @details Provides a human-readable string representation of the KeyCode enum value. This is useful for debugging, logging, or displaying key names in the user interface.
 *			The function uses a switch statement to map each KeyCode value to its corresponding string name. If the KeyCode does not match any known value, it returns "Unknown".
 */
const char* ToString(KeyboardKeyCode key);

/**
 * @brief Converts a GLFW key code to a KeyCode enum value
 * @param keycode The integer key code from GLFW to convert
 * @return KeyCode The corresponding KeyCode enum value
 * @details Maps GLFW key codes to the corresponding KeyCode enum values defined in this module.
 *			This allows for easy integration with GLFW's input handling while maintaining a consistent set of key codes within the engine.
 *			The function uses a switch statement to match known GLFW key codes to their KeyCode equivalents.
 *			If the provided keycode does not match any known GLFW key code, it returns KeyCode::Unknown.
 */
KeyboardKeyCode KeyboardKeyCodeFromGlfw(int keycode);

/**
 * @brief Converts a KeyCode enum value to a GLFW key code
 * @param keycode The KeyCode enum value to convert
 * @return int The corresponding integer key code for GLFW
 * @details Maps KeyCode enum values to the corresponding integer codes used by GLFW.
 *			This allows for easy integration with GLFW's input handling when generating events or querying key states.
 *			The function uses a switch statement to match known KeyCode enum values to their GLFW integer equivalents.
 *			If the provided keycode does not match any known KeyCode value, it returns -1 to indicate an invalid key code for GLFW.
 */
int GlfwKeyCodeFromKeyboard(KeyboardKeyCode keycode);

/**
 * @brief Converts a KeyCode enum value to an ImGuiKey
 * @param keycode The KeyCode enum value to convert
 * @return ImGuiKey The corresponding ImGuiKey for the given KeyCode, or ImGuiKey_None if no mapping exists
 * @details Maps KeyCode enum values to the corresponding ImGuiKey values used by ImGui for input handling.
 *			This allows for seamless integration with ImGui's input system when using the engine's KeyCode definitions.
 *			The function uses a switch statement to match known KeyCode enum values to their ImGuiKey equivalents.
 *			If the provided keycode does not match any known KeyCode value, it returns ImGuiKey_None to indicate that there is no valid mapping for that key.
 */
ImGuiKey ImGuiKeyFromKeyboard(KeyboardKeyCode keycode);

/**
 * @brief Converts macOS/Cocoa virtual key codes to CE::KeyCode::KeyboardKeyCode
 * @details Maps the NSEvent keyCode values to the engine's keyboard key codes
 */
KeyboardKeyCode KeyboardKeyCodeFromCocoa(unsigned short keyCode);

/**
 * @brief Converts CE::KeyCode::KeyboardKeyCode to macOS/Cocoa virtual key codes
 * @details Maps the engine's keyboard key codes to NSEvent keyCode values
 */
unsigned short CocoaKeyCodeFromKeyboard(KeyboardKeyCode keycode);

/**
 *  @brief Formats a KeyboardKeyCode enum value as a string
 * @param keycode The KeyboardKeyCode to format as a string
 * @return std::string A string representation of the KeyboardKeyCode
 * @details Provides a convenient way to get a string representation of a KeyboardKeyCode enum value.
 *			This can be useful for logging, debugging, or displaying key names in the user interface.
 *			The function simply calls ToString to get the string representation of the key code and returns it as a std::string
 *			for easier use in C++ contexts where std::string is preferred over const char*.
 */
inline std::string format_as(const KeyboardKeyCode keycode) {
	return ToString(keycode);
}

inline std::ostream& operator<<(std::ostream& os, const KeyboardKeyCode keycode) {
	return os << std::string(ToString(keycode));
}

inline auto operator<=>(const unsigned lhs, const KeyboardKeyCode rhs) {
	return lhs <=> ToUInt(rhs);
};


/**
 *
 * @param key The KeyCode to convert to an integer
 * @return int The integer representation of the KeyboardCharsCode
 * @details Converts the KeyboardCharsCode enum value to its underlying integer representation.
 *			This can be useful for contexts where character codes are expected as integers, such as text input handling or when interfacing with APIs that use integer types for character codes.
 *			The function uses static_cast to ensure a safe conversion from the enum class to its underlying type (uint8_t) and then to int for compatibility with such systems.
 */
inline int ToInt(KeyboardCharsCode key) { return static_cast<int>(key); }

/**
 *
 * @param key The KeyboardCharsCode to convert to an unsigned integer
 * @return unsigned int The unsigned integer representation of the KeyboardCharsCode
 * @details Converts the KeyboardCharsCode enum value to its underlying unsigned integer representation.
 *			This can be useful in contexts where character codes are expected as unsigned integers, such as text input handling or when interfacing with APIs that use unsigned types for character codes.
 *			The function uses static_cast to ensure a safe conversion from the enum class to its underlying type (uint8_t) and then to unsigned int for compatibility with such systems.
 */
inline unsigned int ToUInt(KeyboardCharsCode key) { return static_cast<unsigned int>(key); }

/**
 * @brief Converts a KeyboardCharsCode enum value to its string representation
 * @param key The KeyboardCharsCode to convert to a string
 * @return const char* A string representation of the KeyboardCharsCode
 * @details Provides a human-readable string representation of the KeyboardCharsCode enum value. This is useful for debugging, logging, or displaying character names in the user interface.
 *			The function uses a switch statement to map each KeyboardCharsCode value to its corresponding string name. If the KeyboardCharsCode does not match any known value, it returns "Unknown".
 */
const char* ToString(KeyboardCharsCode key);

/**
 * @brief Converts a GLFW character code to a KeyboardCharsCode enum value
 * @param charCode The unsigned integer character code from GLFW to convert
 * @return KeyboardCharsCode The corresponding KeyboardCharsCode enum value
 * @details Maps GLFW character codes to the corresponding KeyboardCharsCode enum values defined in this module.
 *			This allows for easy integration with GLFW's text input handling while maintaining a consistent set of character codes within the engine.
 *			The function uses a switch statement to match known GLFW character codes to their KeyboardCharsCode equivalents.
 *			If the provided charCode does not match any known GLFW character code, it returns KeyboardCharsCode::Unknown.
 */
KeyboardCharsCode KeyboardCharsCodeFromGlfw(unsigned int charCode);

/**
 * @brief Converts a KeyboardCharsCode enum value to a GLFW character code
 * @param charCode The KeyboardCharsCode enum value to convert
 * @return unsigned int The corresponding unsigned integer character code for GLFW
 * @details Maps KeyboardCharsCode enum values to the corresponding unsigned integer codes used by GLFW for text input.
 *			This allows for easy integration with GLFW's text input handling when generating events or querying character states.
 *			The function uses a switch statement to match known KeyboardCharsCode enum values to their GLFW unsigned integer equivalents.
 *			If the provided charCode does not match any known KeyboardCharsCode value, it returns 0 to indicate an invalid character code for GLFW.
 */
unsigned int GlfwCharCodeFromKeyboard(KeyboardCharsCode charCode);

/**
 * @brief Converts a KeyboardCharsCode enum value to an ImGuiKey
 * @param charCode The KeyboardCharsCode enum value to convert
 * @return unsigned int The corresponding ImGuiKey for the given KeyboardCharsCode, or ImGuiKey_None if no mapping exists
 * @details Maps KeyboardCharsCode enum values to the corresponding ImGuiKey values used by ImGui for input handling.
 *			This allows for seamless integration with ImGui's input system when using the engine's KeyboardCharsCode definitions.
 *			The function uses a switch statement to match known KeyboardCharsCode enum values to their ImGuiKey equivalents.
 *			If the provided charCode does not match any known KeyboardCharsCode value, it returns ImGuiKey_None to indicate that there is no valid mapping for that character code.
 */
unsigned int ImGuiKeyFromKeyboard(KeyboardCharsCode charCode);

inline std::string format_as(const KeyboardCharsCode charCode) {
	return ToString(charCode);
}

inline std::ostream& operator<<(std::ostream& os, const KeyboardCharsCode charCode) {
	return os << std::string(ToString(charCode));
}

inline auto operator<=>(const unsigned lhs, const KeyboardCharsCode rhs) {
	return lhs <=> ToUInt(rhs);
}

}

#endif //CE_TYPES_KEYCODE_KEYBOARDKEYCODE_HPP
