//
// Module: CelestialEngine/Engine/Modules/Render/Shader
// File: OpenGlShaderProgram.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-05-07
//

#pragma once

#ifndef CE_RENDER_SHADER_OPENGLSHADERPROGRAM_HPP
#define CE_RENDER_SHADER_OPENGLSHADERPROGRAM_HPP

#include "Define/DynamicLinker.hpp"
#include "Render/Shader/I_ShaderProgram.hpp"
#include "Render/Shader/Platforms/Common/OpenGl/OpenGlShader.hpp"

#include <initializer_list>
#include <vector>

namespace CE::Render::Shader {

class CE_API OpenGlShaderProgram: public I_ShaderProgram {
public:
	OpenGlShaderProgram();
	OpenGlShaderProgram(std::initializer_list<I_Shader*> shaders);
	OpenGlShaderProgram(const OpenGlShaderProgram& other);
	~OpenGlShaderProgram() override;

public:
	void Bind() const override;
	void Unbind() const override;

	void AddShader(I_Shader* shader) override;
	void RemoveShader(I_Shader* shader) override;

public:
	/**
	 * @brief Gets the unique identifier for the shader program
	 * @return uint32_t Unique identifier for the shader program (e.g., OpenGL program ID)
	 */
	[[nodiscard]] uint32_t GetProgramId() const override;

	/**
	 * @brief Checks if the shader program is successfully linked
	 * @return bool True if the shader program is linked successfully, false otherwise
	 */
	[[nodiscard]] bool IsLinked() const override;

	/**
	 * @brief Checks if the shader program is currently in use (bound)
	 * @return bool True if the shader program is currently bound for rendering, false otherwise
	 */
	[[nodiscard]] bool IsUsed() const override;

private:
	uint32_t _programId;
	std::vector<I_Shader*> _shaders;
};

}

#endif //CE_RENDER_SHADER_OPENGLSHADERPROGRAM_HPP

