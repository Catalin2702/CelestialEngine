//
// Module: CelestialEngine/Engine/Modules/Core/Render/Device
// File: I_GraphicDevice.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-09-18
//

#pragma once

#ifndef CE_CORE_RENDER_DEVICE_I_GRAPHICDEVICE_HPP
#define CE_CORE_RENDER_DEVICE_I_GRAPHICDEVICE_HPP

#include "Types/Types.hpp"

#include <memory>
#include <span>


namespace CE::Core {

class I_CommandEncoder;
struct RenderPassDescriptor;

class I_IndexBuffer;
class I_VertexBuffer;

struct PipelineDescriptor;
class I_PipelineState;

struct ShaderModuleDescriptor;
class I_ShaderModule;

class BufferLayout;

struct TextureDescriptor;
class I_Texture;

class I_GraphicDevice {
public:
	virtual ~I_GraphicDevice() = default;

public:
	[[nodiscard]] static std::unique_ptr<I_GraphicDevice> MakeDevice(Types::GraphicsApi api);

public:
	/**
	 * @brief Produces one shader stage from the engine's name for it
	 * @details Compiles it or looks it up, whichever the backend needs, and finds the artifact itself - which is why
	 *			the descriptor carries a name and not a source or a path. Shared, so the same module can back several
	 *			pipelines without paying for the compile again.
	 */
	[[nodiscard]] virtual std::shared_ptr<I_ShaderModule> CreateShaderModule(const ShaderModuleDescriptor& descriptor) = 0;
	[[nodiscard]] virtual std::shared_ptr<I_PipelineState> CreatePipelineState(const PipelineDescriptor& descriptor) = 0;
	[[nodiscard]] virtual std::shared_ptr<I_IndexBuffer> CreateIndexBuffer(std::span<const u32> indices) = 0;
	[[nodiscard]] virtual std::shared_ptr<I_VertexBuffer> CreateVertexBuffer(std::span<const f32> data, const BufferLayout& layout) = 0;

	/**
	 * @brief Allocates a texture, today only ever a render target
	 * @details Shared like the other resources: a texture outlives the pass that writes it and the one that reads it,
	 *			and the renderer's scene target is held across frames.
	 */
	[[nodiscard]] virtual std::shared_ptr<I_Texture> CreateTexture(const TextureDescriptor& descriptor) = 0;

	[[nodiscard]] virtual std::unique_ptr<I_CommandEncoder> BeginRenderPass(const RenderPassDescriptor& descriptor) = 0;

public:
	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() = 0;

	/**
	 * @brief Gets the depth range this backend's clip space maps onto
	 * @details Backends disagree on it, and a projection matrix built for the wrong one puts half the scene behind
	 *			the near plane. It is asked of the device rather than derived from the API by whoever needs it: the
	 *			device is the one object that always knows, and a camera has no business naming a backend.
	 */
	[[nodiscard]] virtual Types::ClipConvention GetClipConvention() const = 0;
};

template<Types::GraphicsApi Api>
class I_GraphicDeviceBase: public I_GraphicDevice {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() override { return Api; }
};

}

#endif //CE_CORE_RENDER_DEVICE_I_GRAPHICDEVICE_HPP
