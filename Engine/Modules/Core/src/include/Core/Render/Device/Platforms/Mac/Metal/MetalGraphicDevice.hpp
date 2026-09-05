//
// Module: CelestialEngine/Engine/Modules/Core/Device/Platforms/Mac/Metal
// File: MetalGraphicDevice.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_RENDER_DEVICE_METALGRAPHICDEVICE_HPP
#define CE_CORE_RENDER_DEVICE_METALGRAPHICDEVICE_HPP

#include "Core/Render/Device/I_GraphicDevice.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <Foundation/Foundation.hpp>


namespace CA {
	class MetalDrawable;
}

namespace MTL {
	class CommandBuffer;
	class CommandQueue;
	class Device;
	class Texture;
}

namespace CE::Core {

/**
 * @class MetalGraphicDevice
 * @brief The Metal device, its queue and its shader library - everything a resource is created from
 * @details Owns what outlives a frame; owns nothing that belongs to one. The drawable a frame renders into comes from
 *			the swapchain, and is published here through SetFrameTarget for the duration of that frame.
 */
class CE_CORE_API MetalGraphicDevice final: public I_GraphicDeviceBase<Types::GraphicsApi::Metal> {
public:
	/**
	 * @brief Creates the system default device, its command queue and loads the bundled .metallib
	 * @details Throws std::runtime_error if any of the three fails: without them nothing can be rendered, and a
	 *			half-built device would only move the failure to the first draw.
	 */
	MetalGraphicDevice();

	MetalGraphicDevice(const MetalGraphicDevice&) = delete;

	MetalGraphicDevice(MetalGraphicDevice&& other) noexcept = default;

	/**
	 * @brief Implemented by default in the .cpp file because of the forward declarations
	 * @details Implemented by default in the .cpp file because otherwise the destructor calls made by `NS::SharedPtr`
	 *			for the forward declared classes would cause the linker to fail.
	 */
	~MetalGraphicDevice() override;

public:
	MetalGraphicDevice& operator = (const MetalGraphicDevice&) = delete;
	MetalGraphicDevice& operator = (MetalGraphicDevice&& other) noexcept = default;

public:
	[[nodiscard]] std::shared_ptr<I_ShaderModule> CreateShaderModule(const ShaderModuleDescriptor& descriptor) override;
	[[nodiscard]] std::shared_ptr<I_PipelineState> CreatePipelineState(const PipelineDescriptor& descriptor) override;
	[[nodiscard]] std::shared_ptr<I_IndexBuffer> CreateIndexBuffer(std::span<const u32> indices) override;
	[[nodiscard]] std::shared_ptr<I_VertexBuffer> CreateVertexBuffer(std::span<const f32> vertices, const BufferLayout& layout) override;

	/**
	 * @brief Builds the Metal render pass descriptor and opens an encoder on it
	 * @return A MetalCommandEncoder, or null when the pass has nothing to draw into
	 * @details Returns null - the same "skip this pass" answer the OpenGL device gives for a zero-sized render area -
	 *			when no frame target has been published, which is what a frame whose drawable was not acquired looks
	 *			like. That is normal under memory pressure, not an error to escalate.
	 */
	[[nodiscard]] std::unique_ptr<I_CommandEncoder> BeginRenderPass(const RenderPassDescriptor& descriptor) override;

public:
	/**
	 * @brief Publishes the textures the current frame renders into
	 * @param drawable The drawable MetalSwapchain acquired for this frame, or null to clear the target
	 * @param depthTexture The swapchain's depth buffer, or null when the pass does no depth testing
	 * @details Called by MetalSwapchain::AcquireNextTarget, and cleared at Present. It is the seam between "who owns
	 *			the back buffer" (the swapchain) and "who builds render passes" (the device), and it is deliberately
	 *			Metal-only: the portable version of it is a RenderPassDescriptor carrying an I_Texture per attachment,
	 *			which is what this collapses into the day the engine has a texture type.
	 *
	 *			The drawable is borrowed for the frame, not retained: it belongs to the display-link update or the
	 *			layer's autorelease pool, exactly as MetalContext::AcquireDrawable already documents.
	 */
	void SetFrameTarget(CA::MetalDrawable* drawable, MTL::Texture* depthTexture);

	/**
	 * @brief Gets the command buffer every pass of this frame encodes into, opening one if the frame has none yet
	 * @return MTL::CommandBuffer* The frame's buffer, or null when the queue refuses one
	 * @details One buffer per frame, several encoders on it - not one buffer per pass. Metal tracks the dependency
	 *			between a pass that writes an attachment and a later pass that loads it *within* a command buffer; it
	 *			does not do so between separate ones, where all that is guaranteed is the order they execute in. On a
	 *			tile-based GPU that difference is visible: the second pass loads tiles the first has not resolved yet,
	 *			and redraws them as it found them.
	 */
	[[nodiscard]] MTL::CommandBuffer* GetFrameCommandBuffer();

	/**
	 * @brief Takes over a command buffer that has finished encoding into the frame's target
	 * @param commandBuffer The buffer, encoded but not committed
	 * @details The frame's passes hand their buffers here instead of committing them, and the swapchain commits the
	 *			last one together with the present. That ordering is not a tidiness: presentDrawable shows the drawable
	 *			when *its own* command buffer is scheduled, and a buffer holding nothing but a present is scheduled at
	 *			once - possibly while the pass that draws the frame is still running on the GPU. Presenting from the
	 *			buffer that did the drawing is the only arrangement in which that cannot happen.
	 *
	 *			A buffer already held is committed on the spot: it is no longer the last one, so it no longer carries
	 *			the present, and it must still reach the GPU before the one replacing it.
	 */
	void HoldFrameCommandBuffer(NS::SharedPtr<MTL::CommandBuffer> commandBuffer);

	/**
	 * @brief Hands the held command buffer over, leaving none behind
	 * @return NS::SharedPtr<MTL::CommandBuffer> The frame's last buffer, or null when nothing was encoded
	 * @details The caller owns it, and owes it a commit - it is the one that presents.
	 */
	[[nodiscard]] NS::SharedPtr<MTL::CommandBuffer> TakeFrameCommandBuffer();

public:
	[[nodiscard]] MTL::Device* GetDevice() const { return _nativeDevice.get(); }
	[[nodiscard]] MTL::CommandQueue* GetCommandQueue() const { return _nativeCommandQueue.get(); }
	[[nodiscard]] const MetalShaderLibrary& GetShaderLibrary() const { return _shaderLibrary; }

private:
	NS::SharedPtr<MTL::Device> _nativeDevice;
	NS::SharedPtr<MTL::CommandQueue> _nativeCommandQueue;
	MetalShaderLibrary _shaderLibrary;

	CA::MetalDrawable* _nativeFrameDrawable = nullptr; ///< Borrowed for the current frame; see SetFrameTarget
	MTL::Texture* _nativeFrameDepthTexture = nullptr; ///< Borrowed for the current frame; owned by the swapchain

	/// The last command buffer encoded into the frame's target, waiting for the present to be added to it. Owned only
	/// until the swapchain takes it, which is once per frame.
	NS::SharedPtr<MTL::CommandBuffer> _nativeFrameCommandBuffer;
};

}

#endif //CE_CORE_RENDER_DEVICE_METALGRAPHICDEVICE_HPP
