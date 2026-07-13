//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Common/OpenGl
// File: OpenGlShaderCompiler.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_OPENGLSHADERCOMPILER_HPP
#define CE_CORE_RENDER_SHADER_OPENGLSHADERCOMPILER_HPP

#include "Define/DynamicLinker.hpp"

#include <cstdint>

namespace CE::Types {
	enum class ShaderType: uint8_t;
}

namespace CE::Core {

class CE_API OpenGlShaderCompiler {
public:
	static uint32_t Compile(const char* shader, Types::ShaderType type);
};

}

#endif //CE_CORE_RENDER_SHADER_OPENGLSHADERCOMPILER_HPP
