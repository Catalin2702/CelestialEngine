//
// Module: CelestialEngine/Engine/Modules/Core/Device/Platforms/Mac/Metal
// File: MetalGraphicDevice.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#pragma once

#ifndef CE_CORE_RENDER_DEVICE_METALGRAPHICDEVICE_HPP
#define CE_CORE_RENDER_DEVICE_METALGRAPHICDEVICE_HPP

#include "Core/Render/Device/I_GraphicDevice.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>


namespace CE::Core {

class CE_CORE_API MetalGraphicDevice final: public I_GraphicDeviceBase<Types::GraphicsApi::Metal> {
public:
	MetalGraphicDevice();

	MetalGraphicDevice(const MetalGraphicDevice&) = delete;

	MetalGraphicDevice(MetalGraphicDevice&& other) noexcept;

public:
	MetalGraphicDevice& operator = (const MetalGraphicDevice&) = delete;
	MetalGraphicDevice& operator = (MetalGraphicDevice&& other) noexcept;

public:
	[[nodiscard]] std::shared_ptr<I_ShaderModule> CreateShaderModule(const ShaderModuleDescriptor& descriptor) override;
	[[nodiscard]] std::shared_ptr<I_PipelineState> CreatePipelineState(const PipelineDescriptor& descriptor) override;
	[[nodiscard]] std::shared_ptr<I_IndexBuffer> CreateIndexBuffer(std::span<const u32> indices) override;
	[[nodiscard]] std::shared_ptr<I_VertexBuffer> CreateVertexBuffer(std::span<const f32> data, const BufferLayout& layout) override;

	[[nodiscard]] std::unique_ptr<I_CommandEncoder> BeginRenderPass(const RenderPassDescriptor& descriptor) override;

private:
	NS::SharedPtr<MTL::Device> _device;
	NS::SharedPtr<MTL::CommandQueue> _commandQueue;
	MetalShaderLibrary _shaderLibrary;
};

}

#endif //CE_CORE_RENDER_DEVICE_METALGRAPHICDEVICE_HPP
