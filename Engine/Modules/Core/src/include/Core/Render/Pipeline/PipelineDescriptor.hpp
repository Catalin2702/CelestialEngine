//
// Module: CelestialEngine/Engine/Modules/Core/Render/Pipeline
// File: PipelineDescriptor.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_PIPELINE_PIPELINEDESCRIPTOR_HPP
#define CE_CORE_RENDER_PIPELINE_PIPELINEDESCRIPTOR_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Compare/Compare.hpp"


namespace CE::Core {

struct CE_CORE_API DepthState {
	bool testEnabled = true;
	bool writeEnabled = true;
	Types::CompareFunc compareFunc = Types::CompareFunc::Less;
};

struct CE_CORE_API PipelineDescriptor {
public:

};

}

#endif //CE_CORE_RENDER_PIPELINE_PIPELINEDESCRIPTOR_HPP
