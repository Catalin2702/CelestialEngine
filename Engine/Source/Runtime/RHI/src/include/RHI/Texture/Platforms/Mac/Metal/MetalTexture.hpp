//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Texture/Platforms/Mac/Metal
// File: MetalTexture.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef CE_RHI_TEXTURE_METALTEXTURE_HPP
#define CE_RHI_TEXTURE_METALTEXTURE_HPP

#include "RHI/Texture/I_Texture.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <Foundation/Foundation.hpp>

namespace MTL {
class Device;
class Texture;
}

/**
 * @class MetalTexture
 * @brief An MTL::Texture: private for what the GPU fills, CPU-visible for an image uploaded at creation
 * @details Private whenever nothing is uploaded: shared would mean the CPU could map it, and nothing ever wants to.
 *			Private is what lets the driver pick the layout the GPU reads fastest, which for a render target read back
 *			by a full-screen pass is the whole point. A texture created with initial data cannot be private - the CPU
 *			writes it - so it takes the storage mode the device lets the CPU reach: shared on unified memory, managed
 *			on a discrete GPU.
 */
namespace CE::RHI {

class CE_RHI_API MetalTexture: public I_TextureBase<Types::GraphicsApi::Metal> {
public:
	/**
	 * @brief Allocates the texture described by the descriptor
	 * @details Throws std::runtime_error when the allocation fails - at render-target sizes that only happens when the
	 *			device is out of memory - or when the initial data does not fill the texture exactly.
	 */
	MetalTexture(MTL::Device* nativeDevice, const TextureDescriptor& descriptor);

	MetalTexture(const MetalTexture&) = delete;
	MetalTexture(MetalTexture&&) noexcept = default;

	/// Out of line because NS::SharedPtr has to see the full MTL::Texture to release it.
	~MetalTexture() override;

public:
	MetalTexture& operator = (const MetalTexture&) = delete;
	MetalTexture& operator = (MetalTexture&&) noexcept = default;

public:
	[[nodiscard]] u32 GetWidth() const override { return _width; }
	[[nodiscard]] u32 GetHeight() const override { return _height; }
	[[nodiscard]] std::pair<u32, u32> GetSize() const override { return {_width, _height}; }
	[[nodiscard]] Types::PixelFormat GetFormat() const override { return _format; }
	[[nodiscard]] Types::TextureUsage GetUsage() const override { return _usage; }

public:
	[[nodiscard]] MTL::Texture* GetTexture() const { return _nativeTexture.get(); }

private:
	NS::SharedPtr<MTL::Texture> _nativeTexture;

	u32 _width = 0;
	u32 _height = 0;

	Types::PixelFormat _format = Types::PixelFormat::None;
	Types::TextureUsage _usage = Types::TextureUsage::None;
};

}

#endif //CE_RHI_TEXTURE_METALTEXTURE_HPP
