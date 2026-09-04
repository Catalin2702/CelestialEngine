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
};

}

#endif //CE_CORE_RENDER_SURFACE_I_METALSURFACE_HPP
