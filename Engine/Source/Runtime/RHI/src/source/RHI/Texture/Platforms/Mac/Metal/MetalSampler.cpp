//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Texture/Platforms/Mac/Metal
// File: MetalSampler.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-27
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#include "RHI/Texture/Platforms/Mac/Metal/MetalSampler.hpp"
#include "RHI/Conversion/Platforms/Mac/Metal/MetalConversion.hpp"
#include "Tools/Log/Log.hpp"

#include <Metal/Metal.hpp>

#include <stdexcept>
#include <string>


namespace CE::RHI {

MetalSampler::MetalSampler(MTL::Device* nativeDevice, const SamplerDescriptor& descriptor) {
	if (not nativeDevice) [[unlikely]] {
		constexpr auto error = "MetalSampler::MetalSampler: A sampler needs a device!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	const auto samplerDescriptor = NS::TransferPtr(MTL::SamplerDescriptor::alloc()->init());
	samplerDescriptor->setMinFilter(ToMetal(descriptor.minFilter));
	samplerDescriptor->setMagFilter(ToMetal(descriptor.magFilter));

	// Said out loud although it is the default: the textures have one level, and the day they have more this is the
	// line that has to change with them.
	samplerDescriptor->setMipFilter(MTL::SamplerMipFilterNotMipmapped);
	samplerDescriptor->setSAddressMode(ToMetal(descriptor.addressU));
	samplerDescriptor->setTAddressMode(ToMetal(descriptor.addressV));

	if (not descriptor.debugName.empty()) {
		// NS::String::string reads up to a NUL, which a string_view does not promise: copy it into one that does.
		const std::string label{descriptor.debugName};
		samplerDescriptor->setLabel(NS::String::string(label.c_str(), NS::UTF8StringEncoding));
	}

	_nativeSamplerState = NS::TransferPtr(nativeDevice->newSamplerState(samplerDescriptor.get()));
	if (not _nativeSamplerState) [[unlikely]] {
		constexpr auto error = "MetalSampler::MetalSampler: The device refused the sampler state!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
}

MetalSampler::~MetalSampler() = default;

}
