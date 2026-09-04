//
// Module: CelestialEngine/Engine/Modules/Core/Render/Surface/Common/OpenGl
// File: I_OpenGlSurface.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-01
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_RENDER_SURFACE_I_OPENGLSURFACE_HPP
#define CE_CORE_RENDER_SURFACE_I_OPENGLSURFACE_HPP

#include "Types/Types.hpp"

#include <utility>


namespace CE::Core {

/**
 * @class I_OpenGlSurface
 * @brief What a window has to offer for OpenGL to present into it
 * @details OpenGL has no presentation of its own: swapping the buffers, pacing to the display and knowing how many
 *			pixels the drawable holds are all done by the window system layer - WGL, GLX, EGL, CGL - and every window
 *			backend wraps a different one. This is that layer, named and made virtual, so OpenGlSwapchain can be
 *			written once against it instead of once per windowing library.
 *
 *			Implemented by the window, not by the renderer: the window owns the context and outlives everything drawn
 *			through it.
 */
class I_OpenGlSurface {
public:
	virtual ~I_OpenGlSurface() = default;

public:
	/**
	 * @brief Makes this surface's OpenGL context current on the calling thread
	 * @details Every gl* call, including the ones in the swapchain's own setters, applies to whatever context is
	 *			current - so this has to happen before the device is created and again whenever a second context exists.
	 */
	virtual void MakeContextCurrent() = 0;

	/**
	 * @brief Puts the finished back buffer on screen
	 */
	virtual void SwapBuffers() = 0;

	/**
	 * @brief Sets how many display refreshes a present waits for
	 * @param interval 0 to run free, 1 to wait for one refresh; negative values ask for adaptive vsync where supported
	 * @details A property of the context, not of the window, so the implementation must make its context current first.
	 */
	virtual void SetSwapInterval(i32 interval) = 0;

public:
	/**
	 * @brief Gets the size of the drawable, in backing pixels
	 * @details Pixels, not screen coordinates: the two differ by the content scale on a Retina display. Returns {0, 0}
	 *			for a minimised window, which is what tells the swapchain to skip the frame.
	 */
	[[nodiscard]] virtual std::pair<u32, u32> GetDrawableSize() const = 0;

	/**
	 * @brief Gets the colour format the context was created with
	 * @details Fixed at context creation by the pixel format, so it belongs to whoever created the context - the window
	 *			- and not to the swapchain that merely reports it.
	 */
	[[nodiscard]] virtual Types::PixelFormat GetColorFormat() const = 0;
};

}

#endif //CE_CORE_RENDER_SURFACE_I_OPENGLSURFACE_HPP
