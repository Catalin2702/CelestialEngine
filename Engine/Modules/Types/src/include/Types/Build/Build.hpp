//
// Module: CelestialEngine/Engine/Modules/Types/Build
// File: Build.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-13
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_BUILD_BUILD_HPP
#define CE_TYPES_BUILD_BUILD_HPP

#include "Define/DynamicLinker.hpp"

#include <format>
#include <string>
#include <string_view>


namespace CE::Types {

/**
 * @brief Enumeration of the engine build configurations
 * @details Mirrors the CMake configurations: Debug (CE_DEBUG), Release (CE_RELEASE) and Dist (CE_DIST).
 */
enum class BuildType {
	None,
	Debug,
	Release,
	Dist,
};

/**
 * @brief Gets the build configuration the engine was compiled with
 * @return BuildType The current build type, resolved at compile time from the CE_DEBUG/CE_RELEASE/CE_DIST macros
 */
// NOLINTNEXTLINE
inline BuildType GetCurrentBuildType() {
	if constexpr (CE_DEBUG)
		return BuildType::Debug;
	if constexpr (CE_RELEASE)
		return BuildType::Release;
	if constexpr (CE_DIST)
		return BuildType::Dist;
	return BuildType::None;
}

/**
 * @brief Gets the human-readable name of the current build configuration
 * @return std::string "Debug", "Release" or "Dist"
 * @details Kept as a function returning an owning string because its callers hand it to ImGui::Text's %s; anywhere
 *			else, format the BuildType directly.
 */
CE_TYPES_API std::string GetCurrentBuildTypeString();

/**
 * @brief Names a BuildType, for fmt/spdlog and - through the formatter below - for std::format
 * @param buildType The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const BuildType buildType) {
	switch (buildType) {
		case BuildType::None: return "None";
		case BuildType::Debug: return "Debug";
		case BuildType::Release: return "Release";
		case BuildType::Dist: return "Dist";
		default: return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::Types::BuildType>: std::formatter<std::string_view> {
	auto format(const CE::Types::BuildType value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_BUILD_BUILD_HPP
