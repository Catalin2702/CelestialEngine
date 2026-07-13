//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Common/OpenGl
// File: OpenGlShader.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShader.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp"
#include "Tools/Log/Log.hpp"
#include "Utility/FileSystem/File.hpp"

#include <glad/glad.h>

namespace CE::Core {

OpenGlShader::OpenGlShader(const char* shader, const Types::ShaderType type): _type(type) {
	_shaderId = OpenGlShaderCompiler::Compile(shader, type);
}

OpenGlShader::OpenGlShader(const Utility::File& file, Types::ShaderType type) {
	_type = type;
	_shaderId = OpenGlShaderCompiler::Compile(file.GetContentString().c_str(), type);
}

OpenGlShader::OpenGlShader(OpenGlShader&& other) noexcept {
	_shaderId = other._shaderId;
	_type = other._type;
	other._shaderId = 0;
}

OpenGlShader::~OpenGlShader() {
	if (_shaderId != 0)
		glDeleteShader(_shaderId);
}

OpenGlShader& OpenGlShader::operator=(OpenGlShader&& other) noexcept {
	if (this != &other) {
		if (_shaderId != 0)
			glDeleteShader(_shaderId);

		_shaderId = other._shaderId;
		_type = other._type;

		other._shaderId = 0;
	}

	return *this;
}

uint32_t OpenGlShader::GetShaderId() const {
	return _shaderId;
}

Types::ShaderType OpenGlShader::GetType() const {
	return _type;
}

}

