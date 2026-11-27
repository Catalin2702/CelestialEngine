//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Command/Platforms/Mac/Metal
// File: MetalCommandEncoder.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-04
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#include "RHI/Command/Platforms/Mac/Metal/MetalCommandEncoder.hpp"

#include "RHI/Buffer/Platforms/Mac/Metal/MetalBuffer.hpp"
#include "RHI/Command/Viewport.hpp"
#include "RHI/Conversion/Platforms/Mac/Metal/MetalConversion.hpp"
#include "RHI/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#include "RHI/Pipeline/Platforms/Mac/Metal/MetalPipelineState.hpp"
#include "RHI/Texture/Platforms/Mac/Metal/MetalTexture.hpp"
#include "Tools/Tools.hpp"

#include <Metal/Metal.hpp>

#include <cassert>
#include <stdexcept>

namespace CE::RHI {

namespace {
constexpr u32 U32_SIZE = sizeof(u32);
}

MetalCommandEncoder::MetalCommandEncoder(MetalGraphicDevice& graphicDevice, MTL::CommandBuffer* nativeCommandBuffer, const MTL::RenderPassDescriptor* nativePassDescriptor):
	_graphicDevice(&graphicDevice),
	_nativeCommandBuffer(nativeCommandBuffer) {
	if (not _nativeCommandBuffer or not nativePassDescriptor) [[unlikely]] {
		constexpr auto error = "MetalCommandEncoder::MetalCommandEncoder: The pass was given no command buffer or no descriptor!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_nativeCommandEncoder = NS::RetainPtr(_nativeCommandBuffer->renderCommandEncoder(nativePassDescriptor));
	if (not _nativeCommandEncoder) [[unlikely]] {
		constexpr auto error = "MetalCommandEncoder::MetalCommandEncoder: Metal refused to encode against the render pass descriptor!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
}

MetalCommandEncoder::~MetalCommandEncoder() {
	if (not _nativeCommandEncoder or _ended)
		return;

	// A command buffer with an open encoder can never be committed, and Metal raises on the next
	// renderCommandEncoder() call against the same buffer: a pass that was never ended still has to close.
	MetalCommandEncoder::End();
}

void MetalCommandEncoder::DrawIndexed(const u32 indexCount, const u32 firstIndex, const u32 baseVertex) {
	assert(not _ended and "MetMetalCommandEncoder::DrawIndexed: The pass has already been ended!");
	assert(_topology != Types::PrimitiveTopology::None and "MetalCommandEncoder::DrawIndexed: No pipeline was set, so there is no primitive topology to draw with!");
	assert(_nativeIndexBuffer != nullptr and "MetalCommandEncoder::DrawIndexed: No index buffer was set!");
	assert(static_cast<size_t>(firstIndex) + indexCount <= _indexCount and "MetalCommandEncoder::DrawIndexed: Thw draw reads past the end of the index buffer!");

	if (indexCount == 0) [[unlikely]]
		return;

	_nativeCommandEncoder->drawIndexedPrimitives(
		ToMetalPrimitiveType(_topology),
		indexCount,
		MTL::IndexTypeUInt32,
		_nativeIndexBuffer,
		// A byte offset, not an index count: the index buffers are u32, so the two differ by four.
		static_cast<NS::UInteger>(firstIndex) * U32_SIZE,
		1, // instanceCount - instancing is a DrawCommand field away
		baseVertex,
		0 // baseInstance
	);
}

void MetalCommandEncoder::End() {
	if (_ended) [[unlikely]] {
		CE_CORE_WARN("MetalCommandEncoder::End: The pass has already been ended");
		return;
	}

	_nativeCommandEncoder->endEncoding();

	// Nothing else. The command buffer belongs to the frame, and the swapchain commits it once, with the present, when
	// every pass has closed - so all the passes of a frame share one buffer and Metal can see the dependency between
	// what one writes and the next loads.
	_ended = true;
}

void MetalCommandEncoder::SetPipelineState(const I_PipelineState& pipelineState) {
	assert(not _ended and "MetalCommandEncoder::SetPipelineState: The pass has already been ended!");
	assert(pipelineState.GetGraphicApi() == Types::GraphicsApi::Metal and "MetalCommandEncoder::SetPipelineState: The pipeline is not Metal based!");

	const auto& metalPipeline = static_cast<const MetalPipelineState&>(pipelineState);
	const auto& descriptor = metalPipeline.GetDescriptor();

	_nativeCommandEncoder->setRenderPipelineState(metalPipeline.GetPipelineState());
	_nativeCommandEncoder->setDepthStencilState(metalPipeline.GetDepthStencilState());

	_topology = descriptor.topology;
}

void MetalCommandEncoder::SetIndexBuffer(const I_IndexBuffer& indexBuffer) {
	assert(not _ended and "MetalCommandEncoder::SetIndexBuffer: The pass has already been ended!");
	assert(indexBuffer.GetGraphicApi() == Types::GraphicsApi::Metal and "MetalCommandEncoder::SetIndexBuffer: The index buffer is not Metal based!");

	const auto& metalIndexBuffer = static_cast<const MetalIndexBuffer&>(indexBuffer);

	// Nothing is encoded here: Metal has no element-buffer binding point, the buffer is an argument of
	// drawIndexedPrimitives. Remembering it is the whole implementation.
	_nativeIndexBuffer = metalIndexBuffer.GetBuffer();
	_indexCount = metalIndexBuffer.GetCount();
}

void MetalCommandEncoder::SetVertexBuffer(const I_VertexBuffer& vertexBuffer) {
	assert(not _ended and "MetalCommandEncoder::SetVertexBuffer: The pass has already been ended!");
	assert(vertexBuffer.GetGraphicApi() == Types::GraphicsApi::Metal and "MetalCommandEncoder::SetVertexBuffer: The vertex buffer is not Metal based!");

	const auto& metalVertexBuffer = static_cast<const MetalVertexBuffer&>(vertexBuffer);

	// No attribute setup to replay: the pipeline's vertex descriptor already says how to read this slot.
	_nativeCommandEncoder->setVertexBuffer(metalVertexBuffer.GetBuffer(), 0, METAL_VERTEX_BUFFER_INDEX);
}

void MetalCommandEncoder::SetViewport(const Viewport& viewport) {
	assert(not _ended and "MetalCommandEncoder::SetViewport: The pass has already been ended!");

	// Passed through: the origin is already the top-left corner with y growing downwards here, which is the
	// convention every viewport is stated in, so there is nothing to turn round.
	_nativeCommandEncoder->setViewport(MTL::Viewport{
		.originX = viewport.x,
		.originY = viewport.y,
		.width = viewport.width,
		.height = viewport.height,
		.znear = viewport.minDepth,
		.zfar = viewport.maxDepth
	});
}

void MetalCommandEncoder::SetFragmentTexture(const u32 slot, const I_Texture& texture) {
	assert(not _ended and "MetalCommandEncoder::SetFragmentTexture: The pass has already been ended!");
	assert(slot < Types::MAX_TEXTURE_SLOTS and "MetalCommandEncoder::SetFragmentTexture: The slot is past the last texture slot!");
	assert(texture.GetGraphicApi() == Types::GraphicsApi::Metal and "MetalCommandEncoder::SetFragmentTexture: The texture is not Metal based!");

	_nativeCommandEncoder->setFragmentTexture(static_cast<const MetalTexture&>(texture).GetTexture(), slot);
}

void MetalCommandEncoder::SetFragmentSampler(u32 slot, const I_Sampler& sampler) {
	assert(not _ended and "MetalCommandEncoder::SetFragmentSampler: The pass has already been ended!");
	assert(slot < Types::MAX_TEXTURE_SLOTS and "MetalCommandEncoder::SetFragmentSampler: The slot is past the last texture slot!");
	assert(sampler.GetGraphicApi() == Types::GraphicsApi::Metal and "MetalCommandEncoder::SetFragmentSampler: The sampler is not Metal based!");

	// Its own table: [[sampler(slot)]] in the shader, independent of the texture and buffer slots of the same number.
	_nativeCommandEncoder->setFragmentSamplerState(static_cast<const MetalSampler&>(sampler).GetSamplerState(), slot);
}

void MetalCommandEncoder::SetVertexConstants(const u32 slot, const std::span<const std::byte> data) {
	assert(not _ended and "MetalCommandEncoder::SetVertexConstants: The pass has already been ended!");
	assert(slot != METAL_VERTEX_BUFFER_INDEX and "MetalCommandEncoder::SetVertexConstants: The slot is the one the geometry is bound to!");

	// Copied into this frame's slice and bound by offset. The GPU reads the bytes as they are now: a later call - the
	// next draw's transform, or the next frame - writes somewhere else in the ring, never over these.
	if (const auto [nativeBuffer, offset] = _graphicDevice->AllocateFrameConstants(data); nativeBuffer) [[likely]] {
		_nativeCommandEncoder->setVertexBuffer(nativeBuffer, offset, slot);
		return;
	}

	assert(data.size() <= 4096 and "MetalCommandEncoder::SetVertexConstants: The ring is full and setVertexBytes takes at most 4KB!");
	_nativeCommandEncoder->setVertexBytes(data.data(), data.size(), slot);
}

void MetalCommandEncoder::SetFragmentConstants(const u32 slot, const std::span<const std::byte> data) {
	assert(not _ended and "MetalCommandEncoder::SetFragmentConstants: The pass has already been ended!");

	// Same as the vertex stage: the fragment stage has its own table of buffer slots, so no slot is shared with the
	// geometry and there is nothing to check against.
	if (const auto [nativeBuffer, offset] = _graphicDevice->AllocateFrameConstants(data); nativeBuffer) [[likely]] {
		_nativeCommandEncoder->setFragmentBuffer(nativeBuffer, offset, slot);
		return;
	}

	assert(data.size() <= 4096 and "MetalCommandEncoder::SetFragmentConstants: The ring is full and setFragmentBytes takes at most 4 KB!");
	_nativeCommandEncoder->setFragmentBytes(data.data(), data.size(), slot);
}

}
