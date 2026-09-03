//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Common/OpenGl
// File: OpenGlShaderModule.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp"
#include "Core/Render/Shader/ShaderModuleDescriptor.hpp"
#include "Tools/Tools.hpp"
#include "Types/Render/Render.hpp"

#include <glad/glad.h>

#include <format>
#include <stdexcept>
#include <utility>


namespace CE::Core {

OpenGlShaderModule::OpenGlShaderModule(const ShaderModuleDescriptor& descriptor):
	_entryPoint(descriptor.entryPoint), _stage(descriptor.stage) {
	// Owned, not a view: it feeds std::format calls that outlive the ternary's temporaries.
	const std::string name = descriptor.debugName.empty() ? Types::ToString(descriptor.stage) : std::string(descriptor.debugName);

	if (not Types::IsShaderTypeSupported(Types::GraphicsApi::OpenGL, descriptor.stage)) [[unlikely]] {
		const auto message = std::format("OpenGlShaderModule: stage '{}' of module '{}' does not exist in OpenGL.", Types::ToString(descriptor.stage), name);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	if (descriptor.source.empty()) [[unlikely]] {
		const auto message = std::format("OpenGlShaderModule: module '{}' has no GLSL source.", name);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	// GLSL fixes the entry point at main, so anything else silently would not be honoured: say so instead.
	if (_entryPoint != "main") [[unlikely]]
		CE_CORE_WARN("OpenGlShaderModule: module '{}' asks for entry point '{}', but GLSL always enters at main.", name, _entryPoint);

	// string_view is not guaranteed to be null-terminated; glShaderSource wants a C string.
	const std::string source(descriptor.source);
	_shaderId = OpenGlShaderCompiler::Compile(source.c_str(), descriptor.stage);
}

OpenGlShaderModule::OpenGlShaderModule(OpenGlShaderModule&& other) noexcept:
	_entryPoint(std::move(other._entryPoint)), _shaderId(other._shaderId), _stage(other._stage) {
	// The moved-from module must forget the name, otherwise its destructor deletes the shader we just took.
	other._shaderId = 0;
	other._stage = Types::ShaderType::None;
}

OpenGlShaderModule& OpenGlShaderModule::operator = (OpenGlShaderModule&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	if (_shaderId != 0)
		glDeleteShader(_shaderId);

	_entryPoint = std::move(other._entryPoint);
	_shaderId = other._shaderId;
	_stage = other._stage;

	other._shaderId = 0;
	other._stage = Types::ShaderType::None;

	return *this;
}

OpenGlShaderModule::~OpenGlShaderModule() {
	if (_shaderId == 0) [[unlikely]]
		return;

	// GL only frees the object once it is detached from every program, so outliving pipelines is not a leak.
	glDeleteShader(_shaderId);
}

}
