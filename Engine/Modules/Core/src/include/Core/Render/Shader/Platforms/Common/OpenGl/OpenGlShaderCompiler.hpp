//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Common/OpenGl
// File: OpenGlShaderCompiler.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_OPENGLSHADERCOMPILER_HPP
#define CE_CORE_RENDER_SHADER_OPENGLSHADERCOMPILER_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"


namespace CE::Types {
	enum class ShaderType: u8;
}

namespace CE::Core {

/**
 * @class OpenGlShaderCompiler
 * @brief Stateless helper that compiles GLSL source into an OpenGL shader object
 */
class CE_CORE_API OpenGlShaderCompiler {
public:
	/**
	 * @brief Compiles GLSL source code into an OpenGL shader object
	 * @param shader Null-terminated GLSL source code
	 * @param type The shader stage to compile (vertex, fragment, ...)
	 * @return u32 The OpenGL shader object id, or 0 on compilation failure
	 * @details Compilation errors are logged; the caller owns the returned shader object and must delete it
	 *			(or attach it to a program) once linked.
	 */
	static u32 Compile(const char* shader, Types::ShaderType type);
};

}

#endif //CE_CORE_RENDER_SHADER_OPENGLSHADERCOMPILER_HPP
