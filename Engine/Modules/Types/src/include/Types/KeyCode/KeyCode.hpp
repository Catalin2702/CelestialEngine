//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode
// File: KeyCode.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

/**
 * @file KeyCode.hpp
 * @brief Umbrella header for the KeyCode sub-module of Types
 * @details The two input code enums travel together - an input layer that handles a key press handles a mouse
 *			button press too - so they get one include. Either header still stands on its own.
 */

#pragma once

#ifndef CE_TYPES_KEYCODE_KEYCODE_HPP
#define CE_TYPES_KEYCODE_KEYCODE_HPP

#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#endif //CE_TYPES_KEYCODE_KEYCODE_HPP
