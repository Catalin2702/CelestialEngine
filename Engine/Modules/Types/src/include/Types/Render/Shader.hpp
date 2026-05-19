//
// Module: CelestialEngine/Engine/Modules/Types/Shader
// File: Shader.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-05-19
//

#pragma once

#ifndef CE_TYPES_RENDER_SHADER_HPP
#define CE_TYPES_RENDER_SHADER_HPP

#include "Define/DynamicLinker.hpp"

#include <cstdint>

/**
 * @namespace CE::Types::Render
 * @brief Contains type definitions related to rendering, including shader types and graphics APIs
 * @details This namespace encapsulates the various type definitions used in the rendering module of the engine.
 *			It includes enumerations for shader types (e.g., vertex, fragment) and graphics APIs (e.g., OpenGL, Metal).
 *			These types are used throughout the rendering system to ensure consistency and clarity when working with shaders and graphics contexts.
 */
namespace CE::Types::Render {

enum class GraphicsApi: uint8_t;

/**
 * @enum ShaderType
 * @brief Enumeration of shader types
 * @details Defines the types of shaders that can be used in the rendering pipeline, such as vertex and fragment shaders.
 *			These shader types are essential for defining the stages of the graphics pipeline and how vertices and fragments are processed.
 */
enum class ShaderType: uint8_t {
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
 * @param graphicsApi The GraphicsApi for which the shader type is being converted (used for context-specific string formatting if needed)
 * @param shaderType The ShaderType to convert to a string
 * @return const char* A string representation of the ShaderType
 * @details Provides a human-readable string representation of the ShaderType enum value. This is useful for debugging, logging, or displaying shader type names in the user interface.
 *			The function uses a switch statement to map each ShaderType value to its corresponding string name. If the ShaderType does not match any known value, it returns "Unknown".
 */
CE_API const char* ToString(GraphicsApi graphicsApi, ShaderType shaderType);

/**
 * @brief Checks if the specified shader type is supported on the current platform
 * @param type Shader type to check
 * @return bool True if the shader type is supported, false otherwise
 * @details This function checks if the given shader type is supported on the current platform. For example, some platforms may only support vertex and fragment shaders, while others may support additional types like geometry or compute shaders.
 */
CE_API bool IsShaderTypeSupported(ShaderType type);

/**
 * @brief Converts a ShaderType enum value to the corresponding OpenGL shader type constant
 * @param type The ShaderType to convert
 * @return int The OpenGL shader type constant corresponding to the given ShaderType, or -1 if the ShaderType is unknown or unsupported
 * @details This function maps the ShaderType enum values to their corresponding OpenGL shader type constants (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER). This is essential for compiling and using shaders in an OpenGL context. If the provided ShaderType does not have a corresponding OpenGL constant, the function returns -1 to indicate an error.
 */
CE_API int ToOpenGlShaderType(ShaderType type);

}

#endif //CE_TYPES_RENDER_SHADER_HPP
