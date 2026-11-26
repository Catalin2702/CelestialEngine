//
// Module: CelestialEngine/Engine/Source/Programs/ShaderCompiler
// File: Reflection.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#pragma once

#ifndef CE_SHADERCOMPILER_REFLECTION_HPP
#define CE_SHADERCOMPILER_REFLECTION_HPP

#include "ShaderStage.hpp"
#include "SpirvCompiler.hpp"

#include <expected>
#include <string>


namespace CE::ShaderCompiler {

/**
 * @brief Describes what a shader reads and writes, as one line per item of a small text format
 * @param spirv The module CompileToSpirv produced
 * @param stage Its stage
 * @return std::string The reflection text, or SPIRV-Cross's error message
 * @details The lines, one per input, output and resource, each a kind followed by key=value pairs:
 *			  stage vertex
 *			  input location=0 name=inputPosition type=float3
 *			  output location=0 name=color type=float4
 *			  uniform_buffer name=CameraConstants set=0 binding=1 size=64
 *			  sampled_image name=sceneColor set=0 binding=0
 *			Read from SPIR-V, so it is what the compiled shader really declares, not what a comment says it does. Plain
 *			text so that tests (and people) read it without a parser: the engine's tests compare it with the slots and
 *			layouts the C++ side uses.
 */
[[nodiscard]] std::expected<std::string, std::string> Reflect(const SpirvBinary& spirv, ShaderStage stage);

}

#endif //CE_SHADERCOMPILER_REFLECTION_HPP
