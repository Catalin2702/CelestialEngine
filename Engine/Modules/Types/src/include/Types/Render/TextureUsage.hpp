//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: TextureUsage.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_TYPES_RENDER_TEXTUREUSAGE_HPP
#define CE_CORE_TYPES_RENDER_TEXTUREUSAGE_HPP

#include "Define/Types.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


namespace CE::Types {

/**
 * @enum TextureUsage
 * @brief What a texture is allowed to be used for
 * @details Asked up front because the explicit APIs bake it into the allocation: Metal picks a different memory
 *			layout for a texture that will be rendered into than for one that is only sampled, and Vulkan refuses a
 *			view whose usage was not declared. OpenGL has no equivalent and ignores all of this - which is exactly why
 *			it is the backend where a missing flag goes unnoticed until the Metal build fails.
 */
enum class TextureUsage: u8 {
	None = 0,
	ShaderRead = BIT(0), ///< Can be read by a shader - the composite pass needs this on the scene colour
	ShaderWrite = BIT(1), ///< Can be written by a compute shader
	RenderTarget = BIT(2), ///< Can be a colour or depth attachment of a render pass
};

constexpr TextureUsage operator & (TextureUsage x, TextureUsage y) {
	return static_cast<TextureUsage>(static_cast<u8>(x) & static_cast<u8>(y));
}

constexpr TextureUsage operator | (TextureUsage x, TextureUsage y) {
	return static_cast<TextureUsage>(static_cast<u8>(x) | static_cast<u8>(y));
}

constexpr TextureUsage operator ^ (TextureUsage x, TextureUsage y) {
	return static_cast<TextureUsage>(static_cast<u8>(x) ^ static_cast<u8>(y));
}

constexpr bool HasAnyFlags(const TextureUsage x, const TextureUsage y) {
	return (x & y) != TextureUsage::None;
}

}

/**
 * @brief Names a TextureUsage, for fmt/spdlog and - through the formatter below - for std::format
 * @details Single flags only, like GraphicsApi: a combination names itself "Combined" rather than spelling itself
 *			out, because the only place this is printed is an allocation failure.
 */
constexpr std::string_view format_as(const CE::Types::TextureUsage textureUsage) {
	switch (textureUsage) {
		case CE::Types::TextureUsage::None: return "None";
		case CE::Types::TextureUsage::ShaderRead: return "ShaderRead";
		case CE::Types::TextureUsage::ShaderWrite: return "ShaderWrite";
		case CE::Types::TextureUsage::RenderTarget: return "RenderTarget";
		default: return "Combined";
	}
}

template <>
struct std::formatter<CE::Types::TextureUsage>: std::formatter<std::string_view> {
	auto format(const CE::Types::TextureUsage value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //
