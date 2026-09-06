//
// Module: CelestialEngine/Engine/Modules/Core/Render/Texture/Common/OpenGl
// File: OpenGlTexture.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#pragma once

#ifndef CE_CORE_RENDER_TEXTURE_OPENGLTEXTURE_HPP
#define CE_CORE_RENDER_TEXTURE_OPENGLTEXTURE_HPP

#include "Core/Render/Texture/I_Texture.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"


namespace CE::Core {

/**
 * @class OpenGlTexture
 * @brief A GL texture object, allocated with no contents
 * @details Storage only: glTexImage2D with a null pixel pointer, which is how a render target is declared in GL. The
 *			filters are set to nearest and the wrap to clamp because the one thing that reads this is a full-screen
 *			composite at a 1:1 ratio - and GL_LINEAR on an incomplete mip chain is the classic way to get a black
 *			texture with no error reported.
 */
class CE_CORE_API OpenGlTexture: public I_TextureBase<Types::GraphicsApi::OpenGL> {
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
	u32 _nativeTexture;

	u32 _width;
	u32 _height;

	Types::PixelFormat _format;
	Types::TextureUsage _usage;
};

}

#endif //CE_CORE_RENDER_TEXTURE_OPENGLTEXTURE_HPP
