//
// Module: CelestialEngine/Engine/Modules/Core/Render/Device
// File: I_GraphicDevice.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_DEVICE_I_GRAPHICDEVICE_HPP
#define CE_CORE_RENDER_DEVICE_I_GRAPHICDEVICE_HPP

#include "Types/Var/Vars.hpp"

#include <memory>
#include <span>


namespace CE::Core {

class I_CommandEncoder;
class RenderPassDescriptor;

class I_IndexBuffer;
class I_VertexBuffer;

struct PipelineDescriptor;
class I_PipelineState;

class BufferLayout;

class I_GraphicDevice {
public:
	virtual ~I_GraphicDevice() = default;

public:
	[[nodiscard]] virtual std::shared_ptr<I_PipelineState> CreatePipelineState(const PipelineDescriptor& descriptor) = 0;
	[[nodiscard]] virtual std::shared_ptr<I_IndexBuffer> CreateIndexBuffer(std::span<const u32> indices) = 0;
	[[nodiscard]] virtual std::shared_ptr<I_VertexBuffer> CreateVertexBuffer(std::span<const byte> data, const BufferLayout& layout) = 0;

	[[nodiscard]] virtual std::unique_ptr<I_CommandEncoder> BeginRenderPass(const RenderPassDescriptor& descriptor) = 0;
};

}

#endif //CE_CORE_RENDER_DEVICE_I_GRAPHICDEVICE_HPP
