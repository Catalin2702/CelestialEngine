//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader
// File: ShaderModuleDescriptor.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-09-09
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
 * @brief The recipe a graphic device needs to produce one shader module
 * @details A backend reads one half of it and ignores the other: one that compiles at runtime takes `source`, one
 *			that loads an artifact built ahead of time looks `entryPoint` up in it.
 *
 *			TODO: carrying both halves is what forces the caller to know which one its backend reads, and where the
 *			artifact lives. The device should resolve that from the entry point alone, and `source` go away.
 *			Every string is borrowed for the duration of the CreateShaderModule call only - a module copies whatever
 *			it needs to keep.
 */
struct CE_CORE_API ShaderModuleDescriptor {
	Types::ShaderType stage = Types::ShaderType::None;	///< The pipeline stage to compile for

	std::string_view source;							///< GLSL source code; unused by Metal
	std::string_view entryPoint = "main";				///< metallib function name; must stay "main" under OpenGL

	std::string_view debugName;							///< Optional name, used in log messages only
};

}

#endif //CE_CORE_RENDER_SHADER_SHADERMODULEDESCRIPTOR_HPP
