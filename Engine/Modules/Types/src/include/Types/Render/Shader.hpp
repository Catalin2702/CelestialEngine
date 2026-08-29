//
// Module: CelestialEngine/Engine/Modules/Types/Shader
// File: Shader.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_TYPES_RENDER_SHADER_HPP
#define CE_TYPES_RENDER_SHADER_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"


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
 * @brief Converts a ShaderType enum value to its string representation
 * @param shaderType The ShaderType to convert to a string
 * @return const char* A string representation of the ShaderType
 * @details Provides a human-readable string representation of the ShaderType enum value. This is useful for debugging, logging, or displaying shader type names in the user interface.
 *			The function uses a switch statement to map each ShaderType value to its corresponding string name. If the ShaderType does not match any known value, it returns "Unknown".
 */
CE_TYPES_API const char* ToString(ShaderType shaderType);

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
 * @brief Converts a ShaderType enum value to the corresponding OpenGL shader type constant
 * @param type The ShaderType to convert
 * @return int The OpenGL shader type constant corresponding to the given ShaderType, or -1 if the ShaderType is unknown or unsupported
 * @details This function maps the ShaderType enum values to their corresponding OpenGL shader type constants (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER). This is essential for compiling and using shaders in an OpenGL context. If the provided ShaderType does not have a corresponding OpenGL constant, the function returns -1 to indicate an error.
 */
CE_TYPES_API int ToOpenGlShaderType(ShaderType type);

}

#endif //CE_TYPES_RENDER_SHADER_HPP
