//
// Module: CelestialEngine/Engine/Modules/Render/Shader
// File: OpenGlShader.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-01
// Updated by: Catalin Chirosca
// Updated: 2026-04-01
//

#pragma once

#ifndef CE_RENDER_SHADER_OPENGLSHADER_HPP
#define CE_RENDER_SHADER_OPENGLSHADER_HPP

#include "Render/Shader/I_Shader.hpp"

#include <string>

namespace CE::Render::Shader {

class OpenGlShader final: public I_Shader {
public:
	OpenGlShader(const std::string& vertexSource, const std::string& fragmentSource);
	~OpenGlShader() override;

	void Bind() const override;
	void Unbind() const override;

private:
	unsigned int _rendererId = 0;					///< OpenGL shader program ID
};

}

#endif //CE_RENDER_SHADER_OPENGLSHADER_HPP
