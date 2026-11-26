//
// Module: CelestialEngine/Engine/Source/Programs/ShaderCompiler
// File: ShaderStage.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-26
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#pragma once

#ifndef CE_SHADERCOMPILER_SHADERSTAGE_HPP
#define CE_SHADERCOMPILER_SHADERSTAGE_HPP

#include "Types/Var/Vars.hpp"

#include <filesystem>
#include <optional>
#include <string_view>


namespace CE::ShaderCompiler {

/**
 * @enum ShaderStage
 * @brief The pipeline stages the compiler accepts
 * @details Its own enum rather than Types::ShaderType: this program links no engine module (see its CMakeLists.txt).
 */
enum class ShaderStage: u8 {
	Vertex,
	Fragment
};

/**
 * @brief Reads the stage from a source file's extension, the convention glslang's own tools use
 * @param path The source file, e.g. "Vertex.vert"
 * @return std::optional<ShaderStage> Vertex for ".vert", Fragment for ".frag", nothing for any other extension
 */
[[nodiscard]] std::optional<ShaderStage> StageFromExtension(const std::filesystem::path& path);

/// The stage's name in messages and in the reflection file: "vertex", "fragment".
constexpr std::string_view format_as(const ShaderStage shaderStage) {
	switch (shaderStage) {
		case ShaderStage::Vertex: return "Vertex";
		case ShaderStage::Fragment: return "Fragment";
		default: return "Unknown";
	}
}

}

#endif //CE_SHADERCOMPILER_SHADERSTAGE_HPP
