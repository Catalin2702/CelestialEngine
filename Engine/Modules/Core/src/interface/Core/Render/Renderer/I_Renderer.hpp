//
// Module: CelestialEngine/Engine/Modules/Core/Render/Renderer
// File: I_Renderer.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_RENDER_RENDERER_I_RENDERER_HPP
#define CE_CORE_RENDER_RENDERER_I_RENDERER_HPP

#include "Types/Types.hpp"

#include <glm/glm.hpp>

#include <memory>


namespace CE::Core {

class I_GraphicDevice;
class I_Swapchain;
class I_Window;

struct DrawCommand;
struct RenderPassDescriptor;
struct RenderStats;

class I_Renderer {
public:
	virtual ~I_Renderer() = default;

public:
	/**
	 * @brief Builds a renderer, together with the device and swapchain it drives
	 * @param window The window to present into, which must outlive the renderer
	 * @param api The graphics API the device and swapchain are created for
	 * @return std::unique_ptr<I_Renderer> A renderer with a live device and swapchain, before the first frame
	 */
	[[nodiscard]] static std::unique_ptr<I_Renderer> MakeRenderer(I_Window& window, Types::GraphicsApi api);

public:
	/**
	 * @brief Acquires the frame's render target and reports whether the frame can go ahead
	 * @return bool False when this frame must be skipped - a minimised window, or a drawable the backend refused
	 * @details Every other method on this interface is only valid between a BeginFrame that answered true and the
	 *			matching EndFrame.
	 */
	[[nodiscard]] virtual bool BeginFrame() = 0;

	/**
	 * @brief Finishes the frame and presents it
	 * @details Ends a pass left open, so a caller that returns early does not have to unwind by hand.
	 */
	virtual void EndFrame() = 0;

	/**
	 * @brief Opens a pass over the whole swapchain target, clearing it to the renderer's clear colour
	 * @details The common case, spelled once here rather than at every call site: the descriptor it builds is the one
	 *			an application would have written anyway, sized from the swapchain and pointed at its back buffer.
	 */
	virtual void BeginPass() = 0;

	/**
	 * @brief Opens a pass over the attachments the descriptor names
	 * @param descriptor The pass' attachments, load and store actions, and render area
	 * @details Only one pass is open at a time: opening a second ends the first, because a tile-based backend cannot
	 *			interleave two encoders on one command buffer.
	 */
	virtual void BeginPass(const RenderPassDescriptor& descriptor) = 0;

	/**
	 * @brief Closes the open pass
	 * @details A no-op when no pass is open, so EndFrame and an explicit call cannot double-end one.
	 */
	virtual void EndPass() = 0;

	/**
	 * @brief Draws one command into the open pass
	 * @param command What to draw, and the pipeline to draw it with
	 * @details Immediate under every backend the engine has today - the command is encoded as it arrives, not queued
	 *			for a sort - but the signature is the one a sorted queue would need, so batching can be added inside an
	 *			implementation without touching a call site.
	 */
	virtual void Submit(const DrawCommand& command) = 0;

public:
	/**
	 * @brief Resizes the swapchain to a new drawable size, in backing pixels
	 * @details Must not be called between BeginFrame and EndFrame: rebuilding the chain invalidates the target the
	 *			frame acquired.
	 */
	virtual void OnResize(u32 width, u32 height) = 0;

	/**
	 * @brief Switches presentation between waiting for the display's refresh and running free
	 */
	virtual void SetVSync(bool enabled) = 0;

	/**
	 * @brief Sets the colour a default pass clears its target to
	 */
	virtual void SetClearColor(glm::vec4 color);

public:
	[[nodiscard]] virtual const I_GraphicDevice& GetGraphicDevice() const = 0;
	[[nodiscard]] virtual I_GraphicDevice& GetGraphicDevice() = 0;

	[[nodiscard]] virtual const I_Swapchain& GetSwapchain() const = 0;
	[[nodiscard]] virtual I_Swapchain& GetSwapchain() = 0;

	/**
	 * @brief Gets what the last completed frame cost
	 * @details The previous frame's, not the one in flight: a frame's counters are published by EndFrame, so this
	 *			answers the same numbers all through the frame that reads them - which is what a UI drawing them needs.
	 */
	[[nodiscard]] virtual const RenderStats& GetRenderStats() const = 0;

	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;
};

}

#endif //CE_CORE_RENDER_RENDERER_I_RENDERER_HPP
