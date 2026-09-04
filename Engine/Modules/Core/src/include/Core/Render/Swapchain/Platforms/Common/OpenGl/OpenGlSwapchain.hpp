//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain/Platforms/Common/OpenGl
// File: OpenGlSwapchain.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-31
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_RENDER_SWAPCHAIN_OPENGLSWAPCHAIN_HPP
#define CE_CORE_RENDER_SWAPCHAIN_OPENGLSWAPCHAIN_HPP

#include "Core/Render/Swapchain/I_Swapchain.hpp"
#include "Core/Render/Surface/Common/OpenGl/I_OpenGlSurface.hpp"
#include "Define/DynamicLinker.hpp"


namespace CE::Core {

/**
 * @class OpenGlSwapchain
 * @brief The window's default framebuffer, presented as a swapchain
 * @details OpenGL has no swapchain object: the chain is the window's own default framebuffer, the driver rotates
 *			through its buffers, and the window system layer exposes only the two ends of it - swap to present and the
 *			swap interval to pace. So most of what the interface promises is either free or unavailable here, and the
 *			class exists to let the renderer be written against the explicit model the other backends need.
 *
 *			Every one of those two ends is reached through I_OpenGlSurface, so this works over GLFW, over CGL on
 *			Cocoa, or over any window backend that can present an OpenGL context. It does not own the surface; the
 *			window that implements it must outlive this.
 */
class CE_CORE_API OpenGlSwapchain: public I_SwapchainBase<Types::GraphicsApi::OpenGL> {
public:
	/**
	 * @brief Binds the swapchain to a surface whose OpenGL context is already current
	 * @param surface The window's presentation surface, which must outlive this swapchain
	 */
	explicit OpenGlSwapchain(I_OpenGlSurface& surface);

	OpenGlSwapchain(const OpenGlSwapchain&) = delete;

	OpenGlSwapchain(OpenGlSwapchain&&) noexcept = default;

public:
	OpenGlSwapchain& operator = (const OpenGlSwapchain&) = delete;
	OpenGlSwapchain& operator = (OpenGlSwapchain&&) noexcept = default;

public:
	/**
	 * @brief Refreshes the cached framebuffer size and reports whether there is anything to draw into
	 * @details There is nothing to acquire in OpenGL - the driver picks the back buffer itself - so this only reads
	 *			the current framebuffer size and answers false for the one case that does occur, a window minimised to
	 *			a zero-sized framebuffer.
	 */
	[[nodiscard]] bool AcquireNextTarget() override;

	void Present() override;

	/**
	 * @brief Records the new size
	 * @details The default framebuffer follows the window on its own, so there is nothing to rebuild: this only keeps
	 *			GetSize() answering correctly between one frame's acquire and the next.
	 */
	void Resize(u32 width, u32 height) override;

	void SetVSync(bool enabled) override;

public:
	[[nodiscard]] bool IsVSyncEnabled() const override { return _vsync; }

	/**
	 * @brief Gets the colour format of the default framebuffer
	 * @details Decided by the pixel format the surface created its context with, so it is asked rather than assumed:
	 *			GLFW only ever gives 8 bits per channel in RGBA order, while CGL can be asked for the swapped order
	 *			that CAMetalLayer and DXGI prefer.
	 */
	[[nodiscard]] Types::PixelFormat GetColorFormat() const override { return _surface->GetColorFormat(); }

	/**
	 * @brief Gets the number of buffers in the chain
	 * @details Every backend asks its window for a double-buffered framebuffer, and the count is not something OpenGL
	 *			lets us read back.
	 */
	[[nodiscard]] u32 GetBufferCount() const override { return 2; }

	[[nodiscard]] std::pair<u32, u32> GetSize() const override { return {_width, _height}; }

private:
	I_OpenGlSurface* _surface = nullptr;

	u32 _width = 0;
	u32 _height = 0;

	bool _vsync = false;
};

}

#endif //CE_CORE_RENDER_SWAPCHAIN_OPENGLSWAPCHAIN_HPP
