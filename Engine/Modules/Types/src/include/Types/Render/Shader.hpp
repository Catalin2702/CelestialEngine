//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: Shader.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#pragma once

#ifndef CE_TYPES_RENDER_SHADER_HPP
#define CE_TYPES_RENDER_SHADER_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


namespace CE::Types {

	enum class GraphicsApi: u8;

/**
 * @enum ShaderType
 * @brief Enumeration of shader types
 * @details Defines the types of shaders that can be used in the rendering pipeline, such as vertex and fragment shaders.
 *			These shader types are essential for defining the stages of the graphics pipeline and how vertices and fragments are processed.
 */
enum class ShaderType: u8 {
	None,
	Vertex,
	Fragment,
	Compute,
	Geometry,
	TessellationControl,
	TessellationEvaluation,
	Mesh,
	Amplification,
	RayGeneration,
	Unknown
};

/**
 * @brief Checks if the specified shader type is supported by the given graphics API
 * @param graphicsApi Graphics API the shader type must run on
 * @param shaderType Shader type to check
 * @return bool True if the shader type is supported, false otherwise
 * @details Each API exposes a different stage set: OpenGL has the geometry and tessellation stages but no mesh or
 *			ray-tracing ones, Metal has mesh/amplification/ray generation but no geometry or tessellation stages.
 */
CE_TYPES_API bool IsShaderTypeSupported(GraphicsApi graphicsApi, ShaderType shaderType);

/**
 * @brief Names a ShaderType, for fmt/spdlog and - through the formatter below - for std::format
 * @param shaderType The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const ShaderType shaderType) {
	switch (shaderType) {
		case ShaderType::None: return "None";
		case ShaderType::Vertex: return "Vertex";
		case ShaderType::Fragment: return "Fragment";
		case ShaderType::Compute: return "Compute";
		case ShaderType::Geometry: return "Geometry";
		case ShaderType::TessellationControl: return "TessellationControl";
		case ShaderType::TessellationEvaluation: return "TessellationEvaluation";
		case ShaderType::Mesh: return "Mesh";
		case ShaderType::Amplification: return "Amplification";
		case ShaderType::RayGeneration: return "RayGeneration";
		case ShaderType::Unknown:
		default: return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::Types::ShaderType>: std::formatter<std::string_view> {
	auto format(const CE::Types::ShaderType value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_RENDER_SHADER_HPP
