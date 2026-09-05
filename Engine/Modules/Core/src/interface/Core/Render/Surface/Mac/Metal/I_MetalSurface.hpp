//
// Module: CelestialEngine/Engine/Modules/Core/Render/Surface/Mac/Metal
// File: I_MetalSurface.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_RENDER_SURFACE_I_METALSURFACE_HPP
#define CE_CORE_RENDER_SURFACE_I_METALSURFACE_HPP

#include "Types/Types.hpp"

#include <utility>

namespace CA {
	class MetalLayer;
}

namespace CE::Core {

/**
 * @class I_MetalSurface
 * @brief What a window has to offer for Metal to present into it
 * @details One method, where I_OpenGlSurface needs five, and the difference is the whole point: OpenGL has no
 *			presentation object, so swapping, pacing and measuring the drawable are three unrelated calls into
 *			whatever window library is underneath. Metal has one - the CAMetalLayer owns the back buffers, vends the
 *			drawables, holds the colour format and decides whether a present waits for the display - so the only thing
 *			a window still has to answer is which layer is its own.
 *
 *			That makes this the same shape as VkSurfaceKHR: a handle to the presentable surface, obtained from the
 *			window system, that the swapchain is then built against. MetalSwapchain does everything else through it.
 *
 *			Implemented by the window, not by the renderer: the window owns the view the layer is backing, and it
 *			outlives every swapchain built over it.
 */
class I_MetalSurface {
public:
	virtual ~I_MetalSurface() = default;

public:
	/**
	 * @brief Gets the Core Animation layer the window is presented through
	 * @return CA::MetalLayer* The layer, or null when the window has nothing to present into yet
	 * @details Borrowed, never owned: it belongs to the view it backs. A null answer is not fatal by itself - it only
	 *			means a swapchain cannot be built yet - so the caller is the one that decides how loudly to complain.
	 */
	[[nodiscard]] virtual CA::MetalLayer* GetMetalLayer() const = 0;

	/**
	 * @brief Gets the size the layer should present at, in backing pixels
	 * @return std::pair<u32, u32> Width and height in pixels, or {0, 0} for a window with nothing to draw into
	 * @details The one thing the layer cannot be trusted for. A CAMetalLayer reports a drawableSize of zero until
	 *			something has laid its view out, which on a window created and rendered into in the same turn is after
	 *			the first frame would have run - so the size is asked of the window, which knows it from the moment it
	 *			exists, and the swapchain pushes it onto the layer rather than reading it back.
	 *
	 *			Pixels, not screen coordinates: the two differ by the content scale on a Retina display, and pixels are
	 *			what a render area and a viewport are measured in.
	 */
	[[nodiscard]] virtual std::pair<u32, u32> GetDrawableSize() const = 0;
};

}

#endif //CE_CORE_RENDER_SURFACE_I_METALSURFACE_HPP
