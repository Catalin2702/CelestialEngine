//
// Module: CelestialEngine/Engine/Modules/Core/Device/Platforms/Mac/Metal
// File: MetalGraphicDevice.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#include "RHI/Buffer/Platforms/Mac/Metal/MetalBuffer.hpp"
#include "RHI/Conversion/Platforms/Mac/Metal/MetalConversion.hpp"
#include "RHI/Command/Command.hpp"
#include "RHI/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#include "RHI/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp"
#include "RHI/Pipeline/Platforms/Mac/Metal/MetalPipelineState.hpp"
#include "RHI/Texture/Platforms/Mac/Metal/MetalTexture.hpp"
#include "Tools/Tools.hpp"
#include "Types/Types.hpp"

#include <Metal/Metal.hpp>

#include <cassert>
#include <utility>

#include "QuartzCore/CAMetalDrawable.hpp"


namespace CE::RHI {

MetalGraphicDevice::MetalGraphicDevice() {
	_nativeDevice = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
	if (not _nativeDevice) [[unlikely]] {
		constexpr auto error = "MetalGraphicDevice::MetalGraphicDevice: Could not create the MTL::Device!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_nativeCommandQueue = NS::TransferPtr(_nativeDevice->newCommandQueue());
	if (not _nativeCommandQueue) [[unlikely]] {
		constexpr auto error = "MetalGraphicDevice::MetalGraphicDevice: Could not create the MTL::CommandQueue!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_framesInFlight = std::make_unique<std::counting_semaphore<MAX_FRAMES_IN_FLIGHT>>(MAX_FRAMES_IN_FLIGHT);

	// Allocated once and never resized: a slice per frame in flight, written by the CPU and read by the GPU. Shared
	// storage because on Apple silicon the two see the same memory, and write-combined because the CPU only ever
	// writes here, front to back, and never reads a byte back.
	for (auto& frameConstantBuffer: _frameConstantBuffers) {
		frameConstantBuffer = NS::TransferPtr(_nativeDevice->newBuffer(
			FRAME_CONSTANTS_CAPACITY,
			MTL::ResourceStorageModeShared | MTL::ResourceCPUCacheModeWriteCombined));
		if (not frameConstantBuffer) [[unlikely]] {
			constexpr auto error = "MetalGraphicDevice::MetalGraphicDevice: Could not allocate the frame constant ring!";
			CE_CORE_ERROR(error);
			throw std::runtime_error(error);
		}

		// The name a GPU capture shows for it, which is how a slice is told apart from a vertex buffer there
		frameConstantBuffer->setLabel(NS::String::string("FrameConstants", NS::UTF8StringEncoding));
	}

	// Throws on its own if the bundled Main.metallib is missing or unreadable.
	_shaderLibrary = MetalShaderLibrary(_nativeDevice.get());
}

MetalGraphicDevice::~MetalGraphicDevice() {
	// A moved-from device has no semaphore and nothing in flight to wait for.
	if (not _framesInFlight)
		return;

	// A buffer still open belongs to a frame nobody presented. Committed, because a buffer that is never committed
	// never completes: its handler would never run, and the loop below would wait for that slot forever.
	if (_nativeFrameCommandBuffer) [[unlikely]] {
		_nativeFrameCommandBuffer->commit();
		_nativeFrameCommandBuffer.reset();
	}

	// Taking back every slot is only possible once every handler has run, which is exactly "the GPU has finished all
	// this device gave it". From here on no handler can touch the semaphore, and the unique_ptr can free it.
	for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i)
		_framesInFlight->acquire();
}

std::shared_ptr<I_ShaderModule> MetalGraphicDevice::CreateShaderModule(const ShaderModuleDescriptor& descriptor) {
	// A lookup, not a compile: the .metallib was built by CMake, so the module turns the descriptor's name into the
	// entry point that implements it. The OpenGL device does the opposite with the same descriptor - which is the
	// point of the descriptor carrying neither.
	return std::make_shared<MetalShaderModule>(_shaderLibrary, descriptor);
}

std::shared_ptr<I_PipelineState> MetalGraphicDevice::CreatePipelineState(const PipelineDescriptor& descriptor) {
	return std::make_shared<MetalPipelineState>(_nativeDevice.get(), descriptor);
}

std::shared_ptr<I_IndexBuffer> MetalGraphicDevice::CreateIndexBuffer(const std::span<const u32> indices) {
	return std::make_shared<MetalIndexBuffer>(_nativeDevice.get(), indices.data(), indices.size());
}

std::shared_ptr<I_VertexBuffer> MetalGraphicDevice::CreateVertexBuffer(const std::span<const f32> vertices, const BufferLayout& layout) {
	return std::make_shared<MetalVertexBuffer>(_nativeDevice.get(), vertices.data(), vertices.size(), layout);
}

std::shared_ptr<I_Texture> MetalGraphicDevice::CreateTexture(const TextureDescriptor& descriptor) {
	return std::make_shared<MetalTexture>(_nativeDevice.get(), descriptor);
}

std::unique_ptr<I_CommandEncoder> MetalGraphicDevice::BeginRenderPass(const RenderPassDescriptor& descriptor) {
	assert(descriptor.colorCount <= Types::MAX_COLOR_ATTACHMENTS and "MetalGraphicDevice::BeginRenderPass: More color attachments than the APIs allow!");

	if (descriptor.width == 0 or descriptor.height == 0) [[unlikely]] {
		CE_CORE_WARN("MetalGraphicDevice::BeginRenderPass: The render area is {}x{}; the pass is skipped.", descriptor.width, descriptor.height);
		return nullptr;
	}

	const auto& [target_0, loadAction_0, storeAction_0, clearColor_0] = descriptor.colors[0];

	// Null target means the swapchain's back buffer, which is the only attachment that can be missing: a frame whose
	// drawable was never acquired has nowhere to present to, and that is normal under memory pressure. A pass into a
	// texture of ours needs no drawable at all, which is why the check moved inside this branch.
	const MTL::Texture* nativeColorTexture = nullptr;
	if (target_0) {
		assert(target_0->GetGraphicApi() == Types::GraphicsApi::Metal and "MetalGraphicDevice::BeginRenderPass: The color target belongs to another backend!");
		nativeColorTexture = static_cast<const MetalTexture*>(target_0)->GetTexture();
	}
	else if (_nativeFrameDrawable) {
		nativeColorTexture = _nativeFrameDrawable->texture();
	}

	if (not nativeColorTexture) [[unlikely]] {
		CE_CORE_WARN("MetalGraphicDevice::BeginRenderPass: No frame target was published; the pass is skipped.");
		return nullptr;
	}

	const auto passDescriptor = NS::TransferPtr(MTL::RenderPassDescriptor::alloc()->init());

	auto* const colorAttachment_0 = passDescriptor->colorAttachments()->object(0);
	colorAttachment_0->setTexture(nativeColorTexture);
	colorAttachment_0->setLoadAction(ToMetal(loadAction_0));
	colorAttachment_0->setStoreAction(ToMetal(storeAction_0));
	colorAttachment_0->setClearColor(MTL::ClearColor::Make(clearColor_0.r, clearColor_0.g, clearColor_0.b, clearColor_0.a));

	// The extra attachments a G-buffer would use are reachable now, and the loop is the only part of this that had to
	// wait for the texture type.
	for (u32 i = 1; i < descriptor.colorCount; ++i) {
		const auto& [target_i, loadAction_i, storeAction_i, clearColor_i] = descriptor.colors[i];
		if (not target_i) [[unlikely]] {
			CE_CORE_WARN("MetalGraphicDevice::BeginRenderPass: Colour attachment {} has no texture; the pass is skipped.", i);
			return nullptr;
		}

		assert(target_i->GetGraphicApi() == Types::GraphicsApi::Metal and "MetalGraphicDevice::BeginRenderPass: A color target belongs to another backend!");

		const auto& extraClear = clearColor_i;
		auto* const attachment = passDescriptor->colorAttachments()->object(i);
		attachment->setTexture(static_cast<const MetalTexture*>(target_i)->GetTexture());
		attachment->setLoadAction(ToMetal(loadAction_i));
		attachment->setStoreAction(ToMetal(storeAction_i));
		attachment->setClearColor(MTL::ClearColor::Make(extraClear.r, extraClear.g, extraClear.b, extraClear.a));
	}

	// This is where a tile-based GPU earns its keep: a DontCare store on the depth buffer means the tile is simply
	// never written back to memory, so the depth buffer costs bandwidth only while the pass is running.
	if (descriptor.depth.enabled and descriptor.depth.target) {
		assert(descriptor.depth.target->GetGraphicApi() == Types::GraphicsApi::Metal and "MetalGraphicDevice::BeginRenderPass: The depth target belongs to another backend!");

		auto* const depthAttachment = passDescriptor->depthAttachment();
		depthAttachment->setTexture(static_cast<const MetalTexture*>(descriptor.depth.target)->GetTexture());
		depthAttachment->setLoadAction(ToMetal(descriptor.depth.loadAction));
		depthAttachment->setStoreAction(ToMetal(descriptor.depth.storeAction));
		depthAttachment->setClearDepth(descriptor.depth.clearDepth);
	}

	passDescriptor->setRenderTargetWidth(descriptor.width);
	passDescriptor->setRenderTargetHeight(descriptor.height);

	return std::make_unique<MetalCommandEncoder>(*this, GetFrameCommandBuffer(), passDescriptor.get());
}

void MetalGraphicDevice::SetFrameTarget(CA::MetalDrawable* drawable) {
	// A buffer still held while the target is being cleared belongs to a frame nobody presented - a renderer torn
	// down mid-frame, or a pass opened outside one. It has to reach the GPU anyway: the encoder is closed and the
	// resources it read are about to be released.
	if (not drawable and _nativeFrameCommandBuffer) [[unlikely]] {
		_nativeFrameCommandBuffer->commit();
		_nativeFrameCommandBuffer.reset();
	}

	_nativeFrameDrawable = drawable;
}

MTL::CommandBuffer* MetalGraphicDevice::GetFrameCommandBuffer() {
	// Every pass after the first of the frame: the buffer is open, and the slot is already ours.
	if (_nativeFrameCommandBuffer)
		return _nativeFrameCommandBuffer.get();

	// The first pass of a new frame. Blocks only when MAX_FRAMES_IN_FLIGHT frames are still on the GPU, and only
	// until the oldest of them completes.
	_framesInFlight->acquire();

	// Autoreleased by the queue, so retained: it has to survive until the swapchain commits it at the end of the
	// frame, which is well past the pool this was handed out in.
	_nativeFrameCommandBuffer = NS::RetainPtr(_nativeCommandQueue->commandBuffer());
	if (not _nativeFrameCommandBuffer) [[unlikely]] {
		CE_CORE_ERROR("MetalGraphicDevice::GetFrameCommandBuffer: The queue handed back no command buffer!");

		// No buffer means no completion handler, and nobody else would ever give this slot back.
		_framesInFlight->release();
		return nullptr;
	}

	// Runs on one of Metal's threads once the GPU has finished every command in the buffer - or given up on them: a
	// buffer that fails completes too, so the slot comes back either way. Added before the commit, which is the only
	// time Metal accepts it.
	_nativeFrameCommandBuffer->addCompletedHandler([framesInFlight = _framesInFlight.get()](MTL::CommandBuffer*) {
		framesInFlight->release();
	});

	// A new frame moves to the next slice and starts it empty. Safe to overwrite: the acquire above returned, so the
	// frame that wrote this slice MAX_FRAMES_IN_FLIGHT frames ago has completed.
	_frameIndex = (_frameIndex + 1) % MAX_FRAMES_IN_FLIGHT;
	_frameConstantOffset = 0;

	return _nativeFrameCommandBuffer.get();
}

NS::SharedPtr<MTL::CommandBuffer> MetalGraphicDevice::TakeFrameCommandBuffer() {
	return std::move(_nativeFrameCommandBuffer);
}

MetalConstantAllocation MetalGraphicDevice::AllocateFrameConstants(const std::span<const std::byte> data) {
	assert(_nativeFrameCommandBuffer and "MetalGraphicDevice::AllocateFrameConstants: No frame is open, so no slice of the ring is safe to write!");
	assert(not data.empty() and "MetalGraphicDevice::AllocateFrameConstants: There are no constants to copy!");

	// Rounded up at the start, not at the end: Metal checks where a block begins, and the previous one can have ended
	// anywhere. The alignment is a power of two, so the mask does what a division would.
	const auto offset = (_frameConstantOffset + CONSTANT_OFFSET_ALIGNMENT - 1) & ~(CONSTANT_OFFSET_ALIGNMENT - 1);
	if (offset + data.size() > FRAME_CONSTANTS_CAPACITY) [[unlikely]]
		return {};

	auto* const nativeBuffer = _frameConstantBuffers[_frameIndex].get();
	std::memcpy(static_cast<std::byte*>(nativeBuffer->contents()) + offset, data.data(), data.size());

	_frameConstantOffset = offset + data.size();

	return {
		.buffer = nativeBuffer,
		.offset = offset
	};
}

}
