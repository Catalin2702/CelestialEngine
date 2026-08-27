//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Common/OpenGl
// File: OpenGlShaderProgram.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-08-24
//

#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderProgram.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShader.hpp"
#include "Tools/Log/Log.hpp"

#include <glad/glad.h>

#include <memory>


namespace CE::Core {

OpenGlShaderProgram::OpenGlShaderProgram() {
	_programId = 0;
}

OpenGlShaderProgram::OpenGlShaderProgram(const std::initializer_list<OpenGlShader> shaders): _shaders(shaders) {
	_programId = glCreateProgram();
	for (const auto& shader : _shaders) {
		glAttachShader(_programId, shader.GetShaderId());
	}
}

OpenGlShaderProgram::OpenGlShaderProgram(OpenGlShaderProgram&& other) noexcept: _programId(other._programId), _shaders(std::move(other._shaders)) {
	other._programId = 0;
}

OpenGlShaderProgram& OpenGlShaderProgram::operator=(const OpenGlShaderProgram& other) {
	if (this == &other) [[unlikely]]
		return *this;

	_programId = other._programId;
	_shaders = other._shaders;
	return *this;
}

OpenGlShaderProgram& OpenGlShaderProgram::operator=(OpenGlShaderProgram&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	_programId = other._programId;
	_shaders = std::move(other._shaders);
	other._programId = 0;
	return *this;
}

OpenGlShaderProgram::~OpenGlShaderProgram() {
	OpenGlShaderProgram::Unbind();

	// If Link() was never called, shaders may still be attached; detach them before deleting.
	for (const auto& shader : _shaders)
		glDetachShader(_programId, shader.GetShaderId());

	_shaders.clear();

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
	if (isLinked == GL_FALSE) [[unlikely]] {
		GLint maxLength = 0;
		glGetProgramiv(_programId, GL_INFO_LOG_LENGTH, &maxLength);

		if (maxLength <= 0) [[unlikely]] {
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
		glDetachShader(_programId, shader.GetShaderId());

	_shaders.clear();
}

void OpenGlShaderProgram::AddShader(OpenGlShader&& shader) {
	if (IsLinked()) [[unlikely]] {
		CE_CORE_WARN("OpenGlShaderProgram::AddShader: Cannot add shader to a linked shader program. Please link the program after adding all shaders.");
		return;
	}
	if (IsUsed()) [[unlikely]] {
		CE_CORE_WARN("OpenGlShaderProgram::AddShader: Cannot add shader to a shader program that is currently in use. Please unbind the program before adding shaders.");
		return;
	}

	if (std::ranges::find_if(_shaders, [shader](const auto& s) { return s.GetShaderId() == shader.GetShaderId(); }) != _shaders.end()) [[unlikely]] {
		CE_CORE_WARN("OpenGlShaderProgram::AddShader: Shader is already added to the shader program. Ignoring duplicate.");
		return;
	}
	glAttachShader(_programId, shader.GetShaderId());
	_shaders.emplace_back(shader); // takes ownership
}

void OpenGlShaderProgram::RemoveShader(const OpenGlShader& shader) {
	if (IsLinked()) [[unlikely]] {
		CE_CORE_WARN("OpenGlShaderProgram::RemoveShader: Cannot remove shader from a linked shader program. Please link the program after adding all shaders.");
		return;
	}
	if (IsUsed()) [[unlikely]] {
		CE_CORE_WARN("OpenGlShaderProgram::RemoveShader: Cannot remove shader from a shader program that is currently in use. Please unbind the program before removing shaders.");
		return;
	}

	if (const auto it = std::ranges::find_if(_shaders, [shader](const auto& s) { return s.GetShaderId() == shader.GetShaderId(); }); it != _shaders.end()) [[likely]] {
		glDetachShader(_programId, it->GetShaderId());
		_shaders.erase(it);
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
