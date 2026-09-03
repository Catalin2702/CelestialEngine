//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode
// File: KeyboardKeyCode.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-08
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_KEYCODE_KEYBOARDKEYCODE_HPP
#define CE_TYPES_KEYCODE_KEYBOARDKEYCODE_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string>


enum ImGuiKey: int;

namespace CE::Types {

/**
 * @brief Enumeration of keyboard key codes
 * @details This enum defines the various keyboard keys that can be used within the engine.
 *			Each key is assigned a unique integer value corresponding to its GLFW code, allowing for easy integration with GLFW's input handling.
 *			The enum includes standard keys (A-Z, 0-9), function keys (F1-F12), arrow keys, and special keys (Space, Enter, Escape, etc.).
 *			An Unknown value is also provided to represent unrecognized or unsupported key codes.
 */
enum class KeyboardKeyCode : u16 {
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


/**
 *
 * @param key The KeyboardKeyCode to convert to an integer
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
 * @brief Names a KeyboardKeyCode, for fmt/spdlog and - through the formatter below - for std::format
 * @param key The value to name
 * @return std::string_view The key's name, or "Unknown" for a value outside the enum
 * @details The only one of these switches kept out of line: it maps over a hundred keys, and every translation unit
 *			that touches input includes this header.
 */
CE_TYPES_API std::string_view format_as(KeyboardKeyCode key);

/**
 * @brief Converts a GLFW key code to a KeyCode enum value
 * @param keycode The integer key code from GLFW to convert
 * @return KeyCode The corresponding KeyCode enum value
 * @details Maps GLFW key codes to the corresponding KeyCode enum values defined in this module.
 *			This allows for easy integration with GLFW's input handling while maintaining a consistent set of key codes within the engine.
 *			The function uses a switch statement to match known GLFW key codes to their KeyCode equivalents.
 *			If the provided keycode does not match any known GLFW key code, it returns KeyCode::Unknown.
 */
CE_TYPES_API KeyboardKeyCode KeyboardKeyCodeFromGlfw(int keycode);

/**
 * @brief Converts a KeyCode enum value to a GLFW key code
 * @param keycode The KeyCode enum value to convert
 * @return int The corresponding integer key code for GLFW
 * @details Maps KeyCode enum values to the corresponding integer codes used by GLFW.
 *			This allows for easy integration with GLFW's input handling when generating events or querying key states.
 *			The function uses a switch statement to match known KeyCode enum values to their GLFW integer equivalents.
 *			If the provided keycode does not match any known KeyCode value, it returns -1 to indicate an invalid key code for GLFW.
 */
CE_TYPES_API int GlfwKeyCodeFromKeyboard(KeyboardKeyCode keycode);

/**
 * @brief Converts a KeyCode enum value to an ImGuiKey
 * @param keycode The KeyCode enum value to convert
 * @return ImGuiKey The corresponding ImGuiKey for the given KeyCode, or ImGuiKey_None if no mapping exists
 * @details Maps KeyCode enum values to the corresponding ImGuiKey values used by ImGui for input handling.
 *			This allows for seamless integration with ImGui's input system when using the engine's KeyCode definitions.
 *			The function uses a switch statement to match known KeyCode enum values to their ImGuiKey equivalents.
 *			If the provided keycode does not match any known KeyCode value, it returns ImGuiKey_None to indicate that there is no valid mapping for that key.
 */
CE_TYPES_API ImGuiKey ImGuiKeyFromKeyboard(KeyboardKeyCode keycode);

/**
 * @brief Converts macOS/Cocoa virtual key codes to CE::KeyCode::KeyboardKeyCode
 * @details Maps the NSEvent keyCode values to the engine's keyboard key codes
 */
CE_TYPES_API KeyboardKeyCode KeyboardKeyCodeFromCocoa(unsigned short keyCode);

/**
 * @brief Converts CE::KeyCode::KeyboardKeyCode to macOS/Cocoa virtual key codes
 * @details Maps the engine's keyboard key codes to NSEvent keyCode values
 */
CE_TYPES_API unsigned short CocoaKeyCodeFromKeyboard(KeyboardKeyCode keyCode);

}

template <>
struct std::formatter<CE::Types::KeyboardKeyCode>: std::formatter<std::string_view> {
	auto format(const CE::Types::KeyboardKeyCode value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

inline auto operator<=>(const unsigned lhs, const CE::Types::KeyboardKeyCode rhs) {
	return lhs <=> CE::Types::ToUInt(rhs);
};

#endif //CE_TYPES_KEYCODE_KEYBOARDKEYCODE_HPP
