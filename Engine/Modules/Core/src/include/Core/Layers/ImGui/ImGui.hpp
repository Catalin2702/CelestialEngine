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
 *			implementations. Backend-specific headers come along too, each only where its backend is compiled in -
 *			CE_RENDER_* for the graphics ones, CE_WINDOW_* for the window ones, both chosen by CE_RENDER_BACKENDS -
 *			the same split Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_LAYERS_IMGUI_IMGUI_HPP
#define CE_CORE_LAYERS_IMGUI_IMGUI_HPP

#include "Define/Config.hpp"

#include "Core/Layers/ImGui/I_ImGuiLayer.hpp"

#if CE_RENDER_OPENGL
#include "Core/Layers/ImGui/Platforms/Common/OpenGl/ImGuiOpenGlLayer.hpp"
#endif

#if CE_RENDER_METAL
#include "Core/Layers/ImGui/Platforms/Mac/Metal/ImGuiMetalLayer.hpp"
#endif

#endif //CE_CORE_LAYERS_IMGUI_IMGUI_HPP
