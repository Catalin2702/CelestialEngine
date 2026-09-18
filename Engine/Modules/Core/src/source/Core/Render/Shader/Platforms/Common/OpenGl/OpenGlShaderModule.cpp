//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Common/OpenGl
// File: OpenGlShaderModule.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-09-18
//

#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderCompiler.hpp"
#include "Core/Render/Shader/ShaderModuleDescriptor.hpp"
#include "Tools/Tools.hpp"
#include "Types/Render/Render.hpp"
#include "Utility/FileSystem/File.hpp"
#include "Utility/FileSystem/FileSystem.hpp"

#include <glad/glad.h>

#include <format>
#include <stdexcept>
#include <utility>


namespace CE::Core {

namespace {

/// Where the GLSL shipped with the application is unpacked to, under whichever directory the packaging put it in.
constexpr auto ShadersSubdirectory = "Shaders/OpenGL";

/// GLSL has one entry point and it is spelled this way, so the name never varies from module to module.
constexpr auto GlslEntryPoint = "main";

/**
 * @brief Resolves the engine's name for a shader into the GLSL file that holds it
 * @details The whole convention: one file per name, next to its siblings, with the extension the language uses. It
 *			lives here because it is the OpenGL answer to "where is this shader" - the caller states a name and knows
 *			nothing of files.
 */
std::string ResolveSourcePath(const std::string_view name) {
	return (Utility::FileSystem::GetResourcesDirectory() / ShadersSubdirectory / std::format("{}.glsl", name)).string();
}

}

OpenGlShaderModule::OpenGlShaderModule(const ShaderModuleDescriptor& descriptor):
	_entryPoint(GlslEntryPoint), _stage(descriptor.stage) {
	// Owned, not a view: it feeds std::format calls that outlive the ternary's temporaries.
	const std::string name = descriptor.name.empty() ? std::format("{}", descriptor.stage) : std::string(descriptor.name);

	if (not Types::IsShaderTypeSupported(Types::GraphicsApi::OpenGL, descriptor.stage)) [[unlikely]] {
		const auto message = std::format("OpenGlShaderModule: stage '{}' of module '{}' does not exist in OpenGL.", descriptor.stage, name);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	if (descriptor.name.empty()) [[unlikely]] {
		const auto message = std::format("OpenGlShaderModule: a '{}' module was asked for under no name, so there is no source to find.", descriptor.stage);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

	// Throws on its own, having logged the path, when the file is not where the packaging should have put it - which
	// is the same class of failure as a missing artifact on a backend that compiles ahead of time.
	const auto source = Utility::FileSystem::StLoad(ResolveSourcePath(name)).GetContentString();

	if (source.empty()) [[unlikely]] {
		const auto message = std::format("OpenGlShaderModule: the GLSL file of module '{}' is empty.", name);
		CE_CORE_ERROR(message);
		throw std::runtime_error(message);
	}

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
