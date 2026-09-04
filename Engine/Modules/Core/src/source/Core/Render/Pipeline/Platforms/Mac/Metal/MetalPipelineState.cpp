//
// Module: CelestialEngine/Engine/Modules/Core/Render/Pipeline/Platforms/Mac/Metal
// File: MetalPipelineState.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-04
// Updated by: Catalin Chirosca
// Updated: 2026-09-04
//

#include "Core/Render/Pipeline/Platforms/Mac/Metal/MetalPipelineState.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderModule.hpp"
#include "Tools/Tools.hpp"
#include "Types/Types.hpp"
#include "Utility/Range/Enumerate.hpp"

#include <Metal/Metal.hpp>

#include <format>
#include <stdexcept>


namespace CE::Core {

namespace {

/// Checks one stage of the descriptor and returns the MTL::Function behind it.
MTL::Function* ResolveStage(const std::shared_ptr<I_ShaderModule>& module, const Types::ShaderType expected) {
	if (not module) [[unlikely]] {
		const auto error = std::format("ResolveStage: The descriptor has no {} shader!", expected);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	if (module->GetGraphicApi() != Types::GraphicsApi::Metal) [[unlikely]] {
		const auto error = std::format("ResolveStage: The {} shader was built for {}, not Metal!", expected, module->GetGraphicApi());
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	if (module->GetStage() != expected) [[unlikely]] {
		const auto error = std::format("ResolveStage: A {} module was given where the {} stage was expected!", module->GetStage(), expected);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	const auto function = static_cast<const MetalShaderModule&>(*module).GetFunction();
	if (not function) [[unlikely]] {
		const auto error = std::format("ResolveStage: The {} module holds no function. It was moved from.", expected);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	return function;
}

/**
 * @brief Turns the engine's BufferLayout into the MTLVertexDescriptor the pipeline is compiled with
 * @details This is where Metal's "format lives in the pipeline" model is paid for, and it is the better half of the
 *			trade: the layout is validated once at creation instead of being replayed as glVertexAttribPointer calls
 *			on every draw. Attribute i in the layout becomes attribute i in the shader ([[attribute(i)]]), all fed
 *			from one interleaved buffer at Types::METAL_VERTEX_BUFFER_INDEX.
 */
NS::SharedPtr<MTL::VertexDescriptor> MakeVertexDescriptor(const BufferLayout& layout) {
	auto vertexDescriptor = NS::TransferPtr(MTL::VertexDescriptor::alloc()->init());

	for (const auto [index, element]: Utility::Enumerate(layout)) {
		const auto format = Types::ToMetal(element.type);

		if (format == MTL::VertexFormatInvalid) [[unlikely]] {
			const auto error = std::format("MakeVertexDescriptor: Vertex attribute '{}' has no single Metal vertex format!", element.name);
			CE_CORE_ERROR(error);
			throw std::runtime_error(error);
		}

		const auto attribute = vertexDescriptor->attributes()->object(index);
		attribute->setFormat(format);
		attribute->setOffset(element.offset);
		attribute->setBufferIndex(Types::METAL_VERTEX_BUFFER_INDEX);
	}

	// One interleaved buffer, advanced once per vertex. Per-instance data would be a second layout with
	// VertexStepFunctionPerInstance, which is what instancing will add here.
	const auto bufferLayout = vertexDescriptor->layouts()->object(Types::METAL_VERTEX_BUFFER_INDEX);
	bufferLayout->setStride(layout.GetStride());
	bufferLayout->setStepFunction(MTL::VertexStepFunctionPerVertex);
	bufferLayout->setStepRate(1);

	return vertexDescriptor;
}

/// Fills in one colour attachment: its format, and the blend equation that writes into it.
void ConfigureColorAttachment(MTL::RenderPipelineColorAttachmentDescriptor* attachmentDescriptor, const Types::PixelFormat pixelFormat, const BlendState& blendState) {
	attachmentDescriptor->setPixelFormat(Types::ToMetal(pixelFormat));

	if (not blendState.enabled) {
		attachmentDescriptor->setBlendingEnabled(false);
		return;
	}

	attachmentDescriptor->setBlendingEnabled(true);

	attachmentDescriptor->setSourceRGBBlendFactor(Types::ToMetal(blendState.sourceColor));
	attachmentDescriptor->setDestinationRGBBlendFactor(Types::ToMetal(blendState.destinationColor));

	attachmentDescriptor->setSourceAlphaBlendFactor(Types::ToMetal(blendState.sourceAlpha));
	attachmentDescriptor->setDestinationAlphaBlendFactor(Types::ToMetal(blendState.destinationAlpha));

	attachmentDescriptor->setRgbBlendOperation(MTL::BlendOperationAdd);
	attachmentDescriptor->setAlphaBlendOperation(MTL::BlendOperationAdd);
}

}

MetalPipelineState::MetalPipelineState(MTL::Device* nativeDevice, PipelineDescriptor descriptor): _descriptor(std::move(descriptor)) {
	if (not nativeDevice) [[unlikely]] {
		constexpr auto error = "MetalPipelineState::MetalPipelineState: No device to compile the pipeline on!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	if (not _descriptor.blendState.HasValidDestinationFactors()) [[unlikely]] {
		const auto error = std::format("MetalPipelineState::MetalPipelineState: {} is not a legal destination blend factor", Types::BlendFactor::SrcAlphaSaturated);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	const auto vertexFunction = ResolveStage(_descriptor.vertexShader, Types::ShaderType::Vertex);
	const auto fragmentFunction = ResolveStage(_descriptor.fragmentShader, Types::ShaderType::Fragment);

	const auto pipelineDescriptor = NS::TransferPtr(MTL::RenderPipelineDescriptor::alloc()->init());
	pipelineDescriptor->setVertexFunction(vertexFunction);
	pipelineDescriptor->setFragmentFunction(fragmentFunction);

	const auto vertexDescriptor = MakeVertexDescriptor(_descriptor.vertexLayout);
	pipelineDescriptor->setVertexDescriptor(vertexDescriptor.get());

	// Pipeline state on Metal and DirectX 12, a draw-call argument on OpenGL: both halves come out of the same
	// topology, and PrimitiveTopology exists precisely so the descriptor does not have to choose.
	pipelineDescriptor->setInputPrimitiveTopology(Types::ToMetalTopologyClass(_descriptor.topology));

	const auto& [colors, colorCount, depth] = _descriptor.formats;

	if (colorCount > colors.size()) [[unlikely]] {
		const auto error = std::format("MetalPipelineState::MetalPipelineState: The color count ({}) is higher than the colors size ({})!", colorCount, colors.size());
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	for (u32 i = 0; i < colorCount; ++i) {
		ConfigureColorAttachment(pipelineDescriptor->colorAttachments()->object(i), colors[i], _descriptor.blendState);
	}

	if (_descriptor.depthState.testEnabled or _descriptor.depthState.writeEnabled) {
		pipelineDescriptor->setDepthAttachmentPixelFormat(Types::ToMetal(depth));

		if (Types::HasStencil(depth)) {
			pipelineDescriptor->setStencilAttachmentPixelFormat(Types::ToMetal(depth));
		}
	}

	NS::Error* errorPtr = nullptr;
	_nativePipelineState = NS::TransferPtr(nativeDevice->newRenderPipelineState(pipelineDescriptor.get(), &errorPtr));
	if (not _nativePipelineState) [[unlikely]] {
		const auto error = std::format("MetalPipelineState::MetalPipelineState: The pipeline creation failed to compile. Error: {}", errorPtr ? errorPtr->localizedDescription()->utf8String(): "Unknown");
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	// The depth state is its own object in Metal, and it is built unconditionally: a pipeline with no depth test
	// still needs one that says so, otherwise the encoder keeps whatever the previous draw left set.
	const auto depthDescriptor = NS::TransferPtr(MTL::DepthStencilDescriptor::alloc()->init());
	depthDescriptor->setDepthCompareFunction(_descriptor.depthState.testEnabled ? Types::ToMetal(_descriptor.depthState.compareFunc): MTL::CompareFunctionAlways);
	depthDescriptor->setDepthWriteEnabled(_descriptor.depthState.writeEnabled);

	_nativeDepthStencilState = NS::TransferPtr(nativeDevice->newDepthStencilState(depthDescriptor.get()));
	if (not _nativeDepthStencilState) [[unlikely]] {
		constexpr auto error = "MetalPipelineState::MetalPipelineState: The device refused the depth/stencil state!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
}

}
