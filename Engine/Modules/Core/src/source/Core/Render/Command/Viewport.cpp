//
// Module: CelestialEngine/Engine/Modules/Core/Render/Command
// File: Viewport.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#include "Core/Render/Command/Viewport.hpp"
#include "Tools/Log/Log.hpp"


namespace CE::Core {

Viewport::Viewport(const Types::GraphicsApi graphicsApi, const f32 x, const f32 y, const f32 width, const f32 height,
	const f32 targetHeight, const f32 minDepth, const f32 maxDepth):
	x(x), y(y), width(width), height(height), minDepth(minDepth), maxDepth(maxDepth), _graphicsApi(graphicsApi) {

	// No default label on purpose: adding a graphics API must fail this switch under -Wswitch rather than silently
	// inherit the pass-through rule, because "does this API flip y?" is exactly the question a new backend has to answer.
	switch (graphicsApi) {
		case Types::GraphicsApi::OpenGL:
			// OpenGL is the only one whose window origin is the bottom-left corner, so the top edge the caller gave us
			// has to be measured from the other end of the target.
			if (targetHeight <= 0.f) [[unlikely]] {
				CE_CORE_WARN("Viewport: an OpenGL viewport needs the render target's height to flip the origin, but {} was given; the y coordinate is left unflipped.", targetHeight);
				break;
			}
			this->y = targetHeight - y - height;
			break;

		// Origin already at the top-left with y growing downwards: the rectangle is passed through untouched.
		// Vulkan can additionally be driven with a negative height to flip clip space, but that changes the winding
		// the whole pipeline is built around, so it is a projection-matrix decision rather than a viewport one.
		case Types::GraphicsApi::Metal:
		case Types::GraphicsApi::DirectX11:
		case Types::GraphicsApi::DirectX12:
		case Types::GraphicsApi::Vulkan:
			break;

		case Types::GraphicsApi::None:
			CE_CORE_WARN("Viewport: built with GraphicsApi::None; the rectangle is left in the caller's convention and may not match any backend.");
			break;
	}
}

}
