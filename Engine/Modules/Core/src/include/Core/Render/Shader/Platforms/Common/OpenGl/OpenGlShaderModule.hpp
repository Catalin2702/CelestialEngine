//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Common/OpenGl
// File: OpenGlShaderModule.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_OPENGLSHADERMODULE_HPP
#define CE_CORE_RENDER_SHADER_OPENGLSHADERMODULE_HPP

#include "Core/Render/Shader/I_ShaderModule.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Render/Shader.hpp"

#include <string>


namespace CE::Core {

struct ShaderModuleDescriptor;

/**
 * @class OpenGlShaderModule
 * @brief A single GLSL stage compiled into an OpenGL shader object
 * @details Owns the shader name returned by OpenGlShaderCompiler and keeps it alive until every pipeline that linked
 *			it is gone - GL only frees a shader once it is detached from all programs, so several pipelines can share
 *			one module safely. Move-only: two objects deleting the same shader name would be a f64-delete.
 */
class CE_CORE_API OpenGlShaderModule: public I_ShaderModule {
public:
	/**
	 * @brief Compiles the descriptor's GLSL source into an OpenGL shader object
	 * @param descriptor Stage and source to compile; entryPoint must be "main", GLSL has no other option
	 * @details Throws std::runtime_error when the stage is one OpenGL does not have, when the source is empty, or
	 *			when compilation fails - the compiler logs the GLSL info log before rethrowing.
	 */
	explicit OpenGlShaderModule(const ShaderModuleDescriptor& descriptor);

	OpenGlShaderModule(const OpenGlShaderModule&) = delete;
	OpenGlShaderModule(OpenGlShaderModule&& other) noexcept;

	~OpenGlShaderModule() override;

public:
	OpenGlShaderModule& operator=(const OpenGlShaderModule&) = delete;
	OpenGlShaderModule& operator=(OpenGlShaderModule&& other) noexcept;

public:
	[[nodiscard]] Types::ShaderType GetStage() const override { return _stage; }
	[[nodiscard]] std::string_view GetEntryPoint() const override { return _entryPoint; }
	[[nodiscard]] bool IsValid() const override { return _shaderId != 0; }

	/**
	 * @brief Gets the OpenGL shader object name, for OpenGlPipelineState to attach
	 * @details Backend-specific on purpose, hence absent from I_ShaderModule: the pipeline downcasts to reach it.
	 */
	[[nodiscard]] u32 GetShaderId() const { return _shaderId; }

private:
	std::string _entryPoint;
	u32 _shaderId = 0;
	Types::ShaderType _stage = Types::ShaderType::None;
};

}

#endif //CE_CORE_RENDER_SHADER_OPENGLSHADERMODULE_HPP
