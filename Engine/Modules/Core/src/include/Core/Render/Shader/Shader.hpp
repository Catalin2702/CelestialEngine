//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader
// File: Shader.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

/**
 * @file Shader.hpp
 * @brief Umbrella header for the Shader part of the Core renderer
 * @details One include for everything a shader is made of: its descriptor, its interface and the per-backend
 *			modules and compilers. Backend-specific headers come along too, each only where its backend is compiled
 *			in - CE_RENDER_* for the graphics ones, CE_WINDOW_* for the window ones, both chosen by
 *			CE_RENDER_BACKENDS - the same split Types/Render/Render.hpp makes.
 */

#pragma once

#ifndef CE_CORE_RENDER_SHADER_SHADER_HPP
#define CE_CORE_RENDER_SHADER_SHADER_HPP

#include "Define/Config.hpp"

#include "Core/Render/Shader/I_ShaderModule.hpp"
#include "Core/Render/Shader/ShaderModuleDescriptor.hpp"

#if CE_RENDER_OPENGL
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.hpp"
#endif

#if CE_RENDER_METAL
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp"
#endif

#endif //CE_CORE_RENDER_SHADER_SHADER_HPP
