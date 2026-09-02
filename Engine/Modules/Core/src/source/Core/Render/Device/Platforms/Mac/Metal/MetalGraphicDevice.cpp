//
// Module: CelestialEngine/Engine/Modules/Core/Device/Platforms/Mac/Metal
// File: MetalGraphicDevice.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include "Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#include "Tools/Log/Log.hpp"


namespace CE::Core {

MetalGraphicDevice::MetalGraphicDevice() {
	_device = NS::TransferPtr(MTL::CreateSystemDefaultDevice());
	if (not _device) [[unlikely]] {
		constexpr auto error = "MetalGraphicDevice::MetalGraphicDevice: Could not create the MTL::Device!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_commandQueue = NS::TransferPtr(_device->newCommandQueue());
	if (not _commandQueue) [[unlikely]] {
		constexpr auto error = "MetalGraphicDevice::MetalGraphicDevice: Could not create the MTL::CommandQueue!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_shaderLibrary = MetalShaderLibrary(_device.get());
}

MetalGraphicDevice::MetalGraphicDevice(MetalGraphicDevice&& other) noexcept {
	if (this == &other) [[unlikely]]
		return;

	_device = std::move(other._device);
	_commandQueue = std::move(other._commandQueue);
	_shaderLibrary = std::move(other._shaderLibrary);
}

MetalGraphicDevice& MetalGraphicDevice::operator = (MetalGraphicDevice&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	_device = std::move(other._device);
	_commandQueue = std::move(other._commandQueue);
	_shaderLibrary = std::move(other._shaderLibrary);

	return *this;
}

std::shared_ptr<I_ShaderModule> MetalGraphicDevice::CreateShaderModule(const ShaderModuleDescriptor&) {
	return nullptr;
}

std::shared_ptr<I_PipelineState> MetalGraphicDevice::CreatePipelineState(const PipelineDescriptor&) {
	return nullptr;
}

std::shared_ptr<I_IndexBuffer> MetalGraphicDevice::CreateIndexBuffer(std::span<const u32>) {
	return nullptr;
}

std::shared_ptr<I_VertexBuffer> MetalGraphicDevice::CreateVertexBuffer(std::span<const f32>, const BufferLayout&) {
	return nullptr;
}

std::unique_ptr<I_CommandEncoder> MetalGraphicDevice::BeginRenderPass(const RenderPassDescriptor&) {
	return nullptr;
}

}
