//
// Module: Tools/ImGui
// File: ImGui.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-28
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_TOOLS_IMGUI_IMGUI_HPP
#define CE_TOOLS_IMGUI_IMGUI_HPP

#include <imgui.h>

/**
 * @namespace CE::Tools::ImGui
 * @brief Utility functions for ImGui setup and configuration
 * @details Provides helper functions to initialize and configure ImGui with
 *          appropriate styles and settings for the engine.
 */
namespace CE::Tools::ImGui {

/**
 * @brief Converts a GLFW key code to the corresponding ImGui key code
 * @param key GLFW key code (e.g., GLFW_KEY_A)
 * @return ImGuiKey Corresponding ImGui key code (e.g., ImGuiKey_A), or ImGuiKey_None if no mapping exists
 * @details This function maps GLFW key codes to ImGui key codes to ensure that ImGui receives the correct input events.
 *          It includes mappings for common keys such as navigation keys, editing keys, function keys, alphanumeric keys,
 *          special keys, modifier keys, and keypad keys. If a GLFW key code does not have a corresponding ImGui key code, the function returns ImGuiKey_None.
 */
ImGuiKey GlfwKeyToImGuiKey(int key);

}

#endif //CE_TOOLS_IMGUI_IMGUI_HPP
