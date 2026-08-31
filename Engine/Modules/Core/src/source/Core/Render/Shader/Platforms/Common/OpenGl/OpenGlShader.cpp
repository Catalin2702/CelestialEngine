//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Common/OpenGl
// File: OpenGlShader.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShader.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp"
#include "Utility/FileSystem/File.hpp"

#include <glad/glad.h>

#include "Types/Render/Shader.hpp"


namespace CE::Core {

OpenGlShader::OpenGlShader(const char* shader, const Types::ShaderType type): _type(type) {
	_shaderId = OpenGlShaderCompiler::Compile(shader, type);
}

OpenGlShader::OpenGlShader(const Utility::File& file, const Types::ShaderType type) {
	_type = type;
	_shaderId = OpenGlShaderCompiler::Compile(file.GetContentString().c_str(), type);
}

OpenGlShader::OpenGlShader(OpenGlShader&& other) noexcept {
	_shaderId = other._shaderId;
	_type = other._type;
	other._shaderId = 0;
	other._type = Types::ShaderType::None;
}

OpenGlShader::~OpenGlShader() {
	if (_shaderId != 0) [[likely]]
		glDeleteShader(_shaderId);
}

OpenGlShader& OpenGlShader::operator = (const OpenGlShader& other) {
	if (this == &other) [[unlikely]]
		return *this;

	_shaderId = other._shaderId;
	_type = other._type;
	return *this;
}

OpenGlShader& OpenGlShader::operator = (OpenGlShader&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	if (_shaderId != 0) [[unlikely]]
		glDeleteShader(_shaderId);

	_shaderId = other._shaderId;
	_type = other._type;

	other._shaderId = 0;
	other._type = Types::ShaderType::None;

	return *this;
}

u32 OpenGlShader::GetShaderId() const {
	return _shaderId;
}

Types::ShaderType OpenGlShader::GetType() const {
	return _type;
}

}

