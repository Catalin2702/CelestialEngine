//
// Module: CelestialEngine/Engine/Modules/Core/Render/Texture/Platforms/Mac/Metal
// File: MetalTexture.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#include "Core/Render/Texture/Platforms/Mac/Metal/MetalTexture.hpp"
#include "Tools/Tools.hpp"

#include <Metal/Metal.hpp>

#include <stdexcept>
#include <format>

namespace CE::Core {

MetalTexture::MetalTexture(MTL::Device* nativeDevice, const TextureDescriptor& descriptor):
	_width(descriptor.width), _height(descriptor.height), _format(descriptor.format), _usage(descriptor.usage)
{
	if (not nativeDevice or _width == 0 or _height == 0) [[unlikely]] {
		constexpr auto error = "MetalTexture::MetalTexture: A texture needs a device and a non-zero size!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	const auto textureDescriptor = NS::TransferPtr(MTL::TextureDescriptor::alloc()->init());
	textureDescriptor->setWidth(_width);
	textureDescriptor->setHeight(_height);
	textureDescriptor->setPixelFormat(Types::ToMetal(_format));
	textureDescriptor->setUsage(Types::ToMetal(_usage));
	textureDescriptor->setTextureType(MTL::TextureType2D);
	textureDescriptor->setMipmapLevelCount(1);
	textureDescriptor->setStorageMode(MTL::StorageModePrivate);

	_nativeTexture = NS::TransferPtr(nativeDevice->newTexture(textureDescriptor.get()));
	if (not _nativeTexture) [[unlikely]] {
		const auto error = std::format("MetalTexture::MetalTexture: Could not allocate a {}x{} texture!", _width, _height);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	if (not descriptor.debugName.empty()) {
		_nativeTexture->setLabel(NS::String::string(descriptor.debugName.data(), NS::UTF8StringEncoding));
	}
}

MetalTexture::~MetalTexture() = default;

}
