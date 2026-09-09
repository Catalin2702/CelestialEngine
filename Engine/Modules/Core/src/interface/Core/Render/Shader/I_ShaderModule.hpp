//
// Module: CelestialEngine/Engine/Modules/Core/Render/Shader
// File: I_ShaderModule.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-09-09
//

#pragma once

#ifndef CE_CORE_RENDER_SHADER_I_SHADERMODULE_HPP
#define CE_CORE_RENDER_SHADER_I_SHADERMODULE_HPP

#include "Types/Types.hpp"

#include <string_view>


namespace CE::Types {
	enum class ShaderType: u8;
}

namespace CE::Core {

/**
 * @class I_ShaderModule
 * @brief One compiled shader stage, ready to be assembled into a pipeline
 * @details A shader module is what an API-agnostic PipelineDescriptor carries instead of source code: the graphic
 *			device turns a ShaderModuleDescriptor into the backend's own object once, and every pipeline built from it
 *			reuses that result. The interface deliberately exposes nothing backend-specific, because one backend's
 *			compiled handle means nothing to another, so a concrete pipeline state downcasts to its own module type to
 *			reach the underlying object.
 */
class I_ShaderModule {
public:
	virtual ~I_ShaderModule() = default;

public:
	/**
	 * @brief Gets the pipeline stage this module was compiled for
	 */
	[[nodiscard]] virtual Types::ShaderType GetStage() const = 0;

	/**
	 * @brief Gets the entry point the stage starts at
	 * @details Fixed to "main" wherever the shading language fixes it, and the function's own name elsewhere.
	 */
	[[nodiscard]] virtual std::string_view GetEntryPoint() const = 0;

	/**
	 * @brief Checks whether the module holds a usable backend object
	 * @details False on a moved-from module; a construction failure throws rather than returning an invalid module.
	 */
	[[nodiscard]] virtual bool IsValid() const = 0;

	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;
};

template<Types::GraphicsApi Api>
class I_ShaderModuleBase: public I_ShaderModule {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return Api; }
};

}

#endif //CE_CORE_RENDER_SHADER_I_SHADERMODULE_HPP
