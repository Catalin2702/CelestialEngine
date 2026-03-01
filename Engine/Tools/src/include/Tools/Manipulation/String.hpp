//
// Module: Tools/Manipulation
// File: String.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-21
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_TOOLS_MANIPULATION_STRING_HPP
#define CE_TOOLS_MANIPULATION_STRING_HPP

#include <string>

/**
 * @namespace CE::Tools::Manipulation
 * @brief String manipulation utilities
 * @details Provides helper functions for common string operations like case conversion
 */
namespace CE::Tools::Manipulation {

/**
 * @brief Converts a string to lowercase
 * @param source The source string to convert
 * @return std::string A new string with all characters converted to lowercase
 * @details Creates a copy of the input string and converts all uppercase letters
 *          to their lowercase equivalents. Non-alphabetic characters remain unchanged.
 */
std::string ToLowerCase(const std::string& source);

}

#endif //CE_TOOLS_MANIPULATION_STRING_HPP
