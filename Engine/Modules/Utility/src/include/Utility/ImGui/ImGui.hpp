//
// Module: CelestialEngine/Engine/Modules/Utility/ImGui
// File: ImGui.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_UTILITY_IMGUI_IMGUI_HPP
#define CE_UTILITY_IMGUI_IMGUI_HPP

#include "Define/DynamicLinker.hpp"

#include <imgui.h>

namespace CE::Utility {

/**
 * @brief Converts a GLFW key code to the corresponding ImGui key code
 * @param key GLFW key code (e.g., GLFW_KEY_A)
 * @return ImGuiKey Corresponding ImGui key code (e.g., ImGuiKey_A), or ImGuiKey_None if no mapping exists
 * @details This function maps GLFW key codes to ImGui key codes to ensure that ImGui receives the correct input events.
 *			It includes mappings for common keys such as navigation keys, editing keys, function keys, alphanumeric keys,
 *			special keys, modifier keys, and keypad keys. If a GLFW key code does not have a corresponding ImGui key code, the function returns ImGuiKey_None.
 */
CE_API ImGuiKey GlfwKeyToImGuiKey(int key);

}

#endif //CE_UTILITY_IMGUI_IMGUI_HPP
