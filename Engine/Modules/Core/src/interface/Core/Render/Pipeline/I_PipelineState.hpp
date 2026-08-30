//
// Module: CelestialEngine/Engine/Modules/Core/Render/Pipeline
// File: I_PipelineState.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_PIPELINE_I_PIPELINESTATE_HPP
#define CE_CORE_RENDER_PIPELINE_I_PIPELINESTATE_HPP

#include "Types/Types.hpp"


namespace CE::Core {

struct PipelineDescriptor;

class I_PipelineState {
public:
	virtual ~I_PipelineState() = default;

public:
	[[nodiscard]] virtual const PipelineDescriptor& GetDescriptor() const = 0;
	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;
};

template<Types::GraphicsApi Api>
class I_PipelineStateBase: public I_PipelineState {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return _st_Api; }

private:
	static constexpr Types::GraphicsApi _st_Api = Api;
};

}

#endif //CE_CORE_RENDER_PIPELINE_I_PIPELINESTATE_HPP
