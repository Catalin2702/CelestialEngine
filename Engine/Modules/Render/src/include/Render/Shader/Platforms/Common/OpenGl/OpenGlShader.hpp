//
// Module: CelestialEngine/Engine/Modules/Render/Shader
// File: OpenGlShader.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-05-07
//

#pragma once

#ifndef CE_RENDER_SHADER_OPENGLSHADER_HPP
#define CE_RENDER_SHADER_OPENGLSHADER_HPP

#include "Define/DynamicLinker.hpp"
#include "Render/Shader/I_Shader.hpp"

#include <cstdint>

namespace CE::Types::Render {
	enum class ShaderType: uint32_t;
}

namespace CE::Render::Shader {

/**
 * @class OpenGlShader
 * @brief OpenGL-specific shader implementation
 * @details Provides an OpenGL-specific implementation of the I_Shader interface for managing individual shaders (vertex, fragment, etc.) in an OpenGL context.
 *			Handles shader compilation, resource management, and provides access to the underlying OpenGL shader ID for use in shader programs.
 */
class CE_API OpenGlShader: public I_Shader {
public:
	/**
	 * @brief Constructs an OpenGlShader from shader source code
	 * @param shader Pointer to the shader source code as a null-terminated string
	 * @param type Type of the shader (e.g., vertex, fragment) specified by the ShaderType enum
	 * @details Compiles the provided shader source code and creates an OpenGL shader object. The shader is compiled based on the specified type, and any compilation errors are logged and result in an exception being thrown.
	 */
	OpenGlShader(const char* shader, Types::Render::ShaderType type);

	/**
	 * @brief Copy constructor for OpenGlShader
	 * @param other Reference to another OpenGlShader object to copy from
	 * @details Creates a new OpenGlShader object by copying the shader ID, program ID, and shader type from another OpenGlShader instance. This allows for the creation of a new shader object that shares the same underlying OpenGL resources as the original.
	 */
	OpenGlShader(const OpenGlShader& other);

	/**
	 * @brief Destructor for OpenGlShader
	 * @details Cleans up the OpenGL shader resources by detaching the shader from its program, deleting the shader object, and deleting the shader program. This ensures that all associated OpenGL resources are properly released when the OpenGlShader object is destroyed.
	 */
	~OpenGlShader() override;

public:
	[[nodiscard]] uint32_t GetShaderId() const override;
	[[nodiscard]] Types::Render::ShaderType GetType() const override;

private:
	uint32_t _shaderId;
	Types::Render::ShaderType _type;
};

}

#endif //CE_RENDER_SHADER_OPENGLSHADER_HPP
