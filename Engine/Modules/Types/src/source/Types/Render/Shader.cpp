//
// Module: CelestialEngine/Engine/Modules/Types/Shader
// File: Shader.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-05-07
//

#include "Types/Render/Shader.hpp"

#include <glad/glad.h>

namespace CE::Types::Render {

const char* ToString(const ShaderType type) {
	switch (type) {
		case ShaderType::Vertex:
			return "Vertex";
		case ShaderType::Fragment:
			return "Fragment";
		default:
			return "Unknown";
	}
}

bool IsShaderTypeSupported(const ShaderType type) {
	switch (type) {
		case ShaderType::Vertex:
		case ShaderType::Fragment:
			return true;
		default:
			return false;
	}
}

int ToOpenGlShaderType(const ShaderType type) {
	switch (type) {
		case ShaderType::Vertex:
			return GL_VERTEX_SHADER;
		case ShaderType::Fragment:
			return GL_FRAGMENT_SHADER;
		default:
			return -1; // Invalid shader type
	}
}

}
