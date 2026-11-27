//
// Module: CelestialEngine/Engine/Source/Runtime/RHI/Command
// File: I_CommandEncoder.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-11-27
//

#pragma once

#ifndef CE_RHI_COMMAND_I_COMMANDENCODER_HPP
#define CE_RHI_COMMAND_I_COMMANDENCODER_HPP

#include "Types/Types.hpp"

#include <span>

namespace CE::RHI {

class I_IndexBuffer;
class I_VertexBuffer;

class I_PipelineState;

class I_Sampler;
class I_Texture;

struct Viewport;

class I_CommandEncoder {
public:
	virtual ~I_CommandEncoder() = default;

public:
	virtual void DrawIndexed(u32 indexCount, u32 firstIndex, u32 baseVertex) = 0;

	virtual void End() = 0;

public:
	virtual void SetPipelineState(const I_PipelineState& pipelineState) = 0;

	virtual void SetIndexBuffer(const I_IndexBuffer& indexBuffer) = 0;

	virtual void SetVertexBuffer(const I_VertexBuffer& vertexBuffer) = 0;

	virtual void SetViewport(const Viewport& viewport) = 0;

	/**
	 * @brief Binds a texture the fragment stage can read, at the given slot
	 * @param slot The binding of the shader's sampler2D - [[texture(slot)]] in MSL, texture unit `slot` in GL; below
	 *		  MAX_TEXTURE_SLOTS
	 * @param texture
	 * @details A shader that only fetches by pixel coordinate - the composite - needs nothing else. One that samples
	 *			at UV coordinates also needs SetFragmentSampler at the same slot.
	 */
	virtual void SetFragmentTexture(u32 slot, const I_Texture& texture) = 0;

	/**
	 * @brief Binds the sampler a fragment shader reads the texture at the same slot with
	 * @param slot The binding of the shader's sampler2D - [[sampler(slot)]] in MSL, texture unit `slot` in GL; below
	 *		  MAX_TEXTURE_SLOTS
	 * @param sampler
	 */
	virtual void SetFragmentSampler(u32 slot, const I_Sampler& sampler) = 0;

	virtual void SetVertexConstants(u32 slot, std::span<const std::byte> data) = 0;

	virtual void SetFragmentConstants(u32 slot, std::span<const std::byte> data) = 0;

public:
	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;
};

template <Types::GraphicsApi Api>
class I_CommandEncoderBase: public I_CommandEncoder {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return Api; }
};

}

#endif //CE_RHI_COMMAND_I_COMMANDENCODER_HPP
