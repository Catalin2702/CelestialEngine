//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: PixelFormat.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include "Types/Render/PixelFormat.hpp"


namespace CE::Types {

bool IsDepthFormat(const PixelFormat pixelFormat) {
	switch (pixelFormat) {
		case PixelFormat::Depth16Unorm:
		case PixelFormat::Depth32Float:
		case PixelFormat::Depth24UnormStencil8:
		case PixelFormat::Depth32FloatStencil8:
			return true;
		default:
			return false;
	}
}

bool HasStencil(const PixelFormat pixelFormat) {
	return pixelFormat == PixelFormat::Depth24UnormStencil8 or pixelFormat == PixelFormat::Depth32FloatStencil8;
}

u32 GetBytesPerPixel(const PixelFormat pixelFormat) {
	switch (pixelFormat) {
		case PixelFormat::None:
			return 0;

		case PixelFormat::R8Unorm:
			return 1;

		case PixelFormat::RG8Unorm:
		case PixelFormat::R16Float:
		case PixelFormat::Depth16Unorm:
			return 2;

		case PixelFormat::RGBA8Unorm:
		case PixelFormat::RGBA8UnormSrgb:
		case PixelFormat::BGRA8Unorm:
		case PixelFormat::BGRA8UnormSrgb:
		case PixelFormat::RG16Float:
		case PixelFormat::R32Float:
		case PixelFormat::RGB10A2Unorm:
		case PixelFormat::RG11B10Float:
		case PixelFormat::Depth32Float:
		case PixelFormat::Depth24UnormStencil8:
			return 4;

		// The driver pads this one to 8 bytes: depth and stencil live in separate planes, so the footprint is
		// the 4 bytes of depth plus a byte of stencil, rounded up.
		case PixelFormat::Depth32FloatStencil8:
			return 8;

		case PixelFormat::RGBA16Float:
		case PixelFormat::RG32Float:
			return 8;

		case PixelFormat::RGBA32Float:
			return 16;

		default:
			return 0;
	}
}

}
