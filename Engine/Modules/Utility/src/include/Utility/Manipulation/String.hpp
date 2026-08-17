//
// Module: CelestialEngine/Engine/Modules/Utility/Manipulation
// File: String.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_UTILITY_MANIPULATION_STRING_HPP
#define CE_UTILITY_MANIPULATION_STRING_HPP

#include "Define/DynamicLinker.hpp"

#include <string>


namespace CE::Utility {

/**
 * @brief Converts a string to lowercase
 * @param source The source string to convert
 * @return std::string A new string with all characters converted to lowercase
 * @details Creates a copy of the input string and converts all uppercase letters
 *			to their lowercase equivalents. Non-alphabetic characters remain unchanged.
 */
CE_UTILITY_API std::string ToLowerCase(const std::string& source);

}

#endif //CE_UTILITY_MANIPULATION_STRING_HPP
