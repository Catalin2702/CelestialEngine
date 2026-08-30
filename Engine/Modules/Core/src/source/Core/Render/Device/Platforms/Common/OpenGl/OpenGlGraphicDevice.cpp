//
// Module: CelestialEngine/Engine/Modules/Core/Device/Platforms/Common/OpenGl
// File: OpenGlGraphicDevice.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#include "Core/Render/Device/Platforms/Common/OpenGl/OpenGlGraphicDevice.hpp"
#include "Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp"
#include "Core/Render/Command/I_CommandEncoder.hpp"
#include "Core/Render/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderModule.hpp"

namespace CE::Core {

std::shared_ptr<I_ShaderModule> OpenGlGraphicDevice::CreateShaderModule(const ShaderModuleDescriptor& descriptor) {
	return std::make_shared<OpenGlShaderModule>(descriptor);
}

std::shared_ptr<I_PipelineState> OpenGlGraphicDevice::CreatePipelineState(const PipelineDescriptor& descriptor) {
	return std::make_shared<OpenGlPipelineState>(descriptor);
}

std::shared_ptr<I_IndexBuffer> OpenGlGraphicDevice::CreateIndexBuffer(const std::span<const u32> indices) {
	return std::make_shared<OpenGlIndexBuffer>(indices.data(), indices.size());
}

std::shared_ptr<I_VertexBuffer> OpenGlGraphicDevice::CreateVertexBuffer(const std::span<const f32> data, const BufferLayout& layout) {
	return std::make_shared<OpenGlVertexBuffer>(data.data(), data.size(), layout);
}

std::unique_ptr<I_CommandEncoder> OpenGlGraphicDevice::BeginRenderPass(const RenderPassDescriptor&) {
	return nullptr;
}

}
