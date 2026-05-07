//
// Module: CelestialEngine/Engine/Modules/Render/Shader
// File: OpenGlShaderCompiler.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-05-07
//

#pragma once

#ifndef CE_RENDER_SHADER_OPENGLSHADERCOMPILER_HPP
#define CE_RENDER_SHADER_OPENGLSHADERCOMPILER_HPP

#include "Define/DynamicLinker.hpp"

#include <cstdint>

namespace CE::Types::Render {
	enum class ShaderType: uint32_t;
}

namespace CE::Render::Shader {

class CE_API OpenGlShaderCompiler {
public:
	static uint32_t Compile(const char* shader, Types::Render::ShaderType type);
};

}

#endif //CE_RENDER_SHADER_OPENGLSHADERCOMPILER_HPP
