//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader/Platforms/Mac/Metal
// File: MetalShaderModule.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_METALSHADERMODULE_HPP
#define CE_CORE_RENDER_SHADER_METALSHADERMODULE_HPP

#include "Core/Render/Shader/I_ShaderModule.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Render/Shader.hpp"

#include <string>


namespace MTL {
	class Function;
}

namespace CE::Core {

class MetalShaderLibrary;
struct ShaderModuleDescriptor;

/**
 * @class MetalShaderModule
 * @brief One entry point of the compiled .metallib, resolved into an MTL::Function
 * @details Metal has no compilation step at this level - the .metallib is built by CMake - so a module is a lookup:
 *			the descriptor's entryPoint names a function the library already holds. The function is retained, so a
 *			module stays valid even if the library it came from is destroyed first. Move-only, like its OpenGL twin.
 */
class CE_CORE_API MetalShaderModule: public I_ShaderModule {
public:
	/**
	 * @brief Resolves the descriptor's entry point in the given library
	 * @param library The loaded .metallib to look the entry point up in
	 * @param descriptor Stage and entryPoint to resolve; source is ignored, Metal shaders are compiled ahead of time
	 * @details Throws std::runtime_error when the stage is one Metal does not have, when entryPoint is empty, or when
	 *			the library holds no function under that name.
	 */
	MetalShaderModule(const MetalShaderLibrary& library, const ShaderModuleDescriptor& descriptor);

	MetalShaderModule(const MetalShaderModule&) = delete;
	MetalShaderModule(MetalShaderModule&& other) noexcept;

	~MetalShaderModule() override;

public:
	MetalShaderModule& operator=(const MetalShaderModule&) = delete;
	MetalShaderModule& operator=(MetalShaderModule&& other) noexcept;

public:
	[[nodiscard]] Types::ShaderType GetStage() const override { return _stage; }
	[[nodiscard]] std::string_view GetEntryPoint() const override { return _entryPoint; }
	[[nodiscard]] bool IsValid() const override { return _function != nullptr; }

	/**
	 * @brief Gets the retained Metal function, for MetalPipelineState to hand to the pipeline descriptor
	 * @details Backend-specific on purpose, hence absent from I_ShaderModule: the pipeline downcasts to reach it.
	 */
	[[nodiscard]] MTL::Function* GetFunction() const { return _function; }

private:
	std::string _entryPoint;
	MTL::Function* _function = nullptr;		///< Retained by this module, released in the destructor
	Types::ShaderType _stage = Types::ShaderType::None;
};

}

#endif //CE_CORE_RENDER_SHADER_METALSHADERMODULE_HPP
