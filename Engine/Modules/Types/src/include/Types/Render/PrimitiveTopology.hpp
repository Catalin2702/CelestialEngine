//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: PrimitiveTopology.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_RENDER_PRIMITIVETOPOLOGY_HPP
#define CE_TYPES_RENDER_PRIMITIVETOPOLOGY_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


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
	None,
	PointList,		///< one point per vertex
	LineList,		///< one line per vertex pair, no sharing
	LineStrip,		///< each vertex after the first extends the line
	TriangleList,	///< one triangle per vertex triple - the default, and what an index buffer usually feeds
	TriangleStrip,	///< each vertex after the second adds a triangle, winding alternating every step
};

/**
 * @brief Returns how many primitives a given number of vertices produces under this topology
 * @param topology The PrimitiveTopology in use
 * @param vertexCount Number of vertices (or indices, when drawing indexed) submitted
 * @return u32 The resulting primitive count, or 0 when there are too few vertices to form one
 * @details Useful for draw-call statistics and for asserting that an index count is well formed - a TriangleList
 *			with an index count that is not a multiple of three is a bug every backend will render silently.
 */
CE_TYPES_API u32 GetPrimitiveCount(PrimitiveTopology topology, u32 vertexCount);

/**
 * @brief Converts a PrimitiveTopology to the mode constant the glDraw* calls expect
 * @details Unlike the rest of the pipeline state, topology is not something OpenGL stores: it is an argument of every
 *			draw call, so the pipeline only exposes it and the command encoder passes it on.
 */
CE_TYPES_API u32 ToOpenGl(PrimitiveTopology topology);

/**
 * @brief Names a PrimitiveTopology, for fmt/spdlog and - through the formatter below - for std::format
 * @param topology The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const PrimitiveTopology topology) {
	switch (topology) {
		case PrimitiveTopology::None: return "None";
		case PrimitiveTopology::PointList: return "PointList";
		case PrimitiveTopology::LineList: return "LineList";
		case PrimitiveTopology::LineStrip: return "LineStrip";
		case PrimitiveTopology::TriangleList: return "TriangleList";
		case PrimitiveTopology::TriangleStrip: return "TriangleStrip";
		default: return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::Types::PrimitiveTopology>: std::formatter<std::string_view> {
	auto format(const CE::Types::PrimitiveTopology value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_RENDER_PRIMITIVETOPOLOGY_HPP
