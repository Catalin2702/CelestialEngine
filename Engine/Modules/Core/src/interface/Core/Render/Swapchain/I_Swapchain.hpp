//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain
// File: I_Swapchain.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-31
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#pragma once

#ifndef CE_CORE_RENDER_SWAPCHAIN_I_SWAPCHAIN_HPP
#define CE_CORE_RENDER_SWAPCHAIN_I_SWAPCHAIN_HPP

#include "Types/Types.hpp"

#include <memory>
#include <utility>


namespace CE::Core {

class I_Window;

/**
 * @class I_Swapchain
 * @brief The chain of buffers a window is drawn into and presented from
 * @details One per window, created after the device because it needs it. It owns the back buffers, hands one out per
 *			frame and puts the finished one on screen; it does not own the window, and it is not where rendering
 *			commands go.
 *
 *			The shape follows the APIs that make the chain explicit - IDXGISwapChain and VkSwapchainKHR - because the
 *			ones that hide it (OpenGL, where the chain is the window's default framebuffer) fit inside that shape,
 *			while the reverse is not true.
 */
class I_Swapchain {
public:
	virtual ~I_Swapchain() = default;

public:
	[[nodiscard]] static std::unique_ptr<I_Swapchain> MakeSwapchain(I_Window& window, Types::GraphicsApi api);

public:
	/**
	 * @brief Takes hold of the next back buffer, and reports whether the frame can go ahead
	 * @return bool False when this frame must be skipped rather than drawn
	 * @details Fallible on purpose. Vulkan answers VK_ERROR_OUT_OF_DATE_KHR when the surface no longer matches the
	 *			swapchain, Metal hands back no drawable under memory pressure, and a minimised window has nothing to
	 *			draw into on any backend. A false here is normal operation, not an error to report.
	 */
	[[nodiscard]] virtual bool AcquireNextTarget() = 0;

	/**
	 * @brief Puts the acquired back buffer on screen
	 */
	virtual void Present() = 0;

	/**
	 * @brief Resizes the back buffers to match the window
	 * @details May rebuild the whole chain - Vulkan has to - so every render target obtained before this call must be
	 *			treated as invalid afterwards.
	 */
	virtual void Resize(u32 width, u32 height) = 0;

	/**
	 * @brief Switches presentation between waiting for the display's refresh and running free
	 * @details Not a cheap setter, despite looking like one: Vulkan bakes the present mode into the swapchain at
	 *			creation, so changing it there means rebuilding the chain, with the same consequences as Resize.
	 */
	virtual void SetVSync(bool enabled) = 0;

public:
	[[nodiscard]] virtual bool IsVSyncEnabled() const = 0;

	/**
	 * @brief Gets the format of the colour buffers
	 * @details A pipeline drawing into this swapchain has to declare the same format in its RenderTargetFormats, and
	 *			every backend but OpenGL rejects the draw when they disagree.
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
	 * @details Backing pixels, not screen coordinates: the two differ by the content scale on a Retina display, and it
	 *			is the pixels a viewport and a render pass are measured in.
	 */
	[[nodiscard]] virtual std::pair<u32, u32> GetSize() const = 0;

	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;
};

template<Types::GraphicsApi Api>
class I_SwapchainBase: public I_Swapchain {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return _api; }

private:
	static constexpr Types::GraphicsApi _api = Api;
};



}

#endif //CE_CORE_RENDER_SWAPCHAIN_I_SWAPCHAIN_HPP
