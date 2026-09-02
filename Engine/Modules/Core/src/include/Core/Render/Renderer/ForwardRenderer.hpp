//
// Module: CelestialEngine/Engine/Modules/Core/Render/Renderer
// File: ForwardRenderer.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_CORE_RENDER_RENDERER_FORWARDRENDERER_HPP
#define CE_CORE_RENDER_RENDERER_FORWARDRENDERER_HPP

#include "Core/Render/Renderer/I_Renderer.hpp"
#include "Core/Render/Renderer/RenderStats.hpp"
#include "Define/DynamicLinker.hpp"

#include <glm/glm.hpp>

#include <memory>

#include "Core/Render/Swapchain/I_Swapchain.hpp"


namespace CE::Core {

class I_CommandEncoder;

/**
 * @class ForwardRenderer
 * @brief Shades every submission in the pass it is drawn in, straight into the target
 * @details Forward rendering, in the plain sense: one pass, one shading of each surface, no G-buffer in between. It
 *			is the technique that costs the least to run and the least to reason about, which makes it the right first
 *			one - and the deferred renderer that may follow it is a sibling here, not a replacement, because forward
 *			stays the better answer for transparency and for MSAA.
 *
 *			It owns the device and the swapchain, and holds the encoder of the open pass. It owns nothing that is
 *			drawn: pipelines and buffers stay with whoever created them.
 */
class CE_CORE_API ForwardRenderer final: public I_Renderer {
public:
	ForwardRenderer(std::unique_ptr<I_GraphicDevice> graphicDevice, std::unique_ptr<I_Swapchain> swapchain);

	ForwardRenderer(const ForwardRenderer&) = delete;
	ForwardRenderer(ForwardRenderer&&) noexcept = delete;

	~ForwardRenderer() override;

public:
	ForwardRenderer& operator = (const ForwardRenderer&) = delete;
	ForwardRenderer& operator = (ForwardRenderer&&) noexcept = delete;

public:
	[[nodiscard]] bool BeginFrame() override;
	void EndFrame() override;

	void BeginPass() override;
	void BeginPass(const RenderPassDescriptor& descriptor) override;
	void EndPass() override;

	void Submit(const DrawCommand& command) override;

public:
	void OnResize(u32 width, u32 height) override;

	void SetVSync(bool enabled) override;

	void SetClearColor(glm::vec4 color) override;

public:
	[[nodiscard]] const I_GraphicDevice& GetGraphicDevice() const override { return *_graphicDevice; }
	[[nodiscard]] I_GraphicDevice& GetGraphicDevice() override { return *_graphicDevice; }

	[[nodiscard]] const I_Swapchain& GetSwapchain() const override { return *_swapchain; }
	[[nodiscard]] I_Swapchain& GetSwapchain() override { return *_swapchain; }

	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return _swapchain->GetGraphicApi(); }

	[[nodiscard]] const RenderStats& GetRenderStats() const override { return _frameStats; }

private:
	std::unique_ptr<I_GraphicDevice> _graphicDevice;
	std::unique_ptr<I_Swapchain> _swapchain;

	/// The open pass, or null when none is. Its lifetime *is* the pass: destroying it ends the pass on every backend.
	std::unique_ptr<I_CommandEncoder> _commandEncoder;

	glm::vec4 _clearColor = {0.1_f32, 0.1_f32, 0.1_f32, 1.0_f32};

	RenderStats _stats; ///< Published: what the last completed frame cost
	RenderStats _frameStats; ///< Accumulating: what the frame in flight has cost so far

	bool _inFrame = false;
};

}

#endif //CE_CORE_RENDER_RENDERER_FORWARDRENDERER_HPP
