//
// Module: CelestialEngine/Engine/Modules/Render/Shader
// File: OpenGlShader.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-05-07
//

#include "Render/Shader/Platforms/Common/OpenGl/OpenGlShader.hpp"
#include "Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp"
#include "Tools/Log/Log.hpp"

#include <glad/glad.h>

namespace CE::Render::Shader {

OpenGlShader::OpenGlShader(const char* shader, const Types::Render::ShaderType type): _type(type) {
	_shaderId = OpenGlShaderCompiler::Compile(shader, type);
}

OpenGlShader::OpenGlShader(const OpenGlShader& other) {
	_shaderId = other._shaderId;
	_type = other._type;
}

OpenGlShader::~OpenGlShader() {
	glDeleteShader(_shaderId);
}

uint32_t OpenGlShader::GetShaderId() const {
	return _shaderId;
}

Types::Render::ShaderType OpenGlShader::GetType() const {
	return _type;
}

}

