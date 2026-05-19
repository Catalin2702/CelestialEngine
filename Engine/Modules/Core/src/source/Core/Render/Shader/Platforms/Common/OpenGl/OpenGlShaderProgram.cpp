//
// Module: CelestialEngine/Engine/Modules/Render/Shader
// File: OpenGlShaderProgram.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-05-19
//

#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderProgram.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShader.hpp"
#include "Tools/Log/Log.hpp"

#include <glad/glad.h>

#include <memory>

namespace CE::Core::Render::Shader {

OpenGlShaderProgram::OpenGlShaderProgram() {
	_programId = glCreateProgram();
}

OpenGlShaderProgram::OpenGlShaderProgram(const std::initializer_list<I_Shader*> shaders) {
	_programId = glCreateProgram();
	for (const auto shader : shaders) {
		glAttachShader(_programId, shader->GetShaderId());
		_shaders.emplace_back(shader); // takes ownership of the raw pointer
	}
}

OpenGlShaderProgram::~OpenGlShaderProgram() {
	OpenGlShaderProgram::Unbind();

	// If Link() was never called, shaders may still be attached; detach them before deleting.
	for (const auto& shader : _shaders)
		glDetachShader(_programId, shader->GetShaderId());
	_shaders.clear(); // unique_ptrs destroy each OpenGlShader -> glDeleteShader()

	glDeleteProgram(_programId);
}

void OpenGlShaderProgram::Bind() const {
	glUseProgram(_programId);
}

void OpenGlShaderProgram::Unbind() const {
	glUseProgram(0);
}

void OpenGlShaderProgram::Link() {
	glLinkProgram(_programId);

	GLint isLinked = 0;
	glGetProgramiv(_programId, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength);

		if (maxLength <= 0) {
			CE_CORE_ERROR("OpenGlShaderProgram::Link: Failed to link shader program: No info log available");
			glDeleteProgram(_programId);
			_shaders.clear();
			throw std::runtime_error("OpenGlShaderProgram::Link: Failed to link shader program: No info log available");
		}

		std::vector<char> infoLog(maxLength);
		glGetProgramInfoLog(_programId, maxLength, &maxLength, &infoLog[0]);

		glDeleteProgram(_programId);
		_shaders.clear();

		CE_CORE_ERROR("OpenGlShaderProgram::Link: Failed to link shader program: {0}", infoLog.data());
		throw std::runtime_error("OpenGlShaderProgram::Link: Failed to link shader program");
	}

	// Detach and delete shader objects: once the program is linked their source is no longer needed.
	for (const auto& shader : _shaders)
		glDetachShader(_programId, shader->GetShaderId());
	_shaders.clear(); // unique_ptrs destroy each OpenGlShader -> glDeleteShader()
}

void OpenGlShaderProgram::AddShader(I_Shader* shader) {
	if (shader == nullptr) {
		CE_CORE_WARN("OpenGlShaderProgram::AddShader: Attempted to add a null shader to the shader program. Ignoring.");
		return;
	}
	if (IsLinked()) {
		CE_CORE_WARN("OpenGlShaderProgram::AddShader: Cannot add shader to a linked shader program. Please link the program after adding all shaders.");
		return;
	}
	if (IsUsed()) {
		CE_CORE_WARN("OpenGlShaderProgram::AddShader: Cannot add shader to a shader program that is currently in use. Please unbind the program before adding shaders.");
		return;
	}

	if (std::ranges::find_if(_shaders, [shader](const auto& s) { return s->GetShaderId() == shader->GetShaderId(); }) != _shaders.end()) {
		CE_CORE_WARN("OpenGlShaderProgram::AddShader: Shader is already added to the shader program. Ignoring duplicate.");
		return;
	}
	glAttachShader(_programId, shader->GetShaderId());
	_shaders.emplace_back(shader); // takes ownership
}

void OpenGlShaderProgram::RemoveShader(I_Shader* shader) {
	if (shader == nullptr) {
		CE_CORE_WARN("OpenGlShaderProgram::RemoveShader: Attempted to remove a null shader from the shader program. Ignoring.");
		return;
	}
	if (IsLinked()) {
		CE_CORE_WARN("OpenGlShaderProgram::RemoveShader: Cannot remove shader from a linked shader program. Please link the program after adding all shaders.");
		return;
	}
	if (IsUsed()) {
		CE_CORE_WARN("OpenGlShaderProgram::RemoveShader: Cannot remove shader from a shader program that is currently in use. Please unbind the program before removing shaders.");
		return;
	}

	if (const auto it = std::ranges::find_if(_shaders, [shader](const auto& s) { return s.get() == shader; }); it != _shaders.end()) {
		glDetachShader(_programId, (*it)->GetShaderId());
		_shaders.erase(it); // unique_ptr destructor -> glDeleteShader()
	}
}

uint32_t OpenGlShaderProgram::GetProgramId() const {
	return _programId;
}

bool OpenGlShaderProgram::IsLinked() const {
	GLint isLinked = 0;
	glGetProgramiv(_programId, GL_LINK_STATUS, &isLinked);
	return isLinked == GL_TRUE;
}

bool OpenGlShaderProgram::IsUsed() const {
	GLint currentProgram = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
	return currentProgram == static_cast<GLint>(_programId);
}

}
