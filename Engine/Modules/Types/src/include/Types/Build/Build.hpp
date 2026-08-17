//
// Module: CelestialEngine/Engine/Modules/Types/Build
// File: Build.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_TYPES_BUILD_BUILD_HPP
#define CE_TYPES_BUILD_BUILD_HPP

#include "Define/DynamicLinker.hpp"

#include <string>


namespace CE::Types {

/**
 * @brief Enumeration of the engine build configurations
 * @details Mirrors the CMake configurations: Debug (CE_DEBUG), Release (CE_RELEASE) and Dist (CE_DIST).
 */
enum class BuildType {
	Debug,
	Release,
	Dist
};

/**
 * @brief Gets the build configuration the engine was compiled with
 * @return BuildType The current build type, resolved at compile time from the CE_DEBUG/CE_RELEASE/CE_DIST macros
 */
// NOLINTNEXTLINE
inline BuildType GetCurrentBuildType() {
#if defined(CE_DEBUG)
	return BuildType::Debug;
#elif defined(CE_RELEASE)
	return BuildType::Release;
#elif defined(CE_DIST)
	return BuildType::Dist;
#else
	#error "Unknown build type! Define CE_DEBUG, CE_RELEASE, or CE_DIST."
#endif

}

/**
 * @brief Converts a BuildType to its human-readable name
 * @param type The build type to convert
 * @return std::string "Debug", "Release" or "Dist"
 */
CE_TYPES_API std::string GetBuildTypeString(BuildType type);

/**
 * @brief Gets the human-readable name of the current build configuration
 * @return std::string "Debug", "Release" or "Dist"
 */
CE_TYPES_API std::string GetCurrentBuildTypeString();

}

#endif //CE_TYPES_BUILD_BUILD_HPP
