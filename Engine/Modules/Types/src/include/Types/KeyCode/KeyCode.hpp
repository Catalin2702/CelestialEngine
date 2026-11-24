//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode
// File: KeyCode.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

/**
 * @file KeyCode.hpp
 * @brief Umbrella header for the KeyCode sub-module of Types
 * @details The two input code enums travel together - an input layer that handles a key press handles a mouse
 *			button press too - so they get one include. Either header still stands on its own. The conversions to and from
 *			each window backend's codes are not here: they live in Core, next to the event hub that uses them
 *			(Core/Hub/Events/Platforms/<OS>/<API>/<API>KeyCode.hpp).
 */

#pragma once

#ifndef CE_TYPES_KEYCODE_KEYCODE_HPP
#define CE_TYPES_KEYCODE_KEYCODE_HPP

#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#endif //CE_TYPES_KEYCODE_KEYCODE_HPP
