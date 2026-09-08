//
// Module: CelestialEngine/Engine/Modules/Core/Render/Renderer
// File: ForwardRenderer.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-08
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

class I_VertexBuffer;
class I_IndexBuffer;

class I_PipelineState;

class I_Texture;

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

	void SetCameraData(const Types::CameraData& cameraData) override;

public:
	[[nodiscard]] const I_GraphicDevice& GetGraphicDevice() const override { return *_graphicDevice; }
	[[nodiscard]] I_GraphicDevice& GetGraphicDevice() override { return *_graphicDevice; }

	[[nodiscard]] const I_Swapchain& GetSwapchain() const override { return *_swapchain; }
	[[nodiscard]] I_Swapchain& GetSwapchain() override { return *_swapchain; }

	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return _swapchain->GetGraphicApi(); }

	[[nodiscard]] const RenderStats& GetRenderStats() const override { return _frameStats; }

	/**
	 * @brief Gets the texture the scene and the overlay are drawn into
	 * @return const I_Texture* The scene colour target, or null before the first frame has sized it
	 * @details Not the back buffer: nothing draws into the drawable except the composite pass this renderer runs at
	 *			EndFrame. Anything that wants to add to the frame - the ImGui overlay does - opens its pass on this.
	 */
	[[nodiscard]] const I_Texture* GetSceneColorTarget() const override { return _sceneColor.get(); }

	/**
	 * @brief Gets the colour format a pipeline drawing into the scene must declare
	 * @details Asked rather than taken from the swapchain, which is what every pipeline used to do: the two agree
	 *			today and stop agreeing the moment the scene target becomes RGBA16Float for HDR.
	 */
	[[nodiscard]] Types::PixelFormat GetSceneColorFormat() const override { return _sceneColorFormat; }

	[[nodiscard]] const Types::CameraData& GetCameraData() const override { return _cameraData; }

private:
	/**
	 * @brief Opens a pass without counting it
	 * @details The whole of BeginPass except the statistics. It exists so the composite - a pass the application
	 *			never asked for - can be encoded without appearing in the numbers the application is shown.
	 */
	void _OpenPass(const RenderPassDescriptor& descriptor);

	/**
	 * @brief Creates the scene colour and depth textures, or replaces them when the size changed
	 * @details Called once per frame from BeginFrame, and a no-op on all but the first frame after a resize. The old
	 *			textures are released only once the new pair exists, so a failed allocation leaves the renderer with a
	 *			working target rather than none.
	 */
	void _EnsureSceneTarget(u32 width, u32 height);

	/**
	 * @brief Draws the scene target into the swapchain's back buffer
	 * @details The one pass that touches the drawable. It writes every pixel, loads nothing and tests no depth, so
	 *			the back buffer goes from whatever it held to the finished frame in a single pass - which is the whole
	 *			reason the scene is rendered offscreen in the first place.
	 *
	 *			Checks nothing: everything it needs is an invariant BeginFrame or the constructor already enforced.
	 */
	void _Composite();

	/**
	 * @brief Builds the composite pipeline and its quad
	 * @details Called from the constructor, and throws std::runtime_error if it cannot finish. The composite is the
	 *			only pass that writes the back buffer, so a renderer that has none cannot present anything: there is no
	 *			degraded mode worth keeping, and a failure here is reported once at startup rather than once per frame.
	 */
	void _CreateCompositeResources();

private:
	std::unique_ptr<I_GraphicDevice> _graphicDevice;
	std::unique_ptr<I_Swapchain> _swapchain;

	/// The open pass, or null when none is. Its lifetime *is* the pass: destroying it ends the pass on every backend.
	std::unique_ptr<I_CommandEncoder> _commandEncoder;

	glm::vec4 _clearColor = {0.1_f32, 0.1_f32, 0.1_f32, 1.0_f32};

	/// The point of view the pass draws from. Held but not yet bound: there is no uniform buffer in the engine to
	/// carry it to a shader with, so nothing on the GPU reads it until that seam exists. Defaults to identity, which
	/// is what leaves a vertex stage writing clip space directly - the composite quad relies on that.
	Types::CameraData _cameraData;

	RenderStats _stats; ///< Published: what the last completed frame cost
	RenderStats _frameStats; ///< Accumulating: what the frame in flight has cost so far

	bool _inFrame = false;

	std::shared_ptr<I_Texture> _sceneColor;
	std::shared_ptr<I_Texture> _sceneDepth;

	/// Fixed at construction so a pipeline can be built before the first frame sizes the target.
	Types::PixelFormat _sceneColorFormat = Types::PixelFormat::None;
	Types::PixelFormat _sceneDepthFormat = Types::PixelFormat::Depth32Float;

	std::shared_ptr<I_PipelineState> _compositePipeline;
	std::shared_ptr<I_VertexBuffer> _compositeVertexBuffer;
	std::shared_ptr<I_IndexBuffer> _compositeIndexBuffer;
};

}

#endif //CE_CORE_RENDER_RENDERER_FORWARDRENDERER_HPP
