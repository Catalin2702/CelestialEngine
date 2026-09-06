//
// Module: CelestialEngine/Engine/Modules/Core/Render/Texture/Platforms/Mac/Metal
// File: MetalTexture.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#pragma once

#ifndef CE_CORE_RENDER_TEXTURE_METALTEXTURE_HPP
#define CE_CORE_RENDER_TEXTURE_METALTEXTURE_HPP

#include "Core/Render/Texture/I_Texture.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <Foundation/Foundation.hpp>

namespace MTL {
	class Device;
	class Texture;
}

/**
 * @class MetalTexture
 * @brief An MTL::Texture allocated in device-private memory
 * @details Private storage always, even on unified memory: shared would mean the CPU could map it, and nothing ever
 *			wants to. Private is what lets the driver pick the layout the GPU reads fastest, which for a render target
 *			read back by a full-screen pass is the whole point.
 */
namespace CE::Core {

class CE_CORE_API MetalTexture: public I_TextureBase<Types::GraphicsApi::Metal> {
public:
	/**
	 * @brief Allocates the texture described by the descriptor
	 * @details Throws std::runtime_error when the allocation fails: at render-target sizes that only happens when the
	 *			device is out of memory, and there is nothing sensible to draw into afterwards.
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

#endif //CE_CORE_RENDER_TEXTURE_METALTEXTURE_HPP
