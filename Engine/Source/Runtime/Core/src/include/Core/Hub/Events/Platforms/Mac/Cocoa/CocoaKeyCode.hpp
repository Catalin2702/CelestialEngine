//
// Module: CelestialEngine/Engine/Source/Runtime/Core/Hub/Events/Platforms/Mac/Cocoa
// File: CocoaKeyCode.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

/**
 * @file CocoaKeyCode.hpp
 * @brief Conversions between AppKit's virtual key codes and button numbers and the engine's codes
 * @details Compiled only with the Cocoa window backend (CE_WINDOW_COCOA, derived from CE_RENDER_BACKENDS), the one
 *			caller of these being the Cocoa event hub. AppKit's codes are plain integers, so no AppKit header is needed.
 */

#pragma once

#ifndef CE_CORE_HUB_EVENTS_COCOAKEYCODE_HPP
#define CE_CORE_HUB_EVENTS_COCOAKEYCODE_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"


namespace CE::Core {

/**
 * @brief Converts macOS/Cocoa virtual key codes to CE::KeyCode::KeyboardKeyCode
 * @details Maps the NSEvent keyCode values to the engine's keyboard key codes
 */
CE_CORE_API Types::KeyboardKeyCode KeyboardKeyCodeFromCocoa(unsigned short keyCode);

/**
 * @brief Converts CE::KeyCode::KeyboardKeyCode to macOS/Cocoa virtual key codes
 * @details Maps the engine's keyboard key codes to NSEvent keyCode values
 */
CE_CORE_API unsigned short CocoaKeyCodeFromKeyboard(Types::KeyboardKeyCode keyCode);

/**
 * @brief Converts NSEvent button numbers to CE::KeyCode::MouseButtonCode
 * @details Maps the NSEvent buttonNumber to the engine's mouse button key codes
 */
CE_CORE_API Types::MouseButtonCode MouseButtonKeyCodeFromCocoa(long buttonNumber);

/**
 * @brief Converts CE::KeyCode::MouseButtonCode to NSEvent button numbers
 * @details Maps the engine's mouse button key codes to NSEvent buttonNumber values
 */
CE_CORE_API int CocoaButtonNumberFromMouseButton(Types::MouseButtonCode buttonCode);

}

#endif //CE_CORE_HUB_EVENTS_COCOAKEYCODE_HPP
