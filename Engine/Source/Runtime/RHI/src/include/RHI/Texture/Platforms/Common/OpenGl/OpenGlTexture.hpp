//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Texture/Common/OpenGl
// File: OpenGlTexture.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef CE_RHI_TEXTURE_OPENGLTEXTURE_HPP
#define CE_RHI_TEXTURE_OPENGLTEXTURE_HPP

#include "RHI/Texture/I_Texture.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"


namespace CE::RHI {

/**
 * @class OpenGlTexture
 * @brief A GL texture object: a render target allocated empty, or an image uploaded at creation
 * @details One glTexImage2D either way, with a null pointer for a render target and the descriptor's texels for an
 *			image. The filters stay nearest and the wrap clamp, which is what the composite's texelFetch wants; a draw
 *			that samples binds an OpenGlSampler, and a sampler object bound to the unit overrides these parameters.
 */
class CE_RHI_API OpenGlTexture: public I_TextureBase<Types::GraphicsApi::OpenGL> {
public:
	explicit OpenGlTexture(const TextureDescriptor& descriptor);

	OpenGlTexture(const OpenGlTexture&) = delete;
	OpenGlTexture(OpenGlTexture&& other) noexcept;

	~OpenGlTexture() override;

public:
	OpenGlTexture& operator = (const OpenGlTexture&) = delete;
	OpenGlTexture& operator = (OpenGlTexture&&) noexcept;

public:
	[[nodiscard]] u32 GetWidth() const override { return _width; }
	[[nodiscard]] u32 GetHeight() const override { return _height; }
	[[nodiscard]] std::pair<u32, u32> GetSize() const override { return {_width, _height}; }
	[[nodiscard]] Types::PixelFormat GetFormat() const override { return _format; }
	[[nodiscard]] Types::TextureUsage GetUsage() const override { return _usage; }

	[[nodiscard]] u32 GetTexture() const { return _nativeTexture; }

private:
	u32 _nativeTexture = 0;

	u32 _width;
	u32 _height;

	Types::PixelFormat _format;
	Types::TextureUsage _usage;
};

}

#endif //CE_RHI_TEXTURE_OPENGLTEXTURE_HPP
