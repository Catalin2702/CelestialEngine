//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode/Platforms/Mac/Cocoa
// File: Cocoa.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Cocoa.hpp
 * @brief Conversions between AppKit's virtual key codes and button numbers and the engine's codes
 * @details Compiled only with the Cocoa window backend (CE_WINDOW_COCOA, derived from CE_RENDER_BACKENDS), the one
 *			caller of these being the Cocoa event hub. AppKit's codes are plain integers, so no AppKit header is needed.
 */

#pragma once

#ifndef CE_TYPES_KEYCODE_COCOA_HPP
#define CE_TYPES_KEYCODE_COCOA_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"


namespace CE::Types {

/**
 * @brief Converts macOS/Cocoa virtual key codes to CE::KeyCode::KeyboardKeyCode
 * @details Maps the NSEvent keyCode values to the engine's keyboard key codes
 */
CE_TYPES_API KeyboardKeyCode KeyboardKeyCodeFromCocoa(unsigned short keyCode);

/**
 * @brief Converts CE::KeyCode::KeyboardKeyCode to macOS/Cocoa virtual key codes
 * @details Maps the engine's keyboard key codes to NSEvent keyCode values
 */
CE_TYPES_API unsigned short CocoaKeyCodeFromKeyboard(KeyboardKeyCode keyCode);

/**
 * @brief Converts NSEvent button numbers to CE::KeyCode::MouseButtonCode
 * @details Maps the NSEvent buttonNumber to the engine's mouse button key codes
 */
CE_TYPES_API MouseButtonCode MouseButtonKeyCodeFromCocoa(long buttonNumber);

/**
 * @brief Converts CE::KeyCode::MouseButtonCode to NSEvent button numbers
 * @details Maps the engine's mouse button key codes to NSEvent buttonNumber values
 */
CE_TYPES_API int CocoaButtonNumberFromMouseButton(MouseButtonCode buttonCode);

}

#endif //CE_TYPES_KEYCODE_COCOA_HPP
