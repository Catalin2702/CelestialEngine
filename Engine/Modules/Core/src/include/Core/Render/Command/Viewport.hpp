//
// Module: CelestialEngine/Engine/Modules/Core/Render/Command
// File: Viewport.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#pragma once

#ifndef CE_CORE_RENDER_COMMAND_VIEWPORT_HPP
#define CE_CORE_RENDER_COMMAND_VIEWPORT_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Render/Render.hpp"
#include "Types/Var/Vars.hpp"


namespace CE::Core {

/**
 * @struct Viewport
 * @brief The region of the render target the NDC cube is mapped onto
 * @details Not a clipping rectangle - that is the scissor, which is set separately. The viewport is the affine
 *			transform applied after the perspective divide: it turns normalised device coordinates into target pixels.
 *
 *			Callers describe the rectangle in the convention every API but OpenGL uses - origin at the **top-left**
 *			corner, y growing downwards, which is also the convention the window events report positions in - and the
 *			constructor converts it into the backend's own. The converted values are what the members hold, so a
 *			Viewport is bound to the API it was built for: handing one built for OpenGL to a Metal encoder would draw
 *			in the wrong half of the target. GetGraphicsApi() exists so an encoder can assert against that.
 */
struct CE_CORE_API Viewport {
public:
	Viewport() = default;

	/**
	 * @brief Builds a viewport for one backend, converting the rectangle into that backend's convention
	 * @param graphicsApi The API the viewport is destined for; decides which conversion rules apply
	 * @param x Left edge, in target pixels
	 * @param y Top edge, in target pixels, measured downwards from the top of the target
	 * @param width Width in target pixels
	 * @param height Height in target pixels
	 * @param targetHeight Full height of the render target; only OpenGL needs it, to flip the origin
	 * @param minDepth Depth value the near plane maps to, normally 0
	 * @param maxDepth Depth value the far plane maps to, normally 1 (or 0 under a reversed-Z projection)
	 */
	Viewport(Types::GraphicsApi graphicsApi, f32 x, f32 y, f32 width, f32 height, f32 targetHeight, f32 minDepth = 0.f, f32 maxDepth = 1.f);

public:
	/**
	 * @brief Gets the API this viewport's coordinates were converted for
	 */
	[[nodiscard]] Types::GraphicsApi GetGraphicsApi() const { return _graphicsApi; }

public:
	f32 x = 0.f;
	f32 y = 0.f;
	f32 width = 0.f;
	f32 height = 0.f;

	/// Range the NDC z lands in inside the depth buffer. Almost always 0..1; swapped for reversed-Z, and narrowed to
	/// split the depth range between layered passes.
	f32 minDepth = 0.f;
	f32 maxDepth = 1.f;

private:
	Types::GraphicsApi _graphicsApi = Types::GraphicsApi::None;
};

}

#endif //CE_CORE_RENDER_COMMAND_VIEWPORT_HPP
