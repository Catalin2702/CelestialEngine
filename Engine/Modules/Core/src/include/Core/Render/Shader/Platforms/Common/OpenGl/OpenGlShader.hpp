//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Common/OpenGl
// File: OpenGlShader.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_OPENGLSHADER_HPP
#define CE_CORE_RENDER_SHADER_OPENGLSHADER_HPP

#include "Define/DynamicLinker.hpp"

#include "Core/Render/Shader/I_Shader.hpp"


namespace CE {
namespace Types {
	enum class ShaderType: uint8_t;
}

namespace Utility {
	class File;
}
}

namespace CE::Core {

/**
 * @class OpenGlShader
 * @brief OpenGL-specific shader implementation
 * @details Provides an OpenGL-specific implementation of the I_Shader interface for managing individual shaders (vertex, fragment, etc.) in an OpenGL context.
 *			Handles shader compilation, resource management, and provides access to the underlying OpenGL shader ID for use in shader programs.
 */
class CE_CORE_API OpenGlShader: public I_Shader {
public:
	/**
	 * @brief Constructs an OpenGlShader from shader source code
	 * @param shader Pointer to the shader source code as a null-terminated string
	 * @param type Type of the shader (e.g., vertex, fragment) specified by the ShaderType enum
	 * @details Compiles the provided shader source code and creates an OpenGL shader object. The shader is compiled based on the specified type, and any compilation errors are logged and result in an exception being thrown.
	 */
	OpenGlShader(const char* shader, Types::ShaderType type);

	/**
	 * @brief Constructs an OpenGlShader from a file containing shader source code
	 * @param file Reference to a File object representing the shader source code file
	 * @param type Type of the shader (e.g., vertex, fragment) specified by the ShaderType enum
	 * @details Loads the shader source code from the provided File object, compiles it, and creates an OpenGL shader object. The shader is compiled based on the specified type, and any compilation errors are logged and result in an exception being thrown. If the file cannot be read or loaded, an exception is also thrown.
	 */
	OpenGlShader(const Utility::File& file, Types::ShaderType type);

	/**
	 * @brief Copy constructor (deleted)
	 * @details Copying an OpenGlShader is not allowed because it would result in two objects sharing the same OpenGL shader ID, leading to a double-delete when both are destroyed.
	 */
	OpenGlShader(const OpenGlShader&) = delete;

	/**
	 * @brief Move constructor for OpenGlShader
	 * @param other Rvalue reference to another OpenGlShader object to move from
	 * @details Transfers ownership of the OpenGL shader ID from another OpenGlShader instance. The moved-from object will have its shader ID set to 0 (invalid), preventing double-delete.
	 */
	OpenGlShader(OpenGlShader&& other) noexcept;

	/**
	 * @brief Destructor for OpenGlShader
	 * @details Cleans up the OpenGL shader resources by detaching the shader from its program, deleting the shader object, and deleting the shader program. This ensures that all associated OpenGL resources are properly released when the OpenGlShader object is destroyed.
	 */
	~OpenGlShader() override;

public:
	/**
	 * @brief Copy assignment operator (deleted)
	 * @details Copying an OpenGlShader is not allowed because it would result in two objects sharing the same OpenGL shader ID, leading to a double-delete when both are destroyed.
	 */
	OpenGlShader& operator=(const OpenGlShader&) = delete;

	/**
	 * @brief Move assignment operator for OpenGlShader
	 * @param other Rvalue reference to another OpenGlShader object to move from
	 * @return OpenGlShader& A reference to the current OpenGlShader object after moving
	 * @details Transfers ownership of the OpenGL shader ID from another OpenGlShader instance. The moved-from object will have its shader ID set to 0 (invalid), preventing double-delete. The current object's existing shader resources are cleaned up before taking ownership of the new shader ID.
	 */
	OpenGlShader& operator=(OpenGlShader&& other) noexcept;

public:
	/**
	 * @brief Gets the OpenGL shader object id
	 */
	[[nodiscard]] uint32_t GetShaderId() const override;
	/**
	 * @brief Gets the shader stage this shader belongs to
	 */
	[[nodiscard]] Types::ShaderType GetType() const override;

private:
	uint32_t _shaderId;
	Types::ShaderType _type;
};

}

#endif //CE_CORE_RENDER_SHADER_OPENGLSHADER_HPP
