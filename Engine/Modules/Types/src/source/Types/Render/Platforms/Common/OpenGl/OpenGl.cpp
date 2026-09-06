//
// Module: CelestialEngine/Engine/Modules/Types/Render/Platforms/Common/OpenGl
// File: OpenGl.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-24
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#include "Types/Render/Platforms/Common/OpenGl/OpenGl.hpp"

#include <glad/glad.h>


static_assert(std::is_same_v<GLint, i32>, "Glad changed the type of GLint; the declaration of GlFormat attributes must follow!");
static_assert(std::is_same_v<GLenum, u32>, "Glad changed the type of GLenum; the declaration of GlFormat attributes must follow!");

namespace CE::Types {

int ToInt(BufferBit bit) {
	return static_cast<int>(bit);
}

GlFormat ToOpenGl(const PixelFormat pixelFormat) {
	switch (pixelFormat) {
		case PixelFormat::RGBA8Unorm: return {.internalFormat = GL_RGBA8, .format = GL_RGBA, .type = GL_UNSIGNED_BYTE};
		case PixelFormat::RGBA8UnormSrgb: return {.internalFormat = GL_SRGB8_ALPHA8, .format = GL_RGBA, .type = GL_UNSIGNED_BYTE};

			// GL has no BGRA internal format: the swizzle is a transfer-time thing, and the storage is RGBA8 either way.
		case PixelFormat::BGRA8Unorm: return {.internalFormat = GL_RGBA8, .format = GL_BGRA, .type = GL_UNSIGNED_BYTE};
		case PixelFormat::BGRA8UnormSrgb: return {.internalFormat = GL_SRGB8_ALPHA8, .format = GL_BGRA, .type = GL_UNSIGNED_BYTE};

		case PixelFormat::RGBA16Float: return {.internalFormat = GL_RGBA16F, .format = GL_RGBA, .type = GL_HALF_FLOAT};
		case PixelFormat::Depth32Float: return {.internalFormat = GL_DEPTH_COMPONENT32F, .format = GL_DEPTH_COMPONENT, .type = GL_FLOAT};

		default: return {.internalFormat = 0, .format = 0, .type = 0};
	}
}

}
