//
// Module: CelestialEngine/Engine/Modules/Types/FileSystem
// File: File.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_FILESYSTEM_FILE_HPP
#define CE_TYPES_FILESYSTEM_FILE_HPP

#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


namespace CE::Types {

/**
 * @brief Enumeration of the loading states of a file managed by the FileSystem
 * @details NotLoaded: only the path is known; Loaded: the content is in memory; LazyLoading: the content will be
 *			loaded on first access.
 */
enum class FileLoadState: u8 {
	NotLoaded,
	Loaded,
	LazyLoading,
};

/**
 * @brief Names a FileLoadState, for fmt/spdlog and - through the formatter below - for std::format
 * @param fileLoadState The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const FileLoadState fileLoadState) {
	switch (fileLoadState) {
		case FileLoadState::NotLoaded: return "NotLoaded";
		case FileLoadState::Loaded: return "Loaded";
		case FileLoadState::LazyLoading: return "LazyLoading";
		default: return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::Types::FileLoadState>: std::formatter<std::string_view> {
	auto format(const CE::Types::FileLoadState value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_FILESYSTEM_FILE_HPP
