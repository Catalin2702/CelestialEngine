//
// Module: CelestialEngine/Engine/Modules/Core/Device/Platforms/Common/OpenGl
// File: OpenGlGraphicDevice.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#pragma once

#ifndef CE_CORE_RENDER_DEVICE_OPENGLGRAPHICDEVICE_HPP
#define CE_CORE_RENDER_DEVICE_OPENGLGRAPHICDEVICE_HPP

#include "Core/Render/Device/I_GraphicDevice.hpp"
#include "Types/Render/Render.hpp"

#include <memory>


namespace CE::Core {

class OpenGlGraphicDevice final: public I_GraphicDeviceBase<Types::GraphicsApi::OpenGL> {
public:
	[[nodiscard]] std::shared_ptr<I_ShaderModule> CreateShaderModule(const ShaderModuleDescriptor& descriptor) override;
	[[nodiscard]] std::shared_ptr<I_PipelineState> CreatePipelineState(const PipelineDescriptor& descriptor) override;
	[[nodiscard]] std::shared_ptr<I_IndexBuffer> CreateIndexBuffer(std::span<const u32> indices) override;
	[[nodiscard]] std::shared_ptr<I_VertexBuffer> CreateVertexBuffer(std::span<const f32> data, const BufferLayout& layout) override;

	[[nodiscard]] std::unique_ptr<I_CommandEncoder> BeginRenderPass(const RenderPassDescriptor& descriptor) override;
};

}

#endif //CE_CORE_RENDER_DEVICE_OPENGLGRAPHICDEVICE_HPP
