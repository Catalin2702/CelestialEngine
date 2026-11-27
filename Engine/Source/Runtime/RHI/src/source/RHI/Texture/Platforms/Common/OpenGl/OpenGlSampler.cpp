//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Texture/Platforms/Common/OpenGl
// File: OpenGlSampler.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-27
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#include "RHI/Texture/Platforms/Common/OpenGl/OpenGlSampler.hpp"
#include "RHI/Conversion/Platforms/Common/OpenGl/OpenGlConversion.hpp"

#include <glad/glad.h>

#include <utility>


namespace CE::RHI {

OpenGlSampler::OpenGlSampler(const SamplerDescriptor& descriptor) {
	glGenSamplers(1, &_nativeSampler);

	glSamplerParameteri(_nativeSampler, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(ToOpenGl(descriptor.minFilter)));
	glSamplerParameteri(_nativeSampler, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(ToOpenGl(descriptor.magFilter)));
	glSamplerParameteri(_nativeSampler, GL_TEXTURE_WRAP_S, static_cast<GLint>(ToOpenGl(descriptor.addressU)));
	glSamplerParameteri(_nativeSampler, GL_TEXTURE_WRAP_T, static_cast<GLint>(ToOpenGl(descriptor.addressV)));
}

OpenGlSampler::OpenGlSampler(OpenGlSampler&& other) noexcept: _nativeSampler(std::exchange(other._nativeSampler, 0)) {}

OpenGlSampler::~OpenGlSampler() {
	if (_nativeSampler != 0)
		glDeleteSamplers(1, &_nativeSampler);
}

OpenGlSampler& OpenGlSampler::operator = (OpenGlSampler&& other) noexcept {
	if (this == &other)
		return *this;

	if (_nativeSampler != 0)
		glDeleteSamplers(1, &_nativeSampler);

	_nativeSampler = std::exchange(other._nativeSampler, 0);

	return *this;
}

}
