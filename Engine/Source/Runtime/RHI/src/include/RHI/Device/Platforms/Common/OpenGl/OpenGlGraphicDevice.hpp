//
// Module: CelestialEngine/Engine/Source/Runtime/Core/Device/Platforms/Common/OpenGl
// File: OpenGlGraphicDevice.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef CE_RHI_DEVICE_OPENGLGRAPHICDEVICE_HPP
#define CE_RHI_DEVICE_OPENGLGRAPHICDEVICE_HPP

#include "RHI/Device/I_GraphicDevice.hpp"
#include "RHI/Conversion/Platforms/Common/OpenGl/OpenGlConversion.hpp"
#include "Define/DynamicLinker.hpp"

#include <array>
#include <memory>


namespace CE::RHI {

class CE_RHI_API OpenGlGraphicDevice final: public I_GraphicDeviceBase<Types::GraphicsApi::OpenGL> {
public:
	~OpenGlGraphicDevice() override;

public:
	[[nodiscard]] std::shared_ptr<I_ShaderModule> CreateShaderModule(const ShaderModuleDescriptor& descriptor) override;
	[[nodiscard]] std::shared_ptr<I_PipelineState> CreatePipelineState(const PipelineDescriptor& descriptor) override;
	[[nodiscard]] std::shared_ptr<I_IndexBuffer> CreateIndexBuffer(std::span<const u32> indices) override;
	[[nodiscard]] std::shared_ptr<I_VertexBuffer> CreateVertexBuffer(std::span<const f32> data, const BufferLayout& layout) override;

	[[nodiscard]] std::shared_ptr<I_Texture> CreateTexture(const TextureDescriptor& descriptor) override;

	[[nodiscard]] std::shared_ptr<I_Sampler> CreateSampler(const SamplerDescriptor& descriptor) override;

	[[nodiscard]] std::unique_ptr<I_CommandEncoder> BeginRenderPass(const RenderPassDescriptor& descriptor) override;

public:
	/// Clip space runs from -1 at the near plane to 1 at the far one here; the projection matrix has to be built for
	/// that range, and this is the only object that can say so without naming the API somewhere else.
	[[nodiscard]] Types::ClipConvention GetClipConvention() const override { return Types::ClipConvention::NegativeOneToOne; }

private:
	std::array<u32, OPENGL_UNIFORM_BUFFER_BINDINGS> _uniformBuffers{};

	u32 _frameBuffer = 0;
};

}

#endif //CE_RHI_DEVICE_OPENGLGRAPHICDEVICE_HPP
