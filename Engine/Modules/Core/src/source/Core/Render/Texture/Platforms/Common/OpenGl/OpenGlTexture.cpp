//
// Module: CelestialEngine/Engine/Modules/Core/Render/Texture/Common/OpenGl
// File: OpenGlTexture.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#include "Core/Render/Texture/Platforms/Common/OpenGl/OpenGlTexture.hpp"
#include "Tools/Tools.hpp"

#include <glad/glad.h>

#include <stdexcept>
#include <utility>


namespace CE::Core {

OpenGlTexture::OpenGlTexture(const TextureDescriptor& descriptor):
	_width(descriptor.width), _height(descriptor.height), _format(descriptor.format), _usage(descriptor.usage)
{
	const auto [internalFormat, format, type] = Types::ToOpenGl(_format);
	if (internalFormat == 0 or _width == 0 or _height == 0) [[unlikely]] {
		constexpr auto error = "OpenGlTexture::OpenGlTexture: Unsupported format, or a zero-sized texture!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	glGenTextures(1, &_nativeTexture);
	glBindTexture(GL_TEXTURE_2D, _nativeTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, static_cast<GLsizei>(_width), static_cast<GLsizei>(_height), 0, format, type, nullptr);

	// One level, no filtering, no wrapping: see the class comment.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
}

OpenGlTexture::OpenGlTexture(OpenGlTexture&& other) noexcept:
	_nativeTexture(std::exchange(other._nativeTexture, 0)), _width(other._width), _height(other._height),
	_format(other._format), _usage(other._usage) {}

OpenGlTexture::~OpenGlTexture() {
	if (_nativeTexture != 0)
		glDeleteTextures(1, &_nativeTexture);
}

OpenGlTexture& OpenGlTexture::operator = (OpenGlTexture&& other) noexcept {
	if (this == &other)
		return *this;

	if (_nativeTexture != 0)
		glDeleteTextures(1, &_nativeTexture);

	_nativeTexture = std::exchange(other._nativeTexture, 0);
	_width = other._width;
	_height = other._height;
	_format = other._format;
	_usage = other._usage;

	return *this;
}

}
