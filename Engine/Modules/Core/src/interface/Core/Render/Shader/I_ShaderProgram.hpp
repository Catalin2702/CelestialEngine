//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader
// File: I_ShaderProgram.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_I_SHADERPROGRAM_HPP
#define CE_CORE_RENDER_SHADER_I_SHADERPROGRAM_HPP

#include "Types/Var/Vars.hpp"


namespace CE::Core {

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
	 * @brief Links the shader program
	 * @details Pure virtual method that links the shader program, combining the attached shaders into a complete program that can be used for rendering.
	 *			Must be implemented by derived classes to ensure proper linking of the shader program and validation of the attached shaders.
	 */
	virtual void Link() = 0;

public:
	/**
	 * @brief Gets the unique identifier for the shader program
	 * @return u32 Unique identifier for the shader program (e.g., OpenGL program ID)
	 * @details Pure virtual method that returns a unique identifier for the shader program, which can be used for various operations such as binding and resource management.
	 *			Must be implemented by derived classes to provide access to the underlying shader program resource.
	 */
	[[nodiscard]] virtual u32 GetProgramId() const = 0;

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

#endif //CE_CORE_RENDER_SHADER_I_SHADERPROGRAM_HPP
