//
// Module: Define
// File: Type.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-17
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_DEFINE_TYPE_HPP
#define CE_DEFINE_TYPE_HPP

/**
 * @def BIT(x)
 * @brief Creates a bit mask by shifting 1 left by x positions
 * @param x The bit position (0-based)
 * @details Useful for creating bit flags and masks. For example:
 *          BIT(0) = 0b00000001 = 1
 *          BIT(1) = 0b00000010 = 2
 *          BIT(2) = 0b00000100 = 4
 *          Commonly used for event categories and layer masks.
 * @example
 *          enum EventCategory {
 *              EventCategoryApplication = BIT(0),
 *              EventCategoryInput = BIT(1)
 *          };
 */
#define BIT(x) (1 << x)

#endif //CE_DEFINE_TYPE_HPP
