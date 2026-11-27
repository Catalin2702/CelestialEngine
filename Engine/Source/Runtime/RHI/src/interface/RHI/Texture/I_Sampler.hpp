//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Texture
// File: I_Sampler.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-27
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef CE_RHI_TEXTURE_I_SAMPLER_HPP
#define CE_RHI_TEXTURE_I_SAMPLER_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <string_view>


namespace CE::RHI {

/**
 * @struct SamplerDescriptor
 * @brief How a texture is read: the filters, and what lies outside [0, 1]
 * @details Linear and clamped by default, not repeated: a single image on a quad is the common case, and with a linear filter repeat
 *			blends the opposite edge into the border texels - a thin line of the wrong colour along every side.
 */
struct CE_RHI_API SamplerDescriptor {
	Types::FilterMode minFilter = Types::FilterMode::Linear;
	Types::FilterMode magFilter = Types::FilterMode::Linear;

	Types::AddressMode addressU = Types::AddressMode::ClampToEdge;
	Types::AddressMode addressV = Types::AddressMode::ClampToEdge;

	/// Borrowed for the duration of the call only - it becomes the object's label in a GPU capture.
	std::string_view debugName;
};

/**
 * @class I_Sampler
 * @brief Immutable sampling state, created once and bound beside a texture
 * @details Its own object rather than fields of the texture: the same image is legitimately read with different
 *			filters, and Metal, Vulkan and DirectX 12 keep the two apart anyway. Like the textures it carries no way
 *			to reach the native object; a backend casts to its own class after checking GetGraphicApi.
 */
class I_Sampler {
public:
	virtual ~I_Sampler() = default;

public:
	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;
};

template<Types::GraphicsApi Api>
class I_SamplerBase: public I_Sampler {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return Api; }
};

}

#endif //CE_RHI_TEXTURE_I_SAMPLER_HPP
