//
// Module: CelestialEngine/Engine/Modules/Utility/Manipulation
// File: String.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#include "Utility/Manipulation/String.hpp"

#include <algorithm>

namespace CE::Utility {

std::string ToLowerCase(const std::string& source) {
	std::string result = source;
	std::ranges::transform(result, result.begin(), [](const unsigned char c) {
		return std::tolower(c);
	});
	return result;
}

}