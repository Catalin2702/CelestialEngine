//
// Module: CelestialEngine/Engine/Modules/Core/Render/Pipeline
// File: Pipeline.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Pipeline.hpp
 * @brief Umbrella header for the Pipeline part of the Core renderer
 * @details One include for everything a pipeline state is built from: its descriptor, its interface and the per-
 *			backend states. Backend-specific headers come along too, each only where its backend is compiled in -
 *			CE_RENDER_* for the graphics ones, CE_WINDOW_* for the window ones, both chosen by CE_RENDER_BACKENDS -
 *			the same split Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_RENDER_PIPELINE_PIPELINE_HPP
#define CE_CORE_RENDER_PIPELINE_PIPELINE_HPP

#include "Define/Config.hpp"

#include "Core/Render/Pipeline/I_PipelineState.hpp"
#include "Core/Render/Pipeline/PipelineDescriptor.hpp"

#if CE_RENDER_OPENGL
#include "Core/Render/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.hpp"
#endif

#if CE_RENDER_METAL
#include "Core/Render/Pipeline/Platforms/Mac/Metal/MetalPipelineState.hpp"
#endif

#endif //CE_CORE_RENDER_PIPELINE_PIPELINE_HPP
