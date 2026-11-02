//
// Module: CelestialEngine/Engine/Modules/Core/Render/Renderer
// File: Renderer.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Renderer.hpp
 * @brief Umbrella header for the Renderer part of the Core renderer
 * @details One include for the renderer interface, the forward renderer, the draw command it consumes and the per-frame
 *			statistics it reports. Backend-agnostic: the renderer talks to the device and the swapchain through their
 *			interfaces only.
 */

#pragma once

#ifndef CE_CORE_RENDER_RENDERER_RENDERER_HPP
#define CE_CORE_RENDER_RENDERER_RENDERER_HPP

#include "Core/Render/Renderer/I_Renderer.hpp"

#include "Core/Render/Renderer/DrawCommand.hpp"
#include "Core/Render/Renderer/ForwardRenderer.hpp"
#include "Core/Render/Renderer/RenderStats.hpp"

#endif //CE_CORE_RENDER_RENDERER_RENDERER_HPP
