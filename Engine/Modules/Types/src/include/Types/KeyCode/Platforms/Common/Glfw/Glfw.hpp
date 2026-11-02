//
// Module: CelestialEngine/Engine/Modules/Types/KeyCode/Platforms/Common/Glfw
// File: Glfw.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Glfw.hpp
 * @brief Conversions between GLFW's key and mouse-button codes and the engine's
 * @details Compiled only with the GLFW window backend (CE_WINDOW_GLFW, derived from CE_RENDER_BACKENDS), the one
 *			caller of these being the GLFW event hub.
 */

#pragma once

#ifndef CE_TYPES_KEYCODE_GLFW_HPP
#define CE_TYPES_KEYCODE_GLFW_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"


namespace CE::Types {

/**
 * @brief Converts a GLFW key code to a KeyCode enum value
 * @param keycode The integer key code from GLFW to convert
 * @return KeyCode The corresponding KeyCode enum value
 * @details Maps GLFW key codes to the corresponding KeyCode enum values defined in this module.
 *			This allows for easy integration with GLFW's input handling while maintaining a consistent set of key codes within the engine.
 *			The function uses a switch statement to match known GLFW key codes to their KeyCode equivalents.
 *			If the provided keycode does not match any known GLFW key code, it returns KeyCode::Unknown.
 */
CE_TYPES_API KeyboardKeyCode KeyboardKeyCodeFromGlfw(int keycode);

/**
 * @brief Converts a KeyCode enum value to a GLFW key code
 * @param keycode The KeyCode enum value to convert
 * @return int The corresponding integer key code for GLFW
 * @details Maps KeyCode enum values to the corresponding integer codes used by GLFW.
 *			This allows for easy integration with GLFW's input handling when generating events or querying key states.
 *			The function uses a switch statement to match known KeyCode enum values to their GLFW integer equivalents.
 *			If the provided keycode does not match any known KeyCode value, it returns -1 to indicate an invalid key code for GLFW.
 */
CE_TYPES_API int GlfwKeyCodeFromKeyboard(KeyboardKeyCode keycode);

/**
 * @brief Converts a GLFW mouse button code to a MouseButton enum value
 * @param buttonCode The integer mouse button code from GLFW to convert
 * @return MouseButton The corresponding MouseButton enum value
 * @details This function maps GLFW mouse button codes to the corresponding MouseButton enum values defined in this module.
 *			It allows for easy integration with GLFW's input handling while maintaining a consistent set of mouse button codes within the engine.
 *			The function uses a switch statement to match known GLFW mouse button codes to their MouseButton equivalents.
 *			If the provided buttonCode does not match any known GLFW mouse button code, it returns MouseButton::Unknown.
 */
CE_TYPES_API MouseButtonCode MouseButtonKeyCodeFromGlfw(int buttonCode);

/**
 * @brief Converts a MouseButton enum value to a GLFW mouse button code
 * @param buttonCode The MouseButton enum value to convert
 * @return int The corresponding integer mouse button code for GLFW
 * @details This function maps MouseButton enum values to the corresponding integer codes used by GLFW.
 *			It allows for easy integration with GLFW's input handling when generating events or querying mouse button states.
 *			The function uses a switch statement to match known MouseButton enum values to their GLFW integer equivalents.
 *			If the provided buttonCode does not match any known MouseButton value, it returns -1 to indicate an invalid button code for GLFW.
 */
CE_TYPES_API int GlfwKeyCodeFromMouseButton(MouseButtonCode buttonCode);

}

#endif //CE_TYPES_KEYCODE_GLFW_HPP
