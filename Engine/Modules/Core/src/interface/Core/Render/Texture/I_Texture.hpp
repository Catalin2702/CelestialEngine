//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain
// File: I_Texture.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#pragma once

#ifndef CE_CORE_RENDER_TEXTURE_I_TEXTURE_HPP
#define CE_CORE_RENDER_TEXTURE_I_TEXTURE_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <utility>
#include <string_view>


namespace CE::Core {

/**
 * @struct TextureDescriptor
 * @brief Everything a texture has to be told before it is allocated
 * @details Deliberately short. There is no mip count, no array length and no sampler here, because the only thing
 *			the engine allocates a texture for today is a render target it reads back once - and a field invented
 *			ahead of its first use is a guess that every backend then has to honour.
 */
struct CE_CORE_API TextureDescriptor {
	u32 width = 0;
	u32 height = 0;

	Types::PixelFormat format = Types::PixelFormat::RGBA8Unorm;
	/// Both flags by default: a render target nobody can read is only useful as a depth buffer, and that case says so.
	Types::TextureUsage usage = Types::TextureUsage::RenderTarget | Types::TextureUsage::ShaderRead;

	/// Borrowed for the duration of the call only - it becomes the object's label in a GPU capture.
	std::string_view debugName = {};
};

/**
 * @class I_Texture
 * @brief An image the GPU owns: a render target, and later anything sampled
 * @details The type RenderPassDescriptor has been pointing at since it was written. It carries no way to get at the
 *			native object, on purpose: a backend casts to its own concrete class after checking GetGraphicApi, the
 *			same contract the pipelines and the buffers already follow.
 *
 *			Immutable once created. Every API allows the contents to change and none allows the size or the format to,
 *			so a resize is a new texture and whoever held the old one has a stale pointer - which is exactly what
 *			I_Swapchain::Resize already warns about.
 */
class I_Texture {
public:
	virtual ~I_Texture() = default;

public:
	[[nodiscard]] virtual u32 GetWidth() const = 0;
	[[nodiscard]] virtual u32 GetHeight() const = 0;
	[[nodiscard]] virtual std::pair<u32, u32> GetSize() const = 0;
	[[nodiscard]] virtual Types::PixelFormat GetFormat() const = 0;
	[[nodiscard]] virtual Types::TextureUsage GetUsage() const = 0;

	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;
};

template<Types::GraphicsApi Api>
class I_TextureBase: public I_Texture {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return Api; }
};

}

#endif //CE_CORE_RENDER_TEXTURE_I_TEXTURE_HPP
