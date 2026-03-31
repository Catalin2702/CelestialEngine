//
// Module: CelestialEngine/Engine/Modules/Render/Shader
// File: I_Shader.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-01
// Updated by: Catalin Chirosca
// Updated: 2026-04-01
//

#pragma once

#ifndef CE_RENDER_SHADER_I_SHADER_HPP
#define CE_RENDER_SHADER_I_SHADER_HPP


namespace CE::Render::Shader  {

class I_Shader {
public:
	virtual ~I_Shader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
};

}

#endif //CE_RENDER_SHADER_I_SHADER_HPP
