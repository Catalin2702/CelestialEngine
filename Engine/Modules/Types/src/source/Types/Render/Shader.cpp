//
// Module: CelestialEngine/Engine/Modules/Types/Shader
// File: Shader.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-07
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Types/Render/Shader.hpp"

#include "Types/Render/GraphicsApi.hpp"


namespace CE::Types {

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

}
