//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader
// File: ShaderModuleDescriptor.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
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
 * @details The two backends read different halves of it: OpenGL compiles `source` and ignores `entryPoint` (GLSL
 *			always enters at main), Metal looks `entryPoint` up in the already compiled .metallib and ignores `source`.
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
