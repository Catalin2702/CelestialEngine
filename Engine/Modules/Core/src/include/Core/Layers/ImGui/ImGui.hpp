//
// Module: CelestialEngine/Engine/Modules/Core/Layers/ImGui
// File: ImGui.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file ImGui.hpp
 * @brief Umbrella header for the ImGui layers of Core
 * @details One include for everything the ImGui overlay is built from: the layer interface and its per-backend
 *			implementations. Backend-specific headers come along too: the Common ones (OpenGL, GLFW) on every platform,
 *			the Mac ones (Metal, Cocoa) only where CE_PLATFORM_MACOS is set - the same split Types/Render/Render.hpp
 *			makes.
 */

#pragma once

#ifndef CE_CORE_LAYERS_IMGUI_IMGUI_HPP
#define CE_CORE_LAYERS_IMGUI_IMGUI_HPP

#include "Define/Config.hpp"

#include "Core/Layers/ImGui/I_ImGuiLayer.hpp"

#include "Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp"

#if CE_PLATFORM_MACOS
#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"
#endif

#endif //CE_CORE_LAYERS_IMGUI_IMGUI_HPP
