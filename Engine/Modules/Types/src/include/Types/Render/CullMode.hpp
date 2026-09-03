//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: CullMode.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_TYPES_RENDER_CULLMODE_HPP
#define CE_TYPES_RENDER_CULLMODE_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


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
 * @brief Names a CullMode, for fmt/spdlog and - through the formatter below - for std::format
 * @param cullMode The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const CullMode cullMode) {
	switch (cullMode) {
		case CullMode::None: return "None";
		case CullMode::Front: return "Front";
		case CullMode::Back: return "Back";
		default: return "Unknown";
	}
}

/**
 * @brief Names a FrontFace, for fmt/spdlog and - through the formatter below - for std::format
 * @param frontFace The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const FrontFace frontFace) {
	switch (frontFace) {
		case FrontFace::CounterClockwise: return "CounterClockwise";
		case FrontFace::Clockwise: return "Clockwise";
		default: return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::Types::CullMode>: std::formatter<std::string_view> {
	auto format(const CE::Types::CullMode value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

template <>
struct std::formatter<CE::Types::FrontFace>: std::formatter<std::string_view> {
	auto format(const CE::Types::FrontFace value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_RENDER_CULLMODE_HPP
