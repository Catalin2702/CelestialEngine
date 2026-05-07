//
// Module: CelestialEngine/Engine/Modules/Render/Shader
// File: I_ShaderProgram.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-05-07
//

#pragma once

#ifndef CE_RENDER_SHADER_I_SHADERPROGRAM_HPP
#define CE_RENDER_SHADER_I_SHADERPROGRAM_HPP

#include <cstdint>

/**
 * @namespace CE::Render::Shader
 * @brief Shader management and interfaces
 * @details Contains the I_ShaderProgram interface and related type definitions for shader program management.
 *			This namespace defines the common interface for all shader program implementations,
 *			allowing for platform-specific shader program handling while maintaining a consistent API.
 *			Implementations include OpenGlShaderProgram for OpenGL shader programs and MetalShaderProgram for Metal-based shader programs.
 */
namespace CE::Render::Shader {

class I_Shader;

/**
 * @class I_ShaderProgram
 * @brief Interface for shader program management
 * @details Defines the common interface for all shader program implementations, allowing for platform-specific shader program handling while maintaining a consistent API.
 */
class I_ShaderProgram {
public:
	virtual ~I_ShaderProgram() = default;

public:
	/**
	 * @brief Binds the shader program for use in rendering
	 * @details Pure virtual method that binds the shader program, making it active for subsequent rendering operations.
	 *			Must be implemented by derived classes to ensure proper activation of the shader program.
	 */
	virtual void Bind() const = 0;

	/**
	 * @brief Unbinds the shader program
	 * @details Pure virtual method that unbinds the shader program, deactivating it from subsequent rendering operations.
	 *			Must be implemented by derived classes to ensure proper deactivation of the shader program.
	 */
	virtual void Unbind() const = 0;

	/**
	 * @brief Links the shader program
	 * @details Pure virtual method that links the shader program, combining the attached shaders into a complete program that can be used for rendering.
	 *			Must be implemented by derived classes to ensure proper linking of the shader program and validation of the attached shaders.
	 */
	virtual void Link() = 0;

	/**
	 * @brief Adds a shader to the shader program
	 * @param shader Pointer to the shader to add
	 * @details Pure virtual method that adds a shader to the shader program. This allows the shader program to manage multiple shaders (e.g., vertex and fragment shaders) and link them together for rendering.
	 *			Must be implemented by derived classes to ensure proper management of shaders within the program.
	 */
	virtual void AddShader(I_Shader* shader) = 0;

	/**
	 * @brief Removes a shader from the shader program
	 * @param shader Pointer to the shader to remove
	 * @details Pure virtual method that removes a shader from the shader program. This allows the shader program to manage its shaders and free up resources when a shader is no longer needed.
	 *			Must be implemented by derived classes to ensure proper management of shaders within the program.
	 */
	virtual void RemoveShader(I_Shader* shader) = 0;

public:
	/**
	 * @brief Gets the unique identifier for the shader program
	 * @return uint32_t Unique identifier for the shader program (e.g., OpenGL program ID)
	 * @details Pure virtual method that returns a unique identifier for the shader program, which can be used for various operations such as binding and resource management.
	 *			Must be implemented by derived classes to provide access to the underlying shader program resource.
	 */
	[[nodiscard]] virtual uint32_t GetProgramId() const = 0;

	/**
	 * @brief Checks if the shader program is currently in use
	 * @return bool True if the shader program is currently bound and active, false otherwise
	 * @details Pure virtual method that returns whether the shader program is currently in use (i.e., bound for rendering). This allows the application to query the state of the shader program and manage rendering operations accordingly.
	 *			Must be implemented by derived classes to provide accurate information about the shader program's usage state.
	 */
	[[nodiscard]] virtual bool IsUsed() const = 0;

	/**
	 * @brief Checks if the shader program is successfully linked
	 * @return bool True if the shader program is linked and ready for use, false otherwise
	 * @details Pure virtual method that returns whether the shader program is successfully linked and ready for use in rendering. This allows the application to ensure that the shader program is properly set up before attempting to use it for rendering operations.
	 *			Must be implemented by derived classes to provide accurate information about the shader program's link status.
	 */
	[[nodiscard]] virtual bool IsLinked() const = 0;
};

}

#endif //CE_RENDER_SHADER_I_SHADERPROGRAM_HPP
