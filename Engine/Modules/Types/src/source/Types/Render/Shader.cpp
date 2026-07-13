//
// Module: CelestialEngine/Engine/Modules/Types/Shader
// File: Shader.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Types/Render/Shader.hpp"

#include <glad/glad.h>

#include "Types/Render/Render.hpp"

namespace CE::Types {

const char* ToString(const ShaderType type) {
	switch (type) {
		case ShaderType::Vertex:
			return "Vertex";
		case ShaderType::Fragment:
			return "Fragment";
		case ShaderType::Compute:
			return "Compute";
		case ShaderType::Geometry:
			return "Geometry";
		case ShaderType::TessellationControl:
			return "TessellationControl";
		case ShaderType::TessellationEvaluation:
			return "TessellationEvaluation";
		case ShaderType::Mesh:
			return "Mesh";
		case ShaderType::Amplification:
			return "Amplification";
		case ShaderType::RayGeneration:
			return "RayGeneration";
		default:
			return "Unknown";
	}
}

bool IsShaderTypeSupported(const GraphicsApi graphicsApi, const ShaderType shaderType) {
	switch (graphicsApi) {
		case GraphicsApi::OpenGL: {
			switch (shaderType) {
				case ShaderType::Vertex:
				case ShaderType::Fragment:
				case ShaderType::Compute:
				case ShaderType::Geometry:
				case ShaderType::TessellationControl:
				case ShaderType::TessellationEvaluation:
					return true;
				case ShaderType::Mesh:
				case ShaderType::Amplification:
				case ShaderType::RayGeneration:
				default:
					return false;
			}
		}
		case GraphicsApi::Metal: {
			switch (shaderType) {
				case ShaderType::Vertex:
				case ShaderType::Fragment:
				case ShaderType::Compute:
				case ShaderType::Mesh:
				case ShaderType::Amplification:
				case ShaderType::RayGeneration:
					return true;
				case ShaderType::Geometry:
				case ShaderType::TessellationControl:
				case ShaderType::TessellationEvaluation:
				default:
					return false;
			}
		}
		case GraphicsApi::Vulkan:
		case GraphicsApi::DirectX12: {
			return true;
		}
		case GraphicsApi::DirectX11: {
			switch (shaderType) {
				case ShaderType::Vertex:
				case ShaderType::Fragment:
				case ShaderType::Compute:
				case ShaderType::Geometry:
				case ShaderType::TessellationControl:
				case ShaderType::TessellationEvaluation:
					return true;
				case ShaderType::Mesh:
				case ShaderType::Amplification:
				case ShaderType::RayGeneration:
				default:
					return false;
			}
		}
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
