//
// Module: CelestialEngine/Engine/Modules/Types/Build
// File: Build.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-13
// Updated by: Catalin Chirosca
// Updated: 2026-03-13
//

#pragma once

#ifndef CE_TYPES_BUILD_BUILD_HPP
#define CE_TYPES_BUILD_BUILD_HPP

#include <string>

namespace CE::Types {

enum class BuildType {
	Debug,
	Release,
	Dist
};

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

std::string GetBuildTypeString(BuildType type);
std::string GetCurrentBuildTypeString();

}

#endif //CE_TYPES_BUILD_BUILD_HPP
