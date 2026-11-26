//
// Module: CelestialEngine/Engine/Types/Build
// File: Build.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-13
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include "Types/Build/Build.hpp"

#include <string>


namespace CE::Types {

// NOLINTNEXTLINE
std::string GetCurrentBuildTypeString() {
	return std::string(format_as(GetCurrentBuildType()));
}

}
