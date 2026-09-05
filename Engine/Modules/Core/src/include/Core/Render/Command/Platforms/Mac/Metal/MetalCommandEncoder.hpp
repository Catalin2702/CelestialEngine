//
// Module: CelestialEngine/Engine/Modules/Core/Render/Command/Platforms/Mac/Metal
// File: MetalCommandEncoder.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-04
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_RENDER_COMMAND_METALCOMMANDENCODER_HPP
#define CE_CORE_RENDER_COMMAND_METALCOMMANDENCODER_HPP

#include "Core/Render/Command/I_CommandEncoder.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <Foundation/Foundation.hpp>


namespace MTL {
	class Buffer;
	class CommandBuffer;
	class CommandQueue;
	class RenderCommandEncoder;
	class RenderPassDescriptor;
}

namespace CE::Core {

class MetalGraphicDevice;

/**
 * @class MetalCommandEncoder
 * @brief One render pass, recorded into an MTL::CommandBuffer and committed when the pass ends
 * @details Where the OpenGL encoder pretends to record and actually issues, this one really does record: every call
 *			lands in a command buffer that the GPU only sees at End(). That is the model I_CommandEncoder was shaped
 *			for, and the reason the interface has an End() at all.
 *
 *			One command buffer per pass. It is the simplest arrangement that is correct - buffers submitted to a queue
 *			begin in submission order - and it is what a frame graph would later replace with one buffer per frame,
 *			holding several encoders. Presentation is deliberately not here: MetalSwapchain presents on its own
 *			command buffer, which the queue orders after this one.
 *
 *			It owns the command buffer and the encoder, and nothing that is drawn: the resources passed to the setters
 *			are borrowed until the pass ends, the same contract a Vulkan command buffer carries.
 */
class CE_CORE_API MetalCommandEncoder final: public I_CommandEncoderBase<Types::GraphicsApi::Metal> {
public:
	/**
	 * @brief Opens a command buffer on the queue and starts encoding the pass described by the descriptor
	 * @param nativeCommandBuffer The frame's command buffer, shared with every other pass of the same frame
	 * @param nativePassDescriptor The already built MTL render pass descriptor - attachments, load and store actions
	 * @details Throws std::runtime_error when the queue hands back no command buffer, or when the descriptor is one
	 *			Metal will not encode against; both are fatal for the frame, not skippable.
	 */
	MetalCommandEncoder(MTL::CommandBuffer* nativeCommandBuffer, const MTL::RenderPassDescriptor* nativePassDescriptor);

	MetalCommandEncoder(const MetalCommandEncoder&) = delete;
	MetalCommandEncoder(MetalCommandEncoder&&) noexcept = default;

	~MetalCommandEncoder() override;

public:
	MetalCommandEncoder& operator = (const MetalCommandEncoder&) = delete;
	MetalCommandEncoder& operator = (MetalCommandEncoder&&) noexcept = default;

public:
	/**
	 * @brief Encodes one indexed draw with the current pipeline, vertex buffer and index buffer
	 */
	void DrawIndexed(u32 indexCount, u32 firstIndex, u32 baseVertex) override;

	/**
	 * @brief Ends the pass and commits its command buffer to the GPU
	 * @details Closes the encoder and nothing else. The command buffer is the frame's, not this pass', and the
	 *			swapchain commits it once - with the present - after the last pass has closed.
	 */
	void End() override;

public:
	/**
	 * @brief Sets the compiled pipeline, its depth state, and the rasterizer state Metal keeps on the encoder
	 * @details Three calls, not one: cull mode and winding are encoder state in Metal, so they are read back out of
	 *			the pipeline's descriptor here. Setting them every time a pipeline is bound is what keeps a pass from
	 *			inheriting the previous pipeline's culling.
	 */
	void SetPipelineState(const I_PipelineState& pipelineState) override;

	/**
	 * @brief Remembers the index buffer, which Metal takes at draw time rather than as a binding
	 */
	void SetIndexBuffer(const I_IndexBuffer& indexBuffer) override;

	/**
	 * @brief Remembers the index buffer, which Metal takes at draw time rather than as a binding
	 */
	void SetVertexBuffer(const I_VertexBuffer& vertexBuffer) override;

	/**
	 * @brief Sets the viewport rectangle and the depth range
	 */
	void SetViewport(const Viewport& viewport) override;

private:
	/// Borrowed, and shared with every other pass of this frame: the device owns it and the swapchain commits it.
	MTL::CommandBuffer* _nativeCommandBuffer = nullptr;
	NS::SharedPtr<MTL::RenderCommandEncoder> _nativeCommandEncoder;

	/// Taken from the pipeline: Metal wants the primitive type as an argument of the draw call, as OpenGL does.
	Types::PrimitiveTopology _topology = Types::PrimitiveTopology::None;

	MTL::Buffer* _nativeIndexBuffer = nullptr; ///< Borrowed; owned by the MetalIndexBuffer the caller keeps alive
	size_t _indexCount = 0; ///< Indices in that buffer, to bounds-check the draws in Debug

	bool _ended = false;
};

}

#endif //CE_CORE_RENDER_COMMAND_METALCOMMANDENCODER_HPP
