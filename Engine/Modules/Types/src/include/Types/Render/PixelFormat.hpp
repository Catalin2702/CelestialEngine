//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: PixelFormat.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_RENDER_PIXELFORMAT_HPP
#define CE_TYPES_RENDER_PIXELFORMAT_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


namespace CE::Types {

/**
 * @enum PixelFormat
 * @brief Memory layout of a single texel in a texture or render target
 * @details Metal, DirectX 12 and Vulkan bake the format of every attachment into the compiled pipeline, so a
 *			pipeline is only valid for the render pass it was created against. That is why this type belongs in a
 *			pipeline descriptor rather than in a texture alone, even though OpenGL and DirectX 11 never ask for it.
 *
 *			Three-component 8-bit formats (an RGB8 with no alpha) are deliberately absent: they are an OpenGL-only
 *			convenience, and neither Metal nor DirectX can use one as a render target. Pad to RGBA8 instead.
 */
enum class PixelFormat: u8 {
	None = 0,					///< No format specified

	// --- 8 bits per channel, normalised to [0, 1] in the shader ---
	R8Unorm,					///< single channel, masks and coverage
	RG8Unorm,					///< two channels
	RGBA8Unorm,					///< the portable colour default
	RGBA8UnormSrgb,				///< as above, with the sRGB transfer function applied by the hardware
	BGRA8Unorm,					///< channel order preferred by CAMetalLayer and DXGI swapchains
	BGRA8UnormSrgb,				///< as above, sRGB

	// --- floating point, for HDR and intermediate targets ---
	R16Float,
	RG16Float,
	RGBA16Float,				///< the usual HDR render target
	R32Float,
	RG32Float,
	RGBA32Float,

	// --- packed ---
	RGB10A2Unorm,				///< 10 bits per colour channel, 2 of alpha - wide-gamut presentation
	RG11B10Float,				///< no alpha, cheap HDR - half the footprint of RGBA16Float

	// --- depth and stencil ---
	Depth16Unorm,
	Depth32Float,				///< the depth default: no precision cliff, supported everywhere
	Depth24UnormStencil8,		///< NOT guaranteed on Apple silicon - query before using it
	Depth32FloatStencil8,		///< the portable choice when a stencil buffer is needed
};

/**
 * @brief Checks whether a format can be used as a depth attachment
 * @param pixelFormat The PixelFormat to check
 * @return bool True for the Depth* formats, false otherwise
 */
CE_TYPES_API bool IsDepthFormat(PixelFormat pixelFormat);

/**
 * @brief Checks whether a format carries a stencil component alongside its depth
 * @param pixelFormat The PixelFormat to check
 * @return bool True for Depth24UnormStencil8 and Depth32FloatStencil8, false otherwise
 */
CE_TYPES_API bool HasStencil(PixelFormat pixelFormat);

/**
 * @brief Returns the size in bytes of one texel in the given format
 * @param pixelFormat The PixelFormat to measure
 * @return u32 Bytes per texel, or 0 for PixelFormat::None
 * @details Only valid for the uncompressed formats listed here - a block-compressed format has no meaningful
 *			per-texel size, which is one reason none are in this enum yet.
 */
CE_TYPES_API u32 GetBytesPerPixel(PixelFormat pixelFormat);

/**
 * @brief Names a PixelFormat, for fmt/spdlog and - through the formatter below - for std::format
 * @param pixelFormat The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const PixelFormat pixelFormat) {
	switch (pixelFormat) {
		case PixelFormat::None: return "None";
		case PixelFormat::R8Unorm: return "R8Unorm";
		case PixelFormat::RG8Unorm: return "RG8Unorm";
		case PixelFormat::RGBA8Unorm: return "RGBA8Unorm";
		case PixelFormat::RGBA8UnormSrgb: return "RGBA8UnormSrgb";
		case PixelFormat::BGRA8Unorm: return "BGRA8Unorm";
		case PixelFormat::BGRA8UnormSrgb: return "BGRA8UnormSrgb";
		case PixelFormat::R16Float: return "R16Float";
		case PixelFormat::RG16Float: return "RG16Float";
		case PixelFormat::RGBA16Float: return "RGBA16Float";
		case PixelFormat::R32Float: return "R32Float";
		case PixelFormat::RG32Float: return "RG32Float";
		case PixelFormat::RGBA32Float: return "RGBA32Float";
		case PixelFormat::RGB10A2Unorm: return "RGB10A2Unorm";
		case PixelFormat::RG11B10Float: return "RG11B10Float";
		case PixelFormat::Depth16Unorm: return "Depth16Unorm";
		case PixelFormat::Depth32Float: return "Depth32Float";
		case PixelFormat::Depth24UnormStencil8: return "Depth24UnormStencil8";
		case PixelFormat::Depth32FloatStencil8: return "Depth32FloatStencil8";
		default: return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::Types::PixelFormat>: std::formatter<std::string_view> {
	auto format(const CE::Types::PixelFormat value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_RENDER_PIXELFORMAT_HPP
