//
// Module: Tools/Manipulation
// File: String.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-21
// Updated: 2026-02-28
//

#include "Tools/Manipulation/String.hpp"

#include <algorithm>

namespace CE::Tools::Manipulation {

std::string ToLowerCase(const std::string& source) {
	std::string result = source;
	std::ranges::transform(result, result.begin(), [](const unsigned char c) {
		return std::tolower(c);
	});
	return result;
}

}