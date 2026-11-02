//
// Module: CelestialEngine/Engine/Modules/Types/Render/Platforms/Common/OpenGl
// File: OpenGl.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#pragma once

#ifndef CE_TYPES_RENDER_OPENGL_HPP
#define CE_TYPES_RENDER_OPENGL_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"
#include "Types/Render/BlendFactor.hpp"
#include "Types/Render/Compare.hpp"
#include "Types/Render/CullMode.hpp"
#include "Types/Render/PixelFormat.hpp"
#include "Types/Render/PrimitiveTopology.hpp"
#include "Types/Render/Shader.hpp"
#include "Types/Render/ShaderConstants.hpp"

#include <format>
#include <string>
#include <string_view>


namespace CE::Types {

inline constexpr u32 OPENGL_FRAGMENT_CONSTANTS_FIRST_BINDING = MAX_CONSTANT_SLOTS;

inline constexpr u32 OPENGL_UNIFORM_BUFFER_BINDINGS = 2 * MAX_CONSTANT_SLOTS;

/**
 * @brief Bit flags for the OpenGL framebuffer attachments that can be cleared
 * @details Values mirror the corresponding GL_*_BUFFER_BIT constants so they can be passed to glClear directly.
 */
enum class BufferBit: u16 {
	None = 0,				///< No buffer, the empty mask
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

struct GlFormat {
	i32 internalFormat;
	u32 format;
	u32 type;
};

[[nodiscard]] CE_TYPES_API GlFormat ToOpenGl(PixelFormat pixelFormat);

CE_TYPES_API u32 ToOpenGl(BlendFactor blendFactor);

CE_TYPES_API u32 ToOpenGl(CompareFunc compareFunc);

/**
 * @brief Converts a CullMode to the face constant glCullFace expects
 * @return u32 GL_FRONT or GL_BACK; GL_BACK for None, which has no face of its own
 * @details CullMode::None is not a face but the absence of culling: the caller must glDisable(GL_CULL_FACE) for it
 *			rather than pass the returned value on.
 */
CE_TYPES_API u32 ToOpenGl(CullMode cullMode);

/**
 * @brief Converts a FrontFace to the winding constant glFrontFace expects
 */
CE_TYPES_API u32 ToOpenGl(FrontFace frontFace);

/**
 * @brief Converts a PrimitiveTopology to the mode constant the glDraw* calls expect
 * @details Unlike the rest of the pipeline state, topology is not something OpenGL stores: it is an argument of every
 *			draw call, so the pipeline only exposes it and the command encoder passes it on.
 */
CE_TYPES_API u32 ToOpenGl(PrimitiveTopology topology);

/**
 * @brief Converts a ShaderType enum value to the corresponding OpenGL shader type constant
 * @param type The ShaderType to convert
 * @return int The OpenGL shader type constant corresponding to the given ShaderType, or -1 if the ShaderType is unknown or unsupported
 * @details This function maps the ShaderType enum values to their corresponding OpenGL shader type constants (e.g., GL_VERTEX_SHADER, GL_FRAGMENT_SHADER). This is essential for compiling and using shaders in an OpenGL context. If the provided ShaderType does not have a corresponding OpenGL constant, the function returns -1 to indicate an error.
 */
CE_TYPES_API int ToOpenGlShaderType(ShaderType type);

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
