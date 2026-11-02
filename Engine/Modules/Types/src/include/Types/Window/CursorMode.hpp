//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: CursorMode.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#pragma once

#ifndef CE_TYPES_WINDOW_CURSORMODE_HPP
#define CE_TYPES_WINDOW_CURSORMODE_HPP

#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


namespace CE::Types {

/**
 * @enum CursorMode
 * @brief Who the pointer's motion belongs to: the cursor on screen, or whatever reads the motion itself
 * @details The same split Unreal makes between UI input and game input. Normal hands the motion to the cursor, which
 *			moves, clicks the UI and can leave the window. Captured takes it away from the cursor: the cursor is hidden
 *			and held still, and the motion keeps arriving as a virtual position with no screen edge to stop at - what a
 *			mouse-look camera needs, since a real cursor stops turning the camera the moment it hits the border.
 */
enum class CursorMode: u8 {
	Unknown = 0, ///< No mode set: the zero value, so a forgotten initialisation never passes for a real mode
	Normal, ///< Visible and free: the motion moves the cursor
	Captured, ///< Hidden and held still: the motion is reported as an unbounded virtual position
};

/**
 * @brief Names a CursorMode, for fmt/spdlog and - through the formatter below - for std::format
 * @param cursorMode The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const CursorMode cursorMode) {
	switch (cursorMode) {
		case CursorMode::Normal: return "Normal";
		case CursorMode::Captured: return "Captured";
		case CursorMode::Unknown:
		default:
			return "Unknown";
	}
}

}

template<>
struct std::formatter<CE::Types::CursorMode>: std::formatter<std::string_view> {
	auto format(const CE::Types::CursorMode value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_WINDOW_CURSORMODE_HPP
