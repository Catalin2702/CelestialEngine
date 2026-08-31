//
// Module: CelestialEngine/Engine/Modules/Core/Render/Pipeline/Platforms/Common/OpenGl
// File: OpenGlPipelineState.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#include "Core/Render/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.hpp"
#include "Tools/Log/Log.hpp"

#include <glad/glad.h>

#include <format>
#include <stdexcept>
#include <utility>
#include <vector>


namespace CE::Core {

namespace {

/// Checks one stage of the descriptor and returns the OpenGL shader name to attach.
u32 ResolveStage(const std::shared_ptr<I_ShaderModule>& module, const Types::ShaderType expectedStage) {
	const auto stageName = Types::ToString(expectedStage);

	if (not module) [[unlikely]] {
		const auto message = std::format("OpenGlPipelineState: the descriptor has no {} shader module.", stageName);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	// The descriptor is API-agnostic, so nothing but this check stops a Metal module from reaching an OpenGL pipeline.
	const auto openGlModule = dynamic_cast<const OpenGlShaderModule*>(module.get());
	if (not openGlModule) [[unlikely]] {
		const auto message = std::format("OpenGlPipelineState: the {} shader module is not an OpenGl module.", stageName);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	if (openGlModule->GetStage() != expectedStage) [[unlikely]] {
		const auto message = std::format("OpenGlPipelineState: a {} module was expected, but the descriptor holds a {} one.", stageName, Types::ToString(openGlModule->GetStage()));
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	if (not openGlModule->IsValid()) [[unlikely]] {
		const auto message = std::format("OpenGlPipelineState: the {} shader module holds no compiled shader.", stageName);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	return openGlModule->GetShaderId();
}

/// Reads a failed program's info log, deletes the program and throws.
[[noreturn]] void ThrowLinkFailure(const u32 programID) {
	GLint logLength = 0;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

	std::string infoLog = "No info log available";
	if (logLength > 0) {
		std::vector<char> buffer(logLength);
		glGetProgramInfoLog(programID, logLength, &logLength, buffer.data());
		infoLog.assign(buffer.data(), static_cast<size_t>(logLength));
	}

	glDeleteProgram(programID);

	const auto message = std::format("OpenGlPipelineState: failed to link the shader program: {}", infoLog);
	CE_CORE_ERROR(message);
	throw std::runtime_error(message);
}

}

OpenGlPipelineState::OpenGlPipelineState(PipelineDescriptor descriptor): _descriptor(std::move(descriptor)) {
	// GL_SRC_ALPHA_SATURATE is a source-only factor; caught here rather than left to a GL_INVALID_ENUM at draw time.
	if (not _descriptor.blendState.HasValidDestinationFactors()) [[unlikely]] {
		const auto message = std::format("OpenGlPipelineState: '{}' is not a valid destination blend factor.", Types::ToString(Types::BlendFactor::SrcAlphaSaturated));
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	const auto vertexShaderId = ResolveStage(_descriptor.vertexShader, Types::ShaderType::Vertex);
	const auto fragmentShaderId = ResolveStage(_descriptor.fragmentShader, Types::ShaderType::Fragment);

	_programID = glCreateProgram();
	if (_programID == 0) [[unlikely]] {
		constexpr auto message = "OpenGlPipelineState: glCreateProgram failed; is a GL context current?";
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	glAttachShader(_programID, vertexShaderId);
	glAttachShader(_programID, fragmentShaderId);

	glLinkProgram(_programID);

	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, &isLinked);

	// Detach either way: on success the program keeps its own copy of the compiled code, on failure the shaders must
	// not stay attached to a program that is about to be deleted.
	glDetachShader(_programID, vertexShaderId);
	glDetachShader(_programID, fragmentShaderId);

	if (isLinked == GL_FALSE) [[unlikely]] {
		const auto failedProgram = _programID;
		_programID = 0;
		ThrowLinkFailure(failedProgram);
	}
}

OpenGlPipelineState::OpenGlPipelineState(OpenGlPipelineState&& other) noexcept:
	_descriptor(std::move(other._descriptor)), _programID(other._programID) {
	// The moved-from pipeline must forget the name, otherwise its destructor deletes the program we just took.
	other._programID = 0;
}

OpenGlPipelineState& OpenGlPipelineState::operator = (OpenGlPipelineState&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	if (_programID != 0)
		glDeleteProgram(_programID);

	_descriptor = std::move(other._descriptor);
	_programID = other._programID;
	other._programID = 0;

	return *this;
}

OpenGlPipelineState::~OpenGlPipelineState() {
	if (_programID == 0) [[unlikely]]
		return;

	glDeleteProgram(_programID);
}

void OpenGlPipelineState::Apply() const {
	glUseProgram(_programID);

	if (_descriptor.depthState.testEnabled) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(Types::ToOpenGl(_descriptor.depthState.compareFunc));
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
	// Set outside the branch: the mask also gates the depth clear, which runs with the test off.
	glDepthMask(_descriptor.depthState.writeEnabled ? GL_TRUE : GL_FALSE);

	if (_descriptor.cullMode == Types::CullMode::None) {
		glDisable(GL_CULL_FACE);
	}
	else {
		glEnable(GL_CULL_FACE);
		glCullFace(Types::ToOpenGl(_descriptor.cullMode));
	}
	// Winding decides which side is "front", so it matters to the depth/stencil sides even with culling off.
	glFrontFace(Types::ToOpenGl(_descriptor.frontFace));

	if (_descriptor.blendState.enabled) {
		glEnable(GL_BLEND);
		glBlendFuncSeparate(
			Types::ToOpenGl(_descriptor.blendState.sourceColor),
			Types::ToOpenGl(_descriptor.blendState.destinationColor),
			Types::ToOpenGl(_descriptor.blendState.sourceAlpha),
			Types::ToOpenGl(_descriptor.blendState.destinationAlpha)
		);
	}
	else {
		glDisable(GL_BLEND);
	}
}

}
