//
// Module: CelestialEngine/Engine/Modules/Utility/Manipulation
// File: String.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Utility/Manipulation/String.hpp"

#include <algorithm>

namespace CE::Utility {

/**
 * @brief Converts a string to lowercase
 * @param source The source string to convert
 * @return std::string A new lowercase string
 * @details Creates a copy of the input string and transforms all characters
 *			to lowercase using std::tolower. Non-alphabetic characters are unchanged.
 */
std::string ToLowerCase(const std::string& source) {
	std::string result = source;
	std::ranges::transform(result, result.begin(), [](const unsigned char c) {
		return std::tolower(c);
	});
	return result;
}

}