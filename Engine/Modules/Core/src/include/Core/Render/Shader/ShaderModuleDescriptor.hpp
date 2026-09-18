//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader
// File: ShaderModuleDescriptor.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-09-18
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_SHADERMODULEDESCRIPTOR_HPP
#define CE_CORE_RENDER_SHADER_SHADERMODULEDESCRIPTOR_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Render/Shader.hpp"

#include <string_view>


namespace CE::Core {

/**
 * @struct ShaderModuleDescriptor
 * @brief What a graphic device is asked for when it is to produce one shader module
 * @details A stage and a name, and deliberately nothing else. What a shader *is* differs per backend - source text
 *			compiled on the spot for one, a function of an artifact built ahead of time for another - and so does
 *			where it lives, so a descriptor carrying either would force whoever fills it in to know which backend it
 *			is talking to. The name is the engine's own identifier for the shader, and resolving it into an artifact
 *			is the device's job.
 *
 *			The convention every backend resolves the name through is the same shape: `<Name>` in PascalCase, one
 *			name per stage. Adding a shader means adding it under that name to every backend that must run it, and
 *			nothing else changes.
 *
 *			The strings are borrowed for the duration of the CreateShaderModule call only - a module copies whatever
 *			it needs to keep.
 */
struct CE_CORE_API ShaderModuleDescriptor {
	Types::ShaderType stage = Types::ShaderType::None;	///< The pipeline stage the module is built for

	/// The engine's name for the shader, e.g. "Vertex" or "CompositeFragment". Also what the module is called in log
	/// messages, so it needs no separate debug name.
	std::string_view name;
};

}

#endif //CE_CORE_RENDER_SHADER_SHADERMODULEDESCRIPTOR_HPP
