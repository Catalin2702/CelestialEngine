//
// Module: CelestialEngine/Engine/Modules/Core/Layers/ImGui
// File: ImGuiHelpers.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

#pragma once

#ifndef CE_CORE_LAYERS_IMGUI_IMGUIHELPERS_HPP
#define CE_CORE_LAYERS_IMGUI_IMGUIHELPERS_HPP

#include "Define/Config.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <imgui.h>

#include <array>
#include <filesystem>
#include <format>


namespace CE::Core {

/**
 * @brief Converts an engine key code to the ImGuiKey ImGui expects
 * @param keycode The engine key code
 * @return ImGuiKey The matching ImGuiKey, or ImGuiKey_None when ImGui has no such key
 * @details Lives with the rest of the ImGui glue rather than in Types: the key enum belongs to the engine, the mapping to
 *			ImGui, and Types must not depend on ImGui. On macOS Control and Command (Super) are swapped, the way ImGui
 *			expects them there.
 */
CE_CORE_API ImGuiKey ImGuiKeyFromKeyboard(Types::KeyboardKeyCode keycode);

/**
 * @brief Converts an engine mouse button to ImGui's mouse button index (ImGuiMouseButton)
 * @param buttonCode The engine mouse button
 * @return int 0 left, 1 right, 2 middle (the wheel axes fold onto it); 5 when ImGui has no such button
 */
CE_CORE_API int ImGuiKeyFromMouseButton(Types::MouseButtonCode buttonCode);

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
CE_CORE_API void SetImGuiIniFile(ImGuiIO& io, const std::filesystem::path& iniPath);

template <typename... Args>
void ImguiText(const std::format_string<Args...> fmt, Args&&... args) {
	std::array<char, 256> buffer{};
	const auto result = std::format_to_n(buffer.data(), buffer.size(), fmt, std::forward<Args>(args)...);
	::ImGui::TextUnformatted(buffer.data(), result.out);
}

}

#endif //CE_CORE_LAYERS_IMGUI_IMGUIHELPERS_HPP
