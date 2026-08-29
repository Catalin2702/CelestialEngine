//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: CullMode.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_TYPES_RENDER_CULLMODE_HPP
#define CE_TYPES_RENDER_CULLMODE_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"


namespace CE::Types {

/**
 * @enum FrontFace
 * @brief Winding order that marks a triangle as front-facing
 * @details Separate from CullMode because it answers a different question: CullMode says which side to discard,
 *			FrontFace says which side is which. Splitting them is what every API does, and merging them would
 *			make "cull the back face of a clockwise mesh" inexpressible.
 */
enum class FrontFace: u8 {
	CounterClockwise,	///< the default in OpenGL, Vulkan and Metal
	Clockwise,			///< the default in DirectX
};

/**
 * @enum CullMode
 * @brief Which side of a triangle is discarded before rasterisation
 * @details Three entries only. OpenGL's GL_FRONT_AND_BACK has no equivalent in DirectX 12 or Metal, so an
 *			"cull everything" mode cannot be expressed portably - and it is not a loss, since not issuing the
 *			draw call is both clearer and cheaper.
 *
 *			Metal takes this on the render command encoder while the other four bake it into the pipeline, which
 *			is why it lives in the pipeline descriptor: the descriptor is the union of what each backend needs to
 *			know, and Metal simply reads it back out at encode time.
 */
enum class CullMode: u8 {
	None,	///< both sides rasterise - what a double-sided material wants
	Front,	///< discard front-facing triangles
	Back,	///< discard back-facing triangles - the usual choice for closed meshes
};

/**
 * @brief Converts a CullMode enum value to its string representation
 * @param cullMode The CullMode to convert to a string
 * @return const char* A string representation of the CullMode, or "Unknown" if the value is out of range
 */
CE_TYPES_API const char* ToString(CullMode cullMode);

/**
 * @brief Converts a FrontFace enum value to its string representation
 * @param frontFace The FrontFace to convert to a string
 * @return const char* A string representation of the FrontFace, or "Unknown" if the value is out of range
 */
CE_TYPES_API const char* ToString(FrontFace frontFace);

}

#endif //CE_TYPES_RENDER_CULLMODE_HPP
