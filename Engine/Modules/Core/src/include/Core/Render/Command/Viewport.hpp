//
// Module: CelestialEngine/Engine/Modules/Core/Render/Command
// File: Viewport.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-09-18
//

#pragma once

#ifndef CE_CORE_RENDER_COMMAND_VIEWPORT_HPP
#define CE_CORE_RENDER_COMMAND_VIEWPORT_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"


namespace CE::Core {

/**
 * @struct Viewport
 * @brief The region of the render target the NDC cube is mapped onto
 * @details Not a clipping rectangle - that is the scissor, which is set separately. The viewport is the affine
 *			transform applied after the perspective divide: it turns normalised device coordinates into target pixels.
 *
 *			One convention, stated once: the origin is the **top-left** corner of the target and y grows downwards,
 *			the way a window reports positions. Backends that measure from the other corner convert on the way in,
 *			inside the encoder that receives it - so a Viewport is a plain rectangle that belongs to no backend and
 *			can be handed to any encoder.
 */
struct CE_CORE_API Viewport {
	f32 x = 0.f;
	f32 y = 0.f;			///< Top edge, measured downwards from the top of the target
	f32 width = 0.f;
	f32 height = 0.f;

	/// Range the NDC z lands in inside the depth buffer. Almost always 0..1; swapped for reversed-Z, and narrowed to
	/// split the depth range between layered passes.
	f32 minDepth = 0.f;
	f32 maxDepth = 1.f;
};

}

#endif //CE_CORE_RENDER_COMMAND_VIEWPORT_HPP
