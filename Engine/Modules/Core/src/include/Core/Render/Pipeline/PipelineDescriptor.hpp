//
// Module: CelestialEngine/Engine/Modules/Core/Render/Pipeline
// File: PipelineDescriptor.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-29
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#pragma once

#ifndef CE_CORE_RENDER_PIPELINE_PIPELINEDESCRIPTOR_HPP
#define CE_CORE_RENDER_PIPELINE_PIPELINEDESCRIPTOR_HPP

#include "Core/Render/Buffer/I_Buffer.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <array>
#include <memory>


namespace CE::Core {

class I_ShaderModule;

struct CE_CORE_API DepthState {
	bool testEnabled = true;
	bool writeEnabled = true;
	Types::CompareFunc compareFunc = Types::CompareFunc::Less;
};

struct CE_CORE_API BlendState {
	bool enabled = true;

	Types::BlendFactor sourceColor = Types::BlendFactor::SrcAlpha;
	Types::BlendFactor destinationColor = Types::BlendFactor::OneMinusSrcAlpha;

	/// The alpha channel is blended separately: reusing the colour factors would multiply the incoming alpha by
	/// itself, which is why straight alpha blending wants One / OneMinusSrcAlpha here.
	Types::BlendFactor sourceAlpha = Types::BlendFactor::One;
	Types::BlendFactor destinationAlpha = Types::BlendFactor::OneMinusSrcAlpha;

	/// Checks the two destination slots against the one factor no API accepts on that side.
	[[nodiscard]] bool HasValidDestinationFactors() const {
		return Types::IsValidDestinationFactor(destinationColor) and Types::IsValidDestinationFactor(destinationAlpha);
	}
};

struct CE_CORE_API RenderTargetFormats {
	std::array<Types::PixelFormat, Types::MAX_COLOR_ATTACHMENTS> colors = {Types::PixelFormat::BGRA8Unorm};
	u32 colorCount = 1;
	Types::PixelFormat depth = Types::PixelFormat::Depth32Float;
};

struct CE_CORE_API PipelineDescriptor {
	/// Shared, not owned: one module compiled once can back several pipelines, and the pipeline must keep it alive
	/// for as long as it might be relinked.
	std::shared_ptr<I_ShaderModule> vertexShader;
	std::shared_ptr<I_ShaderModule> fragmentShader;

	/// Describes how the vertex buffers feed the vertex stage. OpenGL cannot store this in the pipeline - a VAO binds
	/// format and buffers together before GL 4.3, and macOS stops at 4.1 - so there it is only used to validate the
	/// vertex arrays the encoder is handed; Metal turns it into an MTLVertexDescriptor.
	BufferLayout vertexLayout;

	Types::PrimitiveTopology topology = Types::PrimitiveTopology::TriangleList;
	Types::CullMode cullMode = Types::CullMode::Back;
	Types::FrontFace frontFace = Types::FrontFace::CounterClockwise;
	DepthState depthState;
	BlendState blendState;
	RenderTargetFormats formats;
};

}

#endif //CE_CORE_RENDER_PIPELINE_PIPELINEDESCRIPTOR_HPP
