//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain/Platforms/Common/OpenGl
// File: OpenGlSwapchain.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-31
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#pragma once

#ifndef CE_CORE_RENDER_SWAPCHAIN_OPENGLSWAPCHAIN_HPP
#define CE_CORE_RENDER_SWAPCHAIN_OPENGLSWAPCHAIN_HPP

#include "Core/Render/Swapchain/I_Swapchain.hpp"
#include "Define/DynamicLinker.hpp"


struct GLFWwindow;

namespace CE::Core {

/**
 * @class OpenGlSwapchain
 * @brief The GLFW window's default framebuffer, presented as a swapchain
 * @details OpenGL has no swapchain object: the chain is the window's own default framebuffer, the driver rotates
 *			through its buffers, and GLFW exposes only the two ends of it - glfwSwapBuffers to present and
 *			glfwSwapInterval to pace. So most of what the interface promises is either free or unavailable here, and
 *			the class exists to let the renderer be written against the explicit model the other backends need.
 *
 *			It does not own the window; the window must outlive it.
 */
class CE_CORE_API OpenGlSwapchain: public I_SwapchainBase<Types::GraphicsApi::OpenGL> {
public:
	/**
	 * @brief Binds the swapchain to a GLFW window whose OpenGL context is already current
	 * @param window The window whose default framebuffer this chain presents
	 */
	explicit OpenGlSwapchain(GLFWwindow* window);

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
	 * @details GLFW creates it as 8 bits per channel in RGBA order, and offers no way to ask for the swapped order
	 *			that CAMetalLayer and DXGI prefer, so this is fixed rather than queried.
	 */
	[[nodiscard]] Types::PixelFormat GetColorFormat() const override { return Types::PixelFormat::RGBA8Unorm; }

	/**
	 * @brief Gets the number of buffers in the chain
	 * @details GLFW asks for a double-buffered framebuffer, and the count is not something OpenGL lets us read back.
	 */
	[[nodiscard]] u32 GetBufferCount() const override { return 2; }

	[[nodiscard]] std::pair<u32, u32> GetSize() const override { return {_width, _height}; }

private:
	GLFWwindow* _window = nullptr;

	u32 _width = 0;
	u32 _height = 0;

	bool _vsync = false;
};

}

#endif //CE_CORE_RENDER_SWAPCHAIN_OPENGLSWAPCHAIN_HPP
