//
// Module: CelestialEngine/Engine/Modules/Render/Shader/Platforms/Mac/Metal
// File: MetalShaderLibrary.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-31
// Updated by: Catalin Chirosca
// Updated: 2026-05-31
//

#pragma once

#ifndef CE_RENDER_SHADER_METALSHADERLIBRARY_HPP
#define CE_RENDER_SHADER_METALSHADERLIBRARY_HPP

#include "Define/DynamicLinker.hpp"

#include "Apple/MetalCpp/Foundation/Foundation.hpp"

#include <string>
#include <unordered_map>

namespace MTL {
class Device;
class Function;
class Library;
}

namespace CE::Core::Render::Shader {

struct ShaderProgram {
	MTL::Function* vertexFunction = nullptr;
	MTL::Function* fragmentFunction = nullptr;
};

class CE_API MetalShaderLibrary {
public:
	explicit MetalShaderLibrary(MTL::Device* device, const std::string& path = "");

	MetalShaderLibrary(const MetalShaderLibrary&) = delete;
	MetalShaderLibrary(MetalShaderLibrary&&) = delete;

	~MetalShaderLibrary();

public:
	MetalShaderLibrary& operator=(const MetalShaderLibrary&) = delete;
	MetalShaderLibrary& operator=(MetalShaderLibrary&&) = delete;

public:
	[[nodiscard]] ShaderProgram GetShaderProgram(const std::string& vertexName, const std::string& fragmentName) const;

	[[nodiscard]] MTL::Function* GetShaderFunction(const std::string& name) const;

	[[nodiscard]] const std::unordered_map<std::string, MTL::Function*>& GetAllShaderFunctions() const { return _functions; }

	[[nodiscard]] const MTL::Library* GetLibrary() const { return _library.get(); }

	[[nodiscard]] bool IsValid() const;

private:
	void _LoadLibrary();

private:
	NS::SharedPtr<MTL::Device> _device;
	NS::SharedPtr<MTL::Library> _library;

	std::unordered_map<std::string, MTL::Function*> _functions;
};

}

#endif //CE_RENDER_SHADER_METALSHADERLIBRARY_HPP
