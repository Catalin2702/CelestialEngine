//
// Module: CelestialEngine/Engine/Types/Build
// File: Build.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-13
// Updated by: Catalin Chirosca
// Updated: 2026-03-13
//

#include "Types/Build/Build.hpp"

#include <string>


namespace CE::Types {

std::string GetBuildTypeString(const BuildType type) {
	switch (type) {
		case BuildType::Debug:
			return "Debug";
		case BuildType::Release:
			return "Release";
		case BuildType::Dist:
			return "Dist";
		default:
			return "Unknown";
	}
}

// NOLINTNEXTLINE
std::string GetCurrentBuildTypeString() {
	return GetBuildTypeString(GetCurrentBuildType());
}

}
