//
// Module: CelestialEngine/Engine/Modules/Utility/ImGui
// File: ImGui.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#pragma once

#ifndef CE_UTILITY_IMGUI_IMGUI_HPP
#define CE_UTILITY_IMGUI_IMGUI_HPP

#include "Define/Config.hpp"
#include "Define/DynamicLinker.hpp"

#include <imgui.h>

#include <filesystem>
#include <format>


namespace CE::Utility {

#if CE_WINDOW_GLFW
/**
 * @brief Converts a GLFW key code to the corresponding ImGui key code
 * @param key GLFW key code (e.g., GLFW_KEY_A)
 * @return ImGuiKey Corresponding ImGui key code (e.g., ImGuiKey_A), or ImGuiKey_None if no mapping exists
 * @details This function maps GLFW key codes to ImGui key codes to ensure that ImGui receives the correct input events.
 *			It includes mappings for common keys such as navigation keys, editing keys, function keys, alphanumeric keys,
 *			special keys, modifier keys, and keypad keys. If a GLFW key code does not have a corresponding ImGui key code, the function returns ImGuiKey_None.
 */
CE_UTILITY_API ImGuiKey GlfwKeyToImGuiKey(int key);
#endif

/**
 * @brief Points ImGui's settings file at a path outside the application, or turns saving off
 * @param io The ImGui IO of the current context
 * @param iniPath Native path of the settings file; empty turns saving off (IniFilename = nullptr)
 * @details ImGui defaults IniFilename to the relative "imgui.ini", which it writes into the process' working directory.
 *			For a bundled macOS app that directory is `CelestialEngine.app/Contents/MacOS`, so the layout ImGui saves on
 *			exit ends up *inside* the signed bundle: it breaks the code signature seal (making the next build's codesign
 *			fail with unsealed contents in the bundle root) and it is lost on every rebuild. The caller resolves
 *			"config://imgui.ini" (IO::FileSystem) and passes the result here: where the layout goes is the layer's
 *			decision, and this helper stays a thin setter over ImGui with no opinion on mounts.
 *
 *			ImGui stores the path as a bare `const char*` and never copies it, so the resolved string is kept alive in a
 *			static that outlives every context. The first call wins: later calls leave the stored path untouched, so a
 *			pointer handed to a live context can never dangle.
 */
CE_UTILITY_API void SetImGuiIniFile(ImGuiIO& io, const std::filesystem::path& iniPath);

template <typename... Args>
void ImguiText(const std::format_string<Args...> fmt, Args&&... args) {
	std::array<char, 256> buffer{};
	const auto result = std::format_to_n(buffer.data(), buffer.size(), fmt, std::forward<Args>(args)...);
	::ImGui::TextUnformatted(buffer.data(), result.out);
}

}

#endif //CE_UTILITY_IMGUI_IMGUI_HPP
