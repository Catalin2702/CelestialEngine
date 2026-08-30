//
// Module: CelestialEngine/Engine/Modules/Core/Render/Device
// File: I_GraphicDevice.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#pragma once

#ifndef CE_CORE_RENDER_DEVICE_I_GRAPHICDEVICE_HPP
#define CE_CORE_RENDER_DEVICE_I_GRAPHICDEVICE_HPP

#include "Types/Types.hpp"

#include <memory>
#include <span>


namespace CE::Core {

class I_CommandEncoder;
class RenderPassDescriptor;

class I_IndexBuffer;
class I_VertexBuffer;

struct PipelineDescriptor;
class I_PipelineState;

struct ShaderModuleDescriptor;
class I_ShaderModule;

class BufferLayout;

class I_GraphicDevice {
public:
	virtual ~I_GraphicDevice() = default;

public:
	/// Compiles (OpenGL) or resolves (Metal) one shader stage. Shared, so the same module can back several pipelines
	/// without paying for the compile again.
	[[nodiscard]] virtual std::shared_ptr<I_ShaderModule> CreateShaderModule(const ShaderModuleDescriptor& descriptor) = 0;
	[[nodiscard]] virtual std::shared_ptr<I_PipelineState> CreatePipelineState(const PipelineDescriptor& descriptor) = 0;
	[[nodiscard]] virtual std::shared_ptr<I_IndexBuffer> CreateIndexBuffer(std::span<const u32> indices) = 0;
	[[nodiscard]] virtual std::shared_ptr<I_VertexBuffer> CreateVertexBuffer(std::span<const f32> data, const BufferLayout& layout) = 0;

	[[nodiscard]] virtual std::unique_ptr<I_CommandEncoder> BeginRenderPass(const RenderPassDescriptor& descriptor) = 0;

public:
	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() = 0;
};

template<Types::GraphicsApi Api>
class I_GraphicDeviceBase: public I_GraphicDevice {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() override { return _st_Api; }

private:
	static constexpr Types::GraphicsApi _st_Api = Api;
};

}

#endif //CE_CORE_RENDER_DEVICE_I_GRAPHICDEVICE_HPP
