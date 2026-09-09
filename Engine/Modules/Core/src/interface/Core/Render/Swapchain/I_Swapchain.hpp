//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain
// File: I_Swapchain.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-31
// Updated by: Catalin Chirosca
// Updated: 2026-09-09
//

#pragma once

#ifndef CE_CORE_RENDER_SWAPCHAIN_I_SWAPCHAIN_HPP
#define CE_CORE_RENDER_SWAPCHAIN_I_SWAPCHAIN_HPP

#include "Types/Types.hpp"

#include <memory>
#include <utility>


namespace CE::Core {

class I_GraphicDevice;
class I_Window;

/**
 * @class I_Swapchain
 * @brief The chain of buffers a window is drawn into and presented from
 * @details One per window, created after the device because it needs it. It owns the back buffers, hands one out per
 *			frame and puts the finished one on screen; it does not own the window, and it is not where rendering
 *			commands go.
 *
 *			The shape follows the APIs that make the chain explicit, because the ones that hide it - where the chain is
 *			the window's default framebuffer - fit inside that shape, while the reverse is not true.
 */
class I_Swapchain {
public:
	virtual ~I_Swapchain() = default;

public:
	[[nodiscard]] static std::unique_ptr<I_Swapchain> MakeSwapchain(I_Window& window, I_GraphicDevice& graphicDevice, Types::GraphicsApi api);

public:
	/**
	 * @brief Brings the back buffers in line with the window, and reports whether this frame can be drawn at all
	 * @return bool False when the window offers no drawable area - a minimised one - and the frame must be skipped
	 * @details Split out of AcquireNextTarget so that a frame can learn its size without holding a back buffer while
	 *			it draws. The size is what the render targets and the viewports are built from, and it is needed at
	 *			the top of the frame; the back buffer is only needed by the pass that writes it, which is the last one.
	 *			Asking for both at once is what made every frame hold a buffer for its whole length.
	 *
	 *			Takes nothing and holds nothing, so a frame that stops after this one has nothing to give back.
	 */
	[[nodiscard]] virtual bool PrepareFrame() = 0;

	/**
	 * @brief Takes hold of the next back buffer, and reports whether the frame can still go ahead
	 * @return bool False when this frame must be skipped rather than presented
	 * @details Call it as late as the frame allows - immediately before the pass that draws into the back buffer.
	 *			Every backend hands out a small, fixed number of them and cannot hand out the next until the display
	 *			system has finished with one, so the interval between this call and Present is time no other frame can
	 *			start in, and it is subtracted from the frame rate wherever the chain is a real one.
	 *
	 *			Fallible on purpose. A backend can find the surface no longer matches the chain, or have no buffer to
	 *			give under memory pressure, and a minimised window has nothing to draw into anywhere. A false here is
	 *			normal operation, not an error to report - but by this point the frame has already been encoded, so a
	 *			backend that answers false has to release whatever work was recorded against it rather than leave it
	 *			pending.
	 */
	[[nodiscard]] virtual bool AcquireNextTarget() = 0;

	/**
	 * @brief Puts the acquired back buffer on screen
	 */
	virtual void Present() = 0;

	/**
	 * @brief Resizes the back buffers to match the window
	 * @details May rebuild the whole chain - some backends have to - so every render target obtained before this call
	 *			must be treated as invalid afterwards.
	 */
	virtual void Resize(u32 width, u32 height) = 0;

	/**
	 * @brief Switches presentation between waiting for the display's refresh and running free
	 * @details Not a cheap setter, despite looking like one: a backend can bake the present mode into the chain at
	 *			creation, so changing it there means rebuilding it, with the same consequences as Resize.
	 */
	virtual void SetVSync(bool enabled) = 0;

public:
	[[nodiscard]] virtual bool IsVSyncEnabled() const = 0;

	/**
	 * @brief Gets the format of the colour buffers
	 * @details A pipeline drawing into this swapchain has to declare the same format in its RenderTargetFormats, and
	 *			most backends reject the draw when the two disagree.
	 */
	[[nodiscard]] virtual Types::PixelFormat GetColorFormat() const = 0;

	/**
	 * @brief Gets how many buffers the chain rotates through
	 * @details Sets the ceiling on frames in flight: with two buffers the CPU can be at most one frame ahead of the
	 *			GPU. Backends that need per-frame resources and fences size them from this.
	 */
	[[nodiscard]] virtual u32 GetBufferCount() const = 0;

	/**
	 * @brief Gets the size of the back buffers, in pixels
	 * @details Backing pixels, not screen coordinates: the two differ by the content scale on a high-DPI display, and
	 *			it is the pixels a viewport and a render pass are measured in.
	 */
	[[nodiscard]] virtual std::pair<u32, u32> GetSize() const = 0;

	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;
};

template<Types::GraphicsApi Api>
class I_SwapchainBase: public I_Swapchain {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return Api; }
};



}

#endif //CE_CORE_RENDER_SWAPCHAIN_I_SWAPCHAIN_HPP
