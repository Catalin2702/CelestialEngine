//
// Module: CelestialEngine/Engine/Modules/Utility/ImGui
// File: ImGui.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
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
CE_UTILITY_API ImGuiKey GlfwKeyToImGuiKey(int key);

/**
 * @brief Points ImGui's settings file at the per-user configuration directory
 * @param io The ImGui IO of the current context
 * @param fileName Name of the settings file inside the configuration directory
 * @details ImGui defaults IniFilename to the relative "imgui.ini", which it writes into the process' working directory.
 *			For a bundled macOS app that directory is `CelestialEngine.app/Contents/MacOS`, so the layout ImGui saves on
 *			exit ends up *inside* the signed bundle: it breaks the code signature seal (making the next build's codesign
 *			fail with unsealed contents in the bundle root) and it is lost on every rebuild. This redirects it to
 *			FileSystem::GetConfigDirectory(), which is writable, persistent and outside the bundle.
 *
 *			ImGui stores the path as a bare `const char*` and never copies it, so the resolved string is kept alive in a
 *			static that outlives every context. The first call wins: later calls leave the stored path untouched, so a
 *			pointer handed to a live context can never dangle.
 */
CE_UTILITY_API void SetImGuiIniFile(ImGuiIO& io, const char* fileName = "imgui.ini");

}

#endif //CE_UTILITY_IMGUI_IMGUI_HPP
