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


namespace CE::Core {

struct PipelineDescriptor;

class I_PipelineState {
public:
	virtual ~I_PipelineState() = default;

public:
	[[nodiscard]] virtual const PipelineDescriptor& GetDescriptor() const = 0;
};

}

#endif //CE_CORE_RENDER_PIPELINE_I_PIPELINESTATE_HPP
