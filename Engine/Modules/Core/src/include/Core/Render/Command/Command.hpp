//
// Module: CelestialEngine/Engine/Modules/Core/Render/Command
// File: Command.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Command.hpp
 * @brief Umbrella header for the Command part of the Core renderer
 * @details One include for everything a frame is recorded with: render pass, viewport, the encoder interface and
 *			its per- backend encoders. Backend-specific headers come along too, each only where its backend is
 *			compiled in - CE_RENDER_* for the graphics ones, CE_WINDOW_* for the window ones, both chosen by
 *			CE_RENDER_BACKENDS - the same split Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_RENDER_COMMAND_COMMAND_HPP
#define CE_CORE_RENDER_COMMAND_COMMAND_HPP

#include "Define/Config.hpp"

#include "Core/Render/Command/I_CommandEncoder.hpp"
#include "Core/Render/Command/RenderPassDescriptor.hpp"
#include "Core/Render/Command/Viewport.hpp"

#if CE_RENDER_OPENGL
#include "Core/Render/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.hpp"
#endif

#if CE_RENDER_METAL
#include "Core/Render/Command/Platforms/Mac/Metal/MetalCommandEncoder.hpp"
#endif

#endif //CE_CORE_RENDER_COMMAND_COMMAND_HPP
