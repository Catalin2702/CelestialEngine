//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Texture/Platforms/Mac/Metal
// File: MetalSampler.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-27
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef CE_RHI_TEXTURE_METALSAMPLER_HPP
#define CE_RHI_TEXTURE_METALSAMPLER_HPP

#include "RHI/Texture/I_Sampler.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <Foundation/Foundation.hpp>

namespace MTL {
class Device;
class SamplerState;
}


namespace CE::RHI {

/**
 * @class MetalSampler
 * @brief An MTL::SamplerState, immutable from the moment it exists
 */
class CE_RHI_API MetalSampler: public I_SamplerBase<Types::GraphicsApi::Metal> {
public:
	/**
	 * @brief Builds the sampler state the descriptor describes
	 * @details Throws std::runtime_error when the device refuses it, which only a broken device does.
	 */
	MetalSampler(MTL::Device* nativeDevice, const SamplerDescriptor& descriptor);

	MetalSampler(const MetalSampler&) = delete;
	MetalSampler(MetalSampler&& other) noexcept = default;

	/// Out of line because NS::SharedPtr has to see the full MTL::SamplerState to release it.
	~MetalSampler() override;

public:
	MetalSampler& operator = (const MetalSampler&) = delete;
	MetalSampler& operator = (MetalSampler&&) noexcept = default;

public:
	[[nodiscard]] MTL::SamplerState* GetSamplerState() const { return _nativeSamplerState.get(); }

private:
	NS::SharedPtr<MTL::SamplerState> _nativeSamplerState;
};

}

#endif //CE_RHI_TEXTURE_METALSAMPLER_HPP
