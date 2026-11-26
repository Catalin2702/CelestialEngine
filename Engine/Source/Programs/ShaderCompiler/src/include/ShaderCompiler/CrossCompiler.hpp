//
// Module: CelestialEngine/Engine/Source/Programs/ShaderCompiler
// File: CrossCompiler.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#pragma once

#ifndef CE_SHADERCOMPILER_CROSSCOMPILER_HPP
#define CE_SHADERCOMPILER_CROSSCOMPILER_HPP

#include "ShaderStage.hpp"
#include "SpirvCompiler.hpp"

#include <expected>
#include <string>


namespace CE::ShaderCompiler {

/**
 * @brief Writes a SPIR-V module back out as Metal Shading Language, with SPIRV-Cross
 * @param spirv The module CompileToSpirv produced
 * @param stage Its stage
 * @param entryPoint The name the MSL function gets, e.g. "vertexMain": what MetalShaderModule looks the shader up by
 * @return std::string The MSL source, or SPIRV-Cross's error message
 * @details Every resource keeps its binding number as its Metal index: `binding = 1` becomes `[[buffer(1)]]` for a
 *			uniform buffer, `[[texture(n)]]` / `[[sampler(n)]]` for a sampled image. That is what makes the engine's
 *			slot constants (Types/Render/ShaderConstants.hpp) mean the same thing in GLSL and in Metal.
 */
[[nodiscard]] std::expected<std::string, std::string> ToMsl(const SpirvBinary& spirv, ShaderStage stage, const std::string& entryPoint);

/**
 * @brief Writes a SPIR-V module back out as GLSL 4.10, the newest the OpenGL of macOS accepts, with SPIRV-Cross
 * @param spirv The module CompileToSpirv produced
 * @return std::string The GLSL source, or SPIRV-Cross's error message
 * @details 4.10 has no `layout(binding = n)` for uniform blocks and samplers (4.20 brought it), so the bindings are
 *			dropped and the names survive: OpenGL binds those resources by name at run time (OpenGlPipelineState,
 *			OpenGlCommandEncoder). The pixel origin is left to OpenGL, the lower-left corner, although the SPIR-V says
 *			upper-left: gl_FragCoord.xy is the pixel of the framebuffer each backend draws into, in its own convention.
 */
[[nodiscard]] std::expected<std::string, std::string> ToGlsl(const SpirvBinary& spirv);

}

#endif //CE_SHADERCOMPILER_CROSSCOMPILER_HPP
