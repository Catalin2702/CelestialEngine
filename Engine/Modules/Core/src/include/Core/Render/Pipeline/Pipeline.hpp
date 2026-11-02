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
 * @details One include for everything a pipeline state is built from: its descriptor, its interface and the per-backend
 *			states. Backend-specific headers come along too: the Common ones (OpenGL, GLFW) on every platform, the Mac
 *			ones (Metal, Cocoa) only where CE_PLATFORM_MACOS is set - the same split Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_RENDER_PIPELINE_PIPELINE_HPP
#define CE_CORE_RENDER_PIPELINE_PIPELINE_HPP

#include "Define/Config.hpp"

#include "Core/Render/Pipeline/I_PipelineState.hpp"
#include "Core/Render/Pipeline/PipelineDescriptor.hpp"

#include "Core/Render/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.hpp"

#if CE_PLATFORM_MACOS
#include "Core/Render/Pipeline/Platforms/Mac/Metal/MetalPipelineState.hpp"
#endif

#endif //CE_CORE_RENDER_PIPELINE_PIPELINE_HPP
