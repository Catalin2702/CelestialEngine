//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode
// File: MouseButtonCode.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-08
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_KEYCODE_MOUSEKEYS_HPP
#define CE_TYPES_KEYCODE_MOUSEKEYS_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string>


namespace CE::Types {

/**
 * @brief Enumeration of mouse button codes
 * @details This enum defines the various mouse buttons and wheel actions that can be used within the engine.
 *			The enum includes standard buttons (Left, Right, Middle) as well as additional buttons (Button4-Button8) and wheel actions (WheelX, WheelY).
 *			An Unknown value is also provided to represent unrecognized or unsupported mouse button codes.
 */
enum class MouseButtonCode : u8 {
	Left = 0,
	Right = 1,
	Middle = 2,
	Button4 = 3,
	Button5 = 4,
	Button6 = 5,
	Button7 = 6,
	Button8 = 7,

	WheelX = 8,
	WheelY = 9,

	Unknown = 255
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
 * @brief Converts a GLFW mouse button code to a MouseButton enum value
 * @param buttonCode The integer mouse button code from GLFW to convert
 * @return MouseButton The corresponding MouseButton enum value
 * @details This function maps GLFW mouse button codes to the corresponding MouseButton enum values defined in this module.
 *			It allows for easy integration with GLFW's input handling while maintaining a consistent set of mouse button codes within the engine.
 *			The function uses a switch statement to match known GLFW mouse button codes to their MouseButton equivalents.
 *			If the provided buttonCode does not match any known GLFW mouse button code, it returns MouseButton::Unknown.
 */
CE_TYPES_API MouseButtonCode MouseButtonKeyCodeFromGlfw(int buttonCode);

/**
 * @brief Converts a MouseButton enum value to a GLFW mouse button code
 * @param buttonCode The MouseButton enum value to convert
 * @return int The corresponding integer mouse button code for GLFW
 * @details This function maps MouseButton enum values to the corresponding integer codes used by GLFW.
 *			It allows for easy integration with GLFW's input handling when generating events or querying mouse button states.
 *			The function uses a switch statement to match known MouseButton enum values to their GLFW integer equivalents.
 *			If the provided buttonCode does not match any known MouseButton value, it returns -1 to indicate an invalid button code for GLFW.
 */
CE_TYPES_API int GlfwKeyCodeFromMouseButton(MouseButtonCode buttonCode);

/**
 * @brief Converts a MouseButton enum value to an ImGuiKey
 * @param buttonCode The MouseButton enum value to convert
 * @return ImGuiKey The corresponding ImGuiKey for the given MouseButton, or ImGuiKey_None if no mapping exists
 * @details This function maps MouseButton enum values to the corresponding ImGuiKey values used by ImGui for input handling.
 *			This allows for seamless integration with ImGui's input system when using the engine's MouseButton definitions.
 *			The function uses a switch statement to match known MouseButton enum values to their ImGuiKey equivalents.
 *			If the provided buttonCode does not match any known MouseButton value, it returns ImGuiKey_None to indicate that there is no valid mapping for that button.
 */
CE_TYPES_API int ImGuiKeyFromMouseButton(MouseButtonCode buttonCode);

/**
 * @brief Converts NSEvent button numbers to CE::KeyCode::MouseButtonCode
 * @details Maps the NSEvent buttonNumber to the engine's mouse button key codes
 */
CE_TYPES_API MouseButtonCode MouseButtonKeyCodeFromCocoa(long buttonNumber);

/**
 * @brief Converts CE::KeyCode::MouseButtonCode to NSEvent button numbers
 * @details Maps the engine's mouse button key codes to NSEvent buttonNumber values
 */
CE_TYPES_API int CocoaButtonNumberFromMouseButton(MouseButtonCode buttonCode);

/**
 * @brief Names a MouseButtonCode, for fmt/spdlog and - through the formatter below - for std::format
 * @param buttonCode The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const MouseButtonCode buttonCode) {
	switch (buttonCode) {
		case MouseButtonCode::Left: return "Left";
		case MouseButtonCode::Right: return "Right";
		case MouseButtonCode::Middle: return "Middle";
		case MouseButtonCode::Button4: return "Button4";
		case MouseButtonCode::Button5: return "Button5";
		case MouseButtonCode::Button6: return "Button6";
		case MouseButtonCode::Button7: return "Button7";
		case MouseButtonCode::Button8: return "Button8";
		case MouseButtonCode::WheelX: return "WheelX";
		case MouseButtonCode::WheelY: return "WheelY";
		default: return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::Types::MouseButtonCode>: std::formatter<std::string_view> {
	auto format(const CE::Types::MouseButtonCode value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

inline auto operator<=>(const unsigned lhs, const CE::Types::MouseButtonCode rhs) {
	return lhs <=> CE::Types::ToUInt(rhs);
}

#endif //CE_TYPES_KEYCODE_MOUSEKEYS_HPP
