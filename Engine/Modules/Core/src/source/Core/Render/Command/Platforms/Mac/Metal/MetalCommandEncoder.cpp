//
// Module: CelestialEngine/Engine/Modules/Core/Render/Command/Platforms/Mac/Metal
// File: MetalCommandEncoder.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-04
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Core/Render/Command/Platforms/Mac/Metal/MetalCommandEncoder.hpp"
#include "Core/Render/Buffer/Platforms/Mac/Metal/MetalBuffer.hpp"
#include "Core/Render/Command/Viewport.hpp"
#include "Core/Render/Pipeline/Platforms/Mac/Metal/MetalPipelineState.hpp"
#include "Tools/Tools.hpp"

#include <Metal/Metal.hpp>

#include <cassert>
#include <stdexcept>

namespace CE::Core {

namespace {
constexpr u32 U32_SIZE = sizeof(u32);
}

MetalCommandEncoder::MetalCommandEncoder(MTL::CommandQueue* nativeQueue, const MTL::RenderPassDescriptor* nativePassDescriptor) {
	if (not nativeQueue or not nativePassDescriptor) [[unlikely]] {
		constexpr auto error = "MetalCommandEncoder::MetalCommandEncoder: The pass was given no command queue or no descriptor!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	// Autoreleased by Metal, so it is retained rather than transferred: RetainPtr adds our own reference, TransferPtr
	// would adopt one we were never given and over-release it when the encoder dies.
	_nativeCommandBuffer = NS::RetainPtr(nativeQueue->commandBuffer());
	if (not _nativeCommandBuffer) [[unlikely]] {
		constexpr auto error = "MetalCommandEncoder::MetalCommandEncoder: The queue handed back no command buffer!";
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
		Types::ToMetalPrimitiveType(_topology),
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

	// Handed to the GPU here. Nothing waits on it: the swapchain's present is a separate command buffer on the same
	// queue, and the queue is what orders the two.
	_nativeCommandBuffer->commit();

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
	_nativeCommandEncoder->setVertexBuffer(metalVertexBuffer.GetBuffer(), 0, Types::METAL_VERTEX_BUFFER_INDEX);
}

void MetalCommandEncoder::SetViewport(const Viewport& viewport) {
	assert(not _ended and "MetalCommandEncoder::SetViewport: The pass has already been ended!");
	assert(viewport.GetGraphicsApi() == Types::GraphicsApi::Metal and "MetalCommandEncoder::SetViewport: The viewport is not Metal based!");

	_nativeCommandEncoder->setViewport(MTL::Viewport{
		.originX = viewport.x,
		.originY = viewport.y,
		.width = viewport.width,
		.height = viewport.height,
		.znear = viewport.minDepth,
		.zfar = viewport.maxDepth
	});
}

}
