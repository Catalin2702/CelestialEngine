//
// Module: CelestialEngine/Engine/Source/Runtime/Types/Render
// File: Sampler.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-27
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef CE_TYPES_RENDER_SAMPLER_HPP
#define CE_TYPES_RENDER_SAMPLER_HPP

#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


namespace CE::Types {

/**
 * @enum FilterMode
 * @brief How a sample between texel centres is computed
 * @details The same two choices for magnification (a texel covers several pixels) and minification (a pixel covers
 *			several texels). Mipmap filtering is a third axis and is absent until a texture can have mip levels: a
 *			mipmapped filter on a one-level texture is exactly what makes OpenGL sample black.
 */
enum class FilterMode: u8 {
	Nearest, ///< The closest texel, unblended - hard edges, pixel art
	Linear, ///< The four closest texels, weighted by distance - smooth, the default
};

/**
 * @enum AddressMode
 * @brief What a coordinate outside [0, 1] reads
 * @details Only the modes every backend has. Metal's ClampToZero and border colours have no OpenGL 4.1 equivalent
 *			for every format, so they stay out until a backend needs them.
 */
enum class AddressMode: u8 {
	ClampToEdge, ///< The edge texel, repeated outwards - the right choice for a single image on a quad
	Repeat, ///< The image tiled - floors, walls
	MirrorRepeat, ///< Tiled, every other copy mirrored - hides the seam of a texture that does not tile
};

constexpr std::string_view format_as(const FilterMode filterMode) {
	switch (filterMode) {
		case FilterMode::Nearest: return "Nearest";
		case FilterMode::Linear: return "Linear";
	}
	return "Invalid"; // Only a value cast from outside the enum gets here.
}

constexpr std::string_view format_as(const AddressMode addressMode) {
	switch (addressMode) {
		case AddressMode::ClampToEdge: return "ClampToEdge";
		case AddressMode::Repeat: return "Repeat";
		case AddressMode::MirrorRepeat: return "MirrorRepeat";
	}
	return "Invalid"; // Only a value cast from outside the enum gets here.
}

}

template <>
struct std::formatter<CE::Types::FilterMode>: std::formatter<std::string_view> {
	auto format(const CE::Types::FilterMode value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

template <>
struct std::formatter<CE::Types::AddressMode>: std::formatter<std::string_view> {
	auto format(const CE::Types::AddressMode value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_RENDER_SAMPLER_HPP
