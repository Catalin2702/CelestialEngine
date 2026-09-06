//
// Module: CelestialEngine/Engine/Modules/Core/Device/Platforms/Mac/Metal
// File: MetalGraphicDevice.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#include "Core/Render/Buffer/Platforms/Mac/Metal/MetalBuffer.hpp"
#include "Core/Render/Command/Platforms/Mac/Metal/MetalCommandEncoder.hpp"
#include "Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp"
#include "Core/Render/Pipeline/Platforms/Mac/Metal/MetalPipelineState.hpp"
#include "Core/Render/Texture/Platforms/Mac/Metal/MetalTexture.hpp"
#include "Tools/Tools.hpp"
#include "Types/Types.hpp"

#include <Metal/Metal.hpp>

#include <cassert>
#include <utility>

#include "Core/Render/Command/RenderPassDescriptor.hpp"
#include "QuartzCore/CAMetalDrawable.hpp"


namespace CE::Core {

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

	// Throws on its own if the bundled Main.metallib is missing or unreadable.
	_shaderLibrary = MetalShaderLibrary(_nativeDevice.get());
}

MetalGraphicDevice::~MetalGraphicDevice() = default;

std::shared_ptr<I_ShaderModule> MetalGraphicDevice::CreateShaderModule(const ShaderModuleDescriptor& descriptor) {
	// A lookup, not a compile: the .metallib was built by CMake, so descriptor.source is ignored here and
	// descriptor.entryPoint is what matters. The OpenGL device does the opposite with the same descriptor.
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

	const auto colorAttachment_0 = passDescriptor->colorAttachments()->object(0);
	colorAttachment_0->setTexture(_nativeFrameDrawable->texture());
	colorAttachment_0->setLoadAction(Types::ToMetal(loadAction_0));
	colorAttachment_0->setStoreAction(Types::ToMetal(storeAction_0));
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
		const auto attachment = passDescriptor->colorAttachments()->object(i);
		attachment->setTexture(static_cast<const MetalTexture*>(target_i)->GetTexture());
		attachment->setLoadAction(Types::ToMetal(loadAction_i));
		attachment->setStoreAction(Types::ToMetal(storeAction_i));
		attachment->setClearColor(MTL::ClearColor::Make(extraClear.r, extraClear.g, extraClear.b, extraClear.a));
	}

	// This is where a tile-based GPU earns its keep: a DontCare store on the depth buffer means the tile is simply
	// never written back to memory, so the depth buffer costs bandwidth only while the pass is running.
	if (descriptor.depth.enabled and descriptor.depth.target) {
		assert(descriptor.depth.target->GetGraphicApi() == Types::GraphicsApi::Metal and "MetalGraphicDevice::BeginRenderPass: The depth target belongs to another backend!");

		const auto depthAttachment = passDescriptor->depthAttachment();
		depthAttachment->setTexture(static_cast<const MetalTexture*>(descriptor.depth.target)->GetTexture());
		depthAttachment->setLoadAction(Types::ToMetal(descriptor.depth.loadAction));
		depthAttachment->setStoreAction(Types::ToMetal(descriptor.depth.storeAction));
		depthAttachment->setClearDepth(descriptor.depth.clearDepth);
	}

	passDescriptor->setRenderTargetWidth(descriptor.width);
	passDescriptor->setRenderTargetHeight(descriptor.height);

	return std::make_unique<MetalCommandEncoder>(GetFrameCommandBuffer(), passDescriptor.get());
}

void MetalGraphicDevice::SetFrameTarget(CA::MetalDrawable* drawable, MTL::Texture* depthTexture) {
	// A buffer still held while the target is being cleared belongs to a frame nobody presented - a renderer torn
	// down mid-frame, or a pass opened outside one. It has to reach the GPU anyway: the encoder is closed and the
	// resources it read are about to be released.
	if (not drawable and _nativeFrameCommandBuffer) [[unlikely]] {
		_nativeFrameCommandBuffer->commit();
		_nativeFrameCommandBuffer.reset();
	}

	_nativeFrameDrawable = drawable;
	_nativeFrameDepthTexture = depthTexture;
}

MTL::CommandBuffer* MetalGraphicDevice::GetFrameCommandBuffer() {
	if (not _nativeFrameCommandBuffer) {
		// Autoreleased by the queue, so retained: it has to survive until the swapchain commits it at the end of the
		// frame, which is well past the pool this was handed out in.
		_nativeFrameCommandBuffer = NS::RetainPtr(_nativeCommandQueue->commandBuffer());
		if (not _nativeFrameCommandBuffer) [[unlikely]]
			CE_CORE_ERROR("MetalGraphicDevice::GetFrameCommandBuffer: The queue handed back no command buffer!");
	}

	return _nativeFrameCommandBuffer.get();
}

NS::SharedPtr<MTL::CommandBuffer> MetalGraphicDevice::TakeFrameCommandBuffer() {
	return std::move(_nativeFrameCommandBuffer);
}

}
