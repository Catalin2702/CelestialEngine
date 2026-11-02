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
 * @details One include for everything a shader is made of: its descriptor, its interface and the per-backend modules
 *			and compilers. Backend-specific headers come along too: the Common ones (OpenGL, GLFW) on every platform,
 *			the Mac ones (Metal, Cocoa) only where CE_PLATFORM_MACOS is set - the same split Types/Render/Render.hpp
 *			makes.
 */

#pragma once

#ifndef CE_CORE_RENDER_SHADER_SHADER_HPP
#define CE_CORE_RENDER_SHADER_SHADER_HPP

#include "Define/Config.hpp"

#include "Core/Render/Shader/I_ShaderModule.hpp"
#include "Core/Render/Shader/ShaderModuleDescriptor.hpp"

#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.hpp"

#if CE_PLATFORM_MACOS
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp"
#endif

#endif //CE_CORE_RENDER_SHADER_SHADER_HPP
