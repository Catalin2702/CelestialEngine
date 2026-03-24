//
// Module: CelestialEngine/Engine/Modules/Types/Render/Platforms/Common
// File: OpenGl.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-24
// Updated by: Catalin Chirosca
// Updated: 2026-03-24
//

#pragma once

#ifndef CE_TYPES_RENDER_PLATFORMS_COMMON_OPENGL_HPP
#define CE_TYPES_RENDER_PLATFORMS_COMMON_OPENGL_HPP

#include <cstdint>

namespace CE::Types::Render {

enum class BufferBit: uint32_t {
	Color = 0x00004000,		///< GL_COLOR_BUFFER_BIT
	Depth = 0x00000100,		///< GL_DEPTH_BUFFER_BIT
	Stencil = 0x00000400,	///< GL_STENCIL_BUFFER_BIT
};

int ToInt(BufferBit bit);

}

#endif //CE_TYPES_RENDER_PLATFORMS_COMMON_OPENGL_HPP
