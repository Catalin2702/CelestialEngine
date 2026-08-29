//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: PrimitiveTopology.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_TYPES_RENDER_PRIMITIVETOPOLOGY_HPP
#define CE_TYPES_RENDER_PRIMITIVETOPOLOGY_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"


namespace CE::Types {

/**
 * @enum PrimitiveTopology
 * @brief How a stream of vertices is assembled into primitives
 * @details Five entries, and that is not an abridged list - it is everything the five backends agree on.
 *			OpenGL's LineLoop and the triangle fan are both missing on purpose: LineLoop exists only in OpenGL,
 *			and the fan was dropped by DirectX 12 and never existed in Metal, so neither can be expressed
 *			portably. A fan is trivially re-indexed as a TriangleList at mesh build time; a loop is a LineStrip
 *			whose first vertex is repeated at the end.
 *
 *			Note that Metal and Vulkan take the topology at draw time while DirectX 12 and Vulkan also want the
 *			topology *class* at pipeline creation - hence its place in the pipeline descriptor rather than as a
 *			parameter to DrawIndexed.
 */
enum class PrimitiveTopology: u8 {
	PointList,		///< one point per vertex
	LineList,		///< one line per vertex pair, no sharing
	LineStrip,		///< each vertex after the first extends the line
	TriangleList,	///< one triangle per vertex triple - the default, and what an index buffer usually feeds
	TriangleStrip,	///< each vertex after the second adds a triangle, winding alternating every step
};

/**
 * @brief Converts a PrimitiveTopology enum value to its string representation
 * @param topology The PrimitiveTopology to convert to a string
 * @return const char* A string representation of the PrimitiveTopology, or "Unknown" if the value is out of range
 */
CE_TYPES_API const char* ToString(PrimitiveTopology topology);

/**
 * @brief Returns how many primitives a given number of vertices produces under this topology
 * @param topology The PrimitiveTopology in use
 * @param vertexCount Number of vertices (or indices, when drawing indexed) submitted
 * @return u32 The resulting primitive count, or 0 when there are too few vertices to form one
 * @details Useful for draw-call statistics and for asserting that an index count is well formed - a TriangleList
 *			with an index count that is not a multiple of three is a bug every backend will render silently.
 */
CE_TYPES_API u32 GetPrimitiveCount(PrimitiveTopology topology, u32 vertexCount);

}

#endif //CE_TYPES_RENDER_PRIMITIVETOPOLOGY_HPP
