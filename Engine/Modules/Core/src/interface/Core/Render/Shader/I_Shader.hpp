//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader
// File: I_Shader.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_I_SHADER_HPP
#define CE_CORE_RENDER_SHADER_I_SHADER_HPP

#include "Types/Var/Vars.hpp"

namespace CE::Types {
	enum class ShaderType: u8;
}

namespace CE::Core {

/**
 * @class I_Shader
 * @brief Interface for shader management
 * @details Defines the common interface for all shader implementations, allowing for platform-specific shader handling while maintaining a consistent API.
 */
class I_Shader {
public:
	virtual ~I_Shader() = default;

public:
	/**
	 * @brief Gets the unique identifier for the shader
	 * @return u32 Unique identifier for the shader (e.g., OpenGL shader ID)
	 * @details Pure virtual method that returns a unique identifier for the shader, which can be used for various operations such as binding and resource management.
	 *			Must be implemented by derived classes to provide access to the underlying shader resource.
	 */
	[[nodiscard]] virtual u32 GetShaderId() const = 0;

	/**
	 * @brief Gets the type of the shader
	 * @return Types::Render::ShaderType Enum value representing the shader type (e.g., Vertex, Fragment)
	 * @details Pure virtual method that returns the type of the shader, allowing the application to query and manage shaders based on their types.
	 *			Must be implemented by derived classes to provide information about the shader's role in the rendering pipeline.
	 */
	[[nodiscard]] virtual Types::ShaderType GetType() const = 0;
};

}

#endif //CE_CORE_RENDER_SHADER_I_SHADER_HPP
