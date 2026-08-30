//
// Module: CelestialEngine/Engine/Modules/Core/Render/Pipeline/Platforms/Common/OpenGl
// File: OpenGlPipelineState.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#pragma once

#ifndef CE_CORE_RENDER_PIPELINE_OPENGLPIPELINESTATE_HPP
#define CE_CORE_RENDER_PIPELINE_OPENGLPIPELINESTATE_HPP

#include "Core/Render/Pipeline/I_PipelineState.hpp"
#include "Core/Render/Pipeline/PipelineDescriptor.hpp"
#include "Define/DynamicLinker.hpp"


namespace CE::Core {

/**
 * @class OpenGlPipelineState
 * @brief An immutable bundle of OpenGL render state, linked once and replayed per draw
 * @details OpenGL has no pipeline object before GL 4.1's separate shader objects, so the "state" is a linked program
 *			plus the handful of global switches Apply() sets on every use. It deliberately owns no vertex array: a VAO
 *			binds attribute format and buffers together until GL 4.3, and macOS caps at 4.1, so the geometry side stays
 *			in OpenGlVertexArray and the descriptor's vertexLayout is validation material only.
 *
 *			Apply() is not on I_PipelineState because Metal has no equivalent - there the pipeline is handed to the
 *			encoder, not applied to a context - so OpenGlCommandEncoder downcasts and calls it from SetPipelineState.
 */
class CE_CORE_API OpenGlPipelineState final: public I_PipelineStateBase<Types::GraphicsApi::OpenGL> {
public:
	/**
	 * @brief Links the descriptor's shader modules into an OpenGL program
	 * @param descriptor The state to bake; its vertex and fragment modules must be OpenGlShaderModule instances
	 * @details Throws std::runtime_error when a stage is missing, is not an OpenGL module, carries the wrong stage,
	 *			when a destination blend factor is one OpenGL rejects, or when linking fails - the info log is logged
	 *			before rethrowing. The modules are detached once linked, so the program no longer depends on them.
	 */
	explicit OpenGlPipelineState(PipelineDescriptor descriptor);

	OpenGlPipelineState(const OpenGlPipelineState&) = delete;
	OpenGlPipelineState(OpenGlPipelineState&& other) noexcept;

	~OpenGlPipelineState() override;

public:
	OpenGlPipelineState& operator=(const OpenGlPipelineState&) = delete;
	OpenGlPipelineState& operator=(OpenGlPipelineState&& other) noexcept;

public:
	[[nodiscard]] const PipelineDescriptor& GetDescriptor() const override { return _descriptor; }

	/**
	 * @brief Gets the linked OpenGL program name
	 */
	[[nodiscard]] u32 GetProgramId() const { return _programID; }

	/**
	 * @brief Gets the topology as the mode constant the glDraw* calls expect
	 * @details Topology is not pipeline state in OpenGL but a draw-call argument, so it is read here and passed on
	 *			rather than set by Apply().
	 */
	[[nodiscard]] u32 GetOpenGlTopology() const;

	/**
	 * @brief Makes this pipeline's program and fixed-function state current
	 * @details Sets every switch it cares about in both directions rather than assuming a starting state: the ImGui
	 *			layer and the clear path touch the same globals, so an Apply() that only enabled things would inherit
	 *			whatever ran before it. It does not touch the vertex array binding - that belongs to the geometry.
	 */
	void Apply() const;

private:
	PipelineDescriptor _descriptor;
	u32 _programID = 0;
};

}

#endif //CE_CORE_RENDER_PIPELINE_OPENGLPIPELINESTATE_HPP
