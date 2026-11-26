//
// Module: CelestialEngine/Engine/Programs/ShaderCompiler
// File: SpirvCompiler.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#pragma once

#ifndef CE_SHADERCOMPILER_SPIRVCOMPILER_HPP
#define CE_SHADERCOMPILER_SPIRVCOMPILER_HPP

#include "ShaderStage.hpp"
#include "Types/Var/Vars.hpp"

#include <expected>
#include <string>
#include <string_view>
#include <vector>


namespace CE::ShaderCompiler {

/// A SPIR-V module: a sequence of 32-bit words, the first of which is the magic number 0x07230203.
using SpirvBinary = std::vector<u32>;


/**
 * @brief Compiles one GLSL shader (Vulkan flavour, #version 450) to SPIR-V with glslang
 * @param source The GLSL text
 * @param sourceName The file it came from, used only to label the diagnostics ("Vertex.vert:12: ...")
 * @param stage The stage the source implements
 * @return SpirvBinary The module, or the compiler's diagnostics when the source does not compile
 * @details The front end of the pipeline: everything a compiler checks - syntax, types, the rules Vulkan adds on top of
 *			GLSL (every resource has a set and a binding) - happens here, once, at build time. The back ends never see
 *			GLSL, only the SPIR-V this returns.
 */
[[nodiscard]] std::expected<SpirvBinary, std::string> CompileToSpirv(std::string_view source, const std::string& sourceName, ShaderStage stage);

}

#endif //CE_SHADERCOMPILER_SPIRVCOMPILER_HPP
