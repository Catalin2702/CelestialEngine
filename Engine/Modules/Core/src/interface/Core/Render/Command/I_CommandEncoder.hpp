//
// Module: CelestialEngine/Engine/Modules/Core/Render/Command
// File: I_CommandEncoder.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#pragma once

#ifndef CE_CORE_RENDER_COMMAND_I_COMMANDENCODER_HPP
#define CE_CORE_RENDER_COMMAND_I_COMMANDENCODER_HPP

#include "Types/Types.hpp"


namespace CE::Core {

class I_IndexBuffer;
class I_VertexBuffer;

class I_PipelineState;

struct Viewport;

class I_CommandEncoder {
public:
	virtual ~I_CommandEncoder() = default;

public:
	virtual void SetPipelineState(const I_PipelineState& pipelineState) = 0;
	virtual void SetIndexBuffer(const I_IndexBuffer& indexBuffer) = 0;
	virtual void SetVertexBuffer(const I_VertexBuffer& vertexBuffer) = 0;
	virtual void SetViewport(const Viewport& viewport) = 0;

public:
	virtual void DrawIndexed(u32 indexCount, u32 firstIndex, u32 baseVertex) = 0;
	virtual void End() = 0;

public:
	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;
};

template <Types::GraphicsApi Api>
class I_CommandEncoderBase: public I_CommandEncoder {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return _api; }

private:
	static constexpr Types::GraphicsApi _api = Api;
};

}

#endif //CE_CORE_RENDER_COMMAND_I_COMMANDENCODER_HPP
