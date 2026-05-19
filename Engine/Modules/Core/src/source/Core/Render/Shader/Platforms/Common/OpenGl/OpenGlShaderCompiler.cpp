//
// Module: CelestialEngine/Engine/Modules/Render/Shader
// File: OpenGlShaderCompiler.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-05-19
//

#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Render/Shader.hpp"

#include <glad/glad.h>

#include <vector>

namespace CE::Core::Render::Shader {

uint32_t OpenGlShaderCompiler::Compile(const char* shader, const Types::Render::ShaderType type) {
	const uint32_t shaderId = glCreateShader(Types::Render::ToOpenGlShaderType(type));

	glShaderSource(shaderId, 1, &shader, nullptr);

	glCompileShader(shaderId);

	GLint isCompiled = 0;

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		if (maxLength <= 0) {
			CE_CORE_ERROR("OpenGlShaderCompiler::Compile: Failed to compile shader: No info log available");
			glDeleteShader(shaderId);
			throw std::runtime_error("OpenGlShaderCompiler::Compile: Failed to compile shader: No info log available");
		}

		std::vector<char> infoLog(maxLength);

		glGetShaderInfoLog(shaderId, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(shaderId);

		CE_CORE_ERROR("OpenGlShaderCompiler::Compile: Failed to compile shader: {0}", infoLog.data());

		throw std::runtime_error("OpenGlShaderCompiler::Compile: Failed to compile shader");
	}

	return shaderId;
}

}
