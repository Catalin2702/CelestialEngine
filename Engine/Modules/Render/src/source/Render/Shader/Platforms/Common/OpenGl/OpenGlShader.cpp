//
// Module: CelestialEngine/Engine/Modules/Render/Shader
// File: OpenGlShader.cpp
// Created by: Catalin Chirosca
// Created: 2026-04-01
// Updated by: Catalin Chirosca
// Updated: 2026-04-02
//

#include "Render/Shader/Platforms/Common/OpenGl/OpenGlShader.hpp"

#include "Tools/Log/Log.hpp"

#include <glad/glad.h>

#include <vector>

namespace CE::Render::Shader {


OpenGlShader::OpenGlShader(const std::string& vertexSource, const std::string& fragmentSource) {
	const auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vertexSourceCStr = vertexSource.c_str();

	glShaderSource(vertexShader, 1, &vertexSourceCStr, nullptr);
	glCompileShader(vertexShader);

	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(vertexShader);

		CE_CORE_ERROR("Failed to compile vertex shader:\n{0}", infoLog.data());
		return;
	}

	const auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fragmentSourceCStr = fragmentSource.c_str();

	glShaderSource(fragmentShader, 1, &fragmentSourceCStr, nullptr);
	glCompileShader(fragmentShader);

	isCompiled = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

		glDeleteShader(fragmentShader);

		CE_CORE_ERROR("Failed to compile fragment shader:\n{0}", infoLog.data());
		return;
	}

	_rendererId = glCreateProgram();
	glAttachShader(_rendererId, vertexShader);
	glAttachShader(_rendererId, fragmentShader);
	glLinkProgram(_rendererId);

	GLint isLinked = 0;
	glGetProgramiv(_rendererId, GL_LINK_STATUS, static_cast<int*>(&isLinked));
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_rendererId, GL_INFO_LOG_LENGTH, &maxLength);
		std::vector<GLchar> infoLog(maxLength);

		glGetProgramInfoLog(_rendererId, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(_rendererId);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		if (infoLog.empty()) {
			CE_CORE_ERROR("Failed to link shader _rendererId: No additional information provided by OpenGL");
		}
		else {
			CE_CORE_ERROR("Failed to link shader _rendererId:\n{0}", infoLog.data());
		}
		return;
	}

	glDetachShader(_rendererId, vertexShader);
	glDetachShader(_rendererId, fragmentShader);
}

OpenGlShader::~OpenGlShader() {
	glDeleteProgram(_rendererId);
}

void OpenGlShader::Bind() const {
	glUseProgram(_rendererId);
}

void OpenGlShader::Unbind() const {
	glUseProgram(0);
}

}
