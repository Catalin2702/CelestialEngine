//
// Module: CelestialEngine/Engine/Modules/RHI/Shader/Platforms/Common/OpenGl
// File: OpenGlShaderCompiler.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#include "RHI/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp"
#include "RHI/Conversion/Platforms/Common/OpenGl/OpenGlConversion.hpp"
#include "Tools/Tools.hpp"

#include <glad/glad.h>

#include <vector>


namespace CE::RHI {

u32 OpenGlShaderCompiler::Compile(const char* shader, const Types::ShaderType type) {
	const u32 shaderId = glCreateShader(ToOpenGlShaderType(type));

	glShaderSource(shaderId, 1, &shader, nullptr);

	glCompileShader(shaderId);

	GLint isCompiled = 0;

	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) [[unlikely]] {
		GLint maxLength = 0;

		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		if (maxLength <= 0) [[unlikely]] {
			CE_CORE_ERROR("OpenGlShaderCompiler::Compile: Failed to compile shader: No info log available");
			glDeleteShader(shaderId);
			throw std::runtime_error("OpenGlShaderCompiler::Compile: Failed to compile shader: No info log available");
		}

		std::vector<char> infoLog(maxLength);

		glGetShaderInfoLog(shaderId, maxLength, &maxLength, infoLog.data());

		glDeleteShader(shaderId);

		CE_CORE_ERROR("OpenGlShaderCompiler::Compile: Failed to compile shader: {0}", infoLog.data());

		throw std::runtime_error("OpenGlShaderCompiler::Compile: Failed to compile shader");
	}

	return shaderId;
}

}
