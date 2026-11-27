//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Texture/Platforms/Common/OpenGl
// File: OpenGlSampler.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-27
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef CE_RHI_TEXTURE_OPENGLSAMPLER_HPP
#define CE_RHI_TEXTURE_OPENGLSAMPLER_HPP

#include "RHI/Texture/I_Sampler.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"


namespace CE::RHI {

/**
 * @class OpenGlSampler
 * @brief A GL sampler object
 * @details Bound to a texture unit with glBindSampler, it overrides the filtering and wrap parameters of whatever
 *			texture is bound to the same unit - which is what lets one OpenGlTexture be read with any sampler, and the
 *			composite keep its own nearest/clamp parameters by binding none.
 */
class CE_RHI_API OpenGlSampler: public I_SamplerBase<Types::GraphicsApi::OpenGL> {
public:
	explicit OpenGlSampler(const SamplerDescriptor& descriptor);

	OpenGlSampler(const OpenGlSampler&) = delete;
	OpenGlSampler(OpenGlSampler&& other) noexcept;

	~OpenGlSampler() override;

public:
	OpenGlSampler& operator = (const OpenGlSampler&) = delete;
	OpenGlSampler& operator = (OpenGlSampler&& other) noexcept;

public:
	[[nodiscard]] u32 GetSampler() const { return _nativeSampler; }

private:
	u32 _nativeSampler = 0;
};

}

#endif //CE_RHI_TEXTURE_OPENGLSAMPLER_HPP
