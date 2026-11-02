//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode
// File: KeyCode.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file KeyCode.hpp
 * @brief Umbrella header for the KeyCode sub-module of Types
 * @details The two input code enums travel together - an input layer that handles a key press handles a mouse
 *			button press too - so they get one include. Either header still stands on its own. The conversions from and
 *			to each window backend's own codes come along where that backend is compiled in (CE_WINDOW_*).
 */

#pragma once

#ifndef CE_TYPES_KEYCODE_KEYCODE_HPP
#define CE_TYPES_KEYCODE_KEYCODE_HPP

#include "Define/Config.hpp"

#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#if CE_WINDOW_GLFW
#include "Types/KeyCode/Platforms/Common/Glfw/Glfw.hpp"
#endif

#if CE_WINDOW_COCOA
#include "Types/KeyCode/Platforms/Mac/Cocoa/Cocoa.hpp"
#endif

#endif //CE_TYPES_KEYCODE_KEYCODE_HPP
