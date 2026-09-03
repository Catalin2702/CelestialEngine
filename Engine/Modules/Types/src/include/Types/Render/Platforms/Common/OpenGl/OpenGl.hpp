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

}

#endif //CE_TYPES_RENDER_OPENGL_HPP
