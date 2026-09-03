//
// Module: CelestialEngine/Engine/Modules/Types/Render/Platforms/Common/OpenGl
// File: OpenGl.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-24
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_RENDER_OPENGL_HPP
#define CE_TYPES_RENDER_OPENGL_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string>
#include <string_view>


namespace CE::Types {

/**
 * @brief Bit flags for the OpenGL framebuffer attachments that can be cleared
 * @details Values mirror the corresponding GL_*_BUFFER_BIT constants so they can be passed to glClear directly.
 */
enum class BufferBit: u32 {
	Color = 0x00004000,		///< GL_COLOR_BUFFER_BIT
	Depth = 0x00000100,		///< GL_DEPTH_BUFFER_BIT
	Stencil = 0x00000400,	///< GL_STENCIL_BUFFER_BIT
};

/**
 * @brief Converts a BufferBit flag to its underlying integer value (the GL bitmask)
 */
CE_TYPES_API int ToInt(BufferBit bit);

constexpr BufferBit operator & (BufferBit x, BufferBit y) {
	return static_cast<BufferBit>(static_cast<u32>(x) & static_cast<u32>(y));
}

constexpr BufferBit operator | (BufferBit x, BufferBit y) {
	return static_cast<BufferBit>(static_cast<u32>(x) | static_cast<u32>(y));
}

constexpr BufferBit operator ^ (BufferBit x, BufferBit y) {
	return static_cast<BufferBit>(static_cast<u32>(x) ^ static_cast<u32>(y));
}

/**
 * @brief Names the bits set in a BufferBit mask, for fmt/spdlog and - through the formatter below - for std::format
 * @param bufferBit The mask to name
 * @return std::string A '|'-separated list of the bits set, or "None" when no known bit is
 * @details Returns an owning string, unlike the other enums here: a mask is a set, so "Color|Depth" has to be built
 *			rather than looked up. The temporary lives to the end of the full expression that formats it, which is
 *			all the formatter below needs.
 */
inline std::string format_as(const BufferBit bufferBit) {
	std::string name;

	const auto append = [&name](const BufferBit bit, const char* text) {
		if (static_cast<u32>(bit) == 0)
			return;
		if (not name.empty())
			name += '|';
		name += text;
	};

	append(bufferBit & BufferBit::Color, "Color");
	append(bufferBit & BufferBit::Depth, "Depth");
	append(bufferBit & BufferBit::Stencil, "Stencil");

	return name.empty() ? "None" : name;
}

}

template <>
struct std::formatter<CE::Types::BufferBit>: std::formatter<std::string_view> {
	auto format(const CE::Types::BufferBit value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_RENDER_OPENGL_HPP
