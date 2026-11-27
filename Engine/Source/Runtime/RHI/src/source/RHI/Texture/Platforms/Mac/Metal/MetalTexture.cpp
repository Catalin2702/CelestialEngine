//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Texture/Platforms/Mac/Metal
// File: MetalTexture.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#include "RHI/Texture/Platforms/Mac/Metal/MetalTexture.hpp"
#include "RHI/Conversion/Platforms/Mac/Metal/MetalConversion.hpp"
#include "Tools/Tools.hpp"

#include <Metal/Metal.hpp>

#include <stdexcept>
#include <format>

namespace CE::RHI {

MetalTexture::MetalTexture(MTL::Device* nativeDevice, const TextureDescriptor& descriptor):
	_width(descriptor.width), _height(descriptor.height), _format(descriptor.format), _usage(descriptor.usage) {
	if (not nativeDevice or _width == 0 or _height == 0) [[unlikely]] {
		constexpr auto error = "MetalTexture::MetalTexture: A texture needs a device and a non-zero size!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	if (not descriptor.IsEmptyOrValidInitialData()) [[unlikely]] {
		const auto error = std::format("MetalTexture::MetalTexture: {} bytes of initial data do not fill a {}x{} {} texture!",
			descriptor.initialData.size(), _width, _height, _format);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	const bool hasInitialData = not descriptor.initialData.empty();

	// replaceRegion is a CPU write, and the CPU cannot reach private memory. Shared where CPU and GPU see the same
	// memory (Apple silicon); managed on a discrete GPU, where Metal keeps a CPU copy and mirrors it to the GPU.
	auto storageMode = MTL::StorageModePrivate;
	if (hasInitialData)
		storageMode = nativeDevice->hasUnifiedMemory() ? MTL::StorageModeShared : MTL::StorageModeManaged;

	const auto textureDescriptor = NS::TransferPtr(MTL::TextureDescriptor::alloc()->init());
	textureDescriptor->setWidth(_width);
	textureDescriptor->setHeight(_height);
	textureDescriptor->setPixelFormat(ToMetal(_format));
	textureDescriptor->setUsage(ToMetal(_usage));
	textureDescriptor->setTextureType(MTL::TextureType2D);
	textureDescriptor->setMipmapLevelCount(1);
	textureDescriptor->setStorageMode(storageMode);

	_nativeTexture = NS::TransferPtr(nativeDevice->newTexture(textureDescriptor.get()));
	if (not _nativeTexture) [[unlikely]] {
		const auto error = std::format("MetalTexture::MetalTexture: Could not allocate a {}x{} texture!", _width, _height);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	// Before the texture is handed out, so no command buffer can be reading it while it is written: the one moment a
	// CPU write needs no synchronisation with the frames in flight.
	if (hasInitialData) {
		const auto bytesPerRow = static_cast<NS::UInteger>(_width) * Types::GetBytesPerPixel(_format);
		_nativeTexture->replaceRegion(MTL::Region::Make2D(0, 0, _width, _height), 0, descriptor.initialData.data(), bytesPerRow);
	}

	if (not descriptor.debugName.empty()) {
		// NS::String::string reads up to a NUL, which a string_view does not promise: copy it into one that does.
		const std::string label{descriptor.debugName};
		_nativeTexture->setLabel(NS::String::string(label.c_str(), NS::UTF8StringEncoding));
	}
}

MetalTexture::~MetalTexture() = default;

}
