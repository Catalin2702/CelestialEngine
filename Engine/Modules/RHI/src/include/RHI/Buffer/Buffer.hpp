//
// Module: CelestialEngine/Engine/Modules/RHI/Buffer
// File: Buffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

/**
 * @file Buffer.hpp
 * @brief Umbrella header for the Buffer part of the Core renderer
 * @details One include for everything a GPU buffer is made of: the interfaces, the vertex layout and the per-
 *			backend buffers and vertex arrays. Backend-specific headers come along too, each only where its backend
 *			is compiled in - CE_RENDER_* for the graphics ones, CE_WINDOW_* for the window ones, both chosen by
 *			CE_RENDER_BACKENDS - the same split Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_RHI_BUFFER_BUFFER_HPP
#define CE_RHI_BUFFER_BUFFER_HPP

#include "Define/Config.hpp"

#include "RHI/Buffer/I_Buffer.hpp"
#include "RHI/Buffer/I_VertexArray.hpp"

#if CE_RENDER_OPENGL
#include "RHI/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp"
#include "RHI/Buffer/Platforms/Common/OpenGl/OpenGlVertexArray.hpp"
#endif

#if CE_RENDER_METAL
#include "RHI/Buffer/Platforms/Mac/Metal/MetalBuffer.hpp"
#endif

#endif //CE_RHI_BUFFER_BUFFER_HPP
