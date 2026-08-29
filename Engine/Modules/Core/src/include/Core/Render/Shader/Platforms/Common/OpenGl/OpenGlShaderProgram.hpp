//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Common/OpenGl
// File: OpenGlShaderProgram.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_OPENGLSHADERPROGRAM_HPP
#define CE_CORE_RENDER_SHADER_OPENGLSHADERPROGRAM_HPP

#include "Define/DynamicLinker.hpp"

#include "Core/Render/Shader/I_ShaderProgram.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShader.hpp"

#include <initializer_list>
#include <vector>


namespace CE::Core {

/**
 * @class OpenGlShaderProgram
 * @brief OpenGL-specific shader program implementation
 * @details Provides an OpenGL-specific implementation of the I_ShaderProgram interface for managing shader programs in an OpenGL context.
 *			Handles shader program creation, linking, and resource management, and provides access to the underlying OpenGL program ID for use in rendering operations.
 */
class CE_CORE_API OpenGlShaderProgram: public I_ShaderProgram {
public:
	/**
	 * @brief Default constructor for OpenGlShaderProgram
	 * @details Creates a new OpenGL shader program by calling glCreateProgram and initializes the program ID. This constructor sets up an empty shader program that can have shaders attached to it before linking.
	 */
	OpenGlShaderProgram() = default;

	/**
	 * @brief Constructs an OpenGlShaderProgram with a list of shaders
	 * @param shaders An initializer list of pointers to I_Shader objects to attach to the shader program
	 * @details Creates a new OpenGL shader program and attaches the provided shaders to it. The constructor initializes the program ID and iterates through the list of shaders, attaching each one to the program using glAttachShader. This allows for convenient creation of a shader program with multiple shaders in a single step.
	 */
	OpenGlShaderProgram(std::initializer_list<OpenGlShader> shaders);

	OpenGlShaderProgram(const OpenGlShaderProgram&) = default;

	OpenGlShaderProgram(OpenGlShaderProgram&& other) noexcept;

	/**
	 * @brief Destructor for OpenGlShaderProgram
	 * @details Cleans up the OpenGL shader program resources by unbinding the program, detaching all attached shaders, and deleting the shader program. This ensures that all associated OpenGL resources are properly released when the OpenGlShaderProgram object is destroyed.
	 */
	~OpenGlShaderProgram() override;

public:
	OpenGlShaderProgram& operator=(const OpenGlShaderProgram& other);

	OpenGlShaderProgram& operator=(OpenGlShaderProgram&& other) noexcept;

public:
	/**
	 * @brief Binds the shader program for use in rendering
	 * @details Attaches all shaders to the shader program and links it, making it active for subsequent rendering operations. This method ensures that the shader program is properly set up and ready for use in the rendering pipeline.
	 */
	void Bind() const;

	/**
	 * @brief Unbinds the shader program
	 * @details Detaches all shaders from the shader program and unbinds it, deactivating it from subsequent rendering operations. This method ensures that the shader program is properly cleaned up and no longer active in the rendering pipeline.
	 */
	void Unbind() const;

	/**
	 * @brief Links the shader program
	 * @details Links the shader program by combining the attached shaders into a complete program that can be used for rendering. This method checks for linking errors and logs any issues that arise during the linking process, ensuring that the shader program is valid and ready for use.
	 */
	void Link() override;

	/**
	 * @brief Adds a shader to the shader program
	 * @param shader Pointer to the shader to add
	 * @details Adds a shader to the shader program, allowing the program to manage multiple shaders (e.g., vertex and fragment shaders) and link them together for rendering. This method checks for null pointers, duplicate shaders, and whether the program is already linked or in use before adding the shader.
	 */
	void AddShader(OpenGlShader&& shader);

	/**
	 * @brief Removes a shader from the shader program
	 * @param shader Pointer to the shader to remove
	 * @details Removes a shader from the shader program, allowing the program to manage its shaders and free up resources when a shader is no longer needed. This method checks for null pointers and whether the program is linked or in use before removing the shader.
	 */
	void RemoveShader(const OpenGlShader& shader);

public:
	/**
	 * @brief Gets the unique identifier for the shader program
	 * @return u32 Unique identifier for the shader program (e.g., OpenGL program ID)
	 */
	[[nodiscard]] u32 GetProgramId() const override;

	/**
	 * @brief Checks if the shader program is successfully linked
	 * @return bool True if the shader program is linked successfully, false otherwise
	 */
	[[nodiscard]] bool IsLinked() const override;

	/**
	 * @brief Checks if the shader program is currently in use (bound)
	 * @return bool True if the shader program is currently bound for rendering, false otherwise
	 */
	[[nodiscard]] bool IsUsed() const override;

private:
	u32 _programID = 0;
	std::vector<OpenGlShader> _shaders; ///< Owned shader objects; cleared after linking
};

}

#endif //CE_CORE_RENDER_SHADER_OPENGLSHADERPROGRAM_HPP

