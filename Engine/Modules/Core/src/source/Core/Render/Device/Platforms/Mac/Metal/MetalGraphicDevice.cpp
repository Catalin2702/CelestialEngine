//
// Module: CelestialEngine/Engine/Modules/Core/Device/Platforms/Mac/Metal
// File: MetalGraphicDevice.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include "Core/Render/Buffer/Platforms/Mac/Metal/MetalBuffer.hpp"
#include "Core/Render/Command/Platforms/Mac/Metal/MetalCommandEncoder.hpp"
#include "Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp"
#include "Core/Render/Pipeline/Platforms/Mac/Metal/MetalPipelineState.hpp"
#include "Tools/Tools.hpp"

#include <Metal/Metal.hpp>

#include <cassert>

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

std::unique_ptr<I_CommandEncoder> MetalGraphicDevice::BeginRenderPass(const RenderPassDescriptor& descriptor) {
	assert(descriptor.colorCount <= Types::MAX_COLOR_ATTACHMENTS and "MetalGraphicDevice::BeginRenderPass: More color attachments than the APIs allow!");

	if (descriptor.width == 0 or descriptor.height == 0) [[unlikely]] {
		CE_CORE_WARN("MetalGraphicDevice::BeginRenderPass: The render area is {}x{}; the pass is skipped.", descriptor.width, descriptor.height);
		return nullptr;
	}

	if (not _nativeFrameDrawable) [[unlikely]] {
		// No drawable was acquired for this frame - the layer had none free, or BeginFrame was never called. Skipped,
		// not fatal: the next frame gets one.
	}

	assert(descriptor.colors[0].target == nullptr and descriptor.depth.target == nullptr and "MetalGraphicDevice::BeginRenderPass: Rendering to a texutre is not supported yet!");

	const auto passDescriptor = NS::TransferPtr(MTL::RenderPassDescriptor::alloc()->init());

	// Attachment 0 is the drawable. The extra attachments a G-buffer would use need their own textures, which is the
	// same missing texture type - so colorCount above 1 is not reachable yet.
	const auto& color0 = descriptor.colors[0];
	const auto& clearColor = color0.clearColor;
	const auto colorAttachment = passDescriptor->colorAttachments()->object(0);
	colorAttachment->setTexture(_nativeFrameDrawable->texture());
	colorAttachment->setLoadAction(Types::ToMetal(color0.loadAction));
	colorAttachment->setStoreAction(Types::ToMetal(color0.storeAction));
	colorAttachment->setClearColor(MTL::ClearColor::Make(clearColor.r, clearColor.g, clearColor.b, clearColor.a));

	// This is where a tile-based GPU earns its keep: a DontCare store on the depth buffer means the tile is simply
	// never written back to memory, so the depth buffer costs bandwidth only while the pass is running.
	if (descriptor.depth.enabled and _nativeFrameDepthTexture) {
		const auto depthAttachment = passDescriptor->depthAttachment();
		depthAttachment->setTexture(_nativeFrameDepthTexture);
		depthAttachment->setLoadAction(Types::ToMetal(descriptor.depth.loadAction));
		depthAttachment->setStoreAction(Types::ToMetal(descriptor.depth.storeAction));
		depthAttachment->setClearDepth(descriptor.depth.clearDepth);
	}

	passDescriptor->setRenderTargetWidth(descriptor.width);
	passDescriptor->setRenderTargetHeight(descriptor.height);

	return std::make_unique<MetalCommandEncoder>(_nativeCommandQueue.get(), passDescriptor.get());
}

void MetalGraphicDevice::SetFrameTarget(CA::MetalDrawable* drawable, MTL::Texture* depthTexture) {
	_nativeFrameDrawable = drawable;
	_nativeFrameDepthTexture = depthTexture;
}

}
