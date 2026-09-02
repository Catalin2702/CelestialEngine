//
// Module: CelestialEngine/Engine/Modules/Core/Render/Renderer
// File: DrawCommand.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_RENDER_RENDERER_DRAWCOMMAND_HPP
#define CE_CORE_RENDER_RENDERER_DRAWCOMMAND_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"


namespace CE::Core {

class I_IndexBuffer;
class I_PipelineState;
class I_VertexBuffer;

/**
 * @struct DrawCommand
 * @brief One indexed draw, and the state it is drawn with
 * @details Everything a renderer needs to turn a submission into encoder calls, and nothing more: no transform, no
 *			material, no sort key. Those belong to the scene layer that will eventually build these, not to the draw
 *			itself.
 *
 *			The pointers are borrowed for the duration of the Submit call, the same contract the encoder's setters
 *			carry: the caller owns the resources and must keep them alive until the pass ends. Shared ownership would
 *			put an atomic increment on the hottest path in the engine to solve a lifetime problem the caller already
 *			has.
 */
struct CE_CORE_API DrawCommand {
	/// The compiled state to draw with. Its render target formats have to match the pass' attachments.
	const I_PipelineState* pipelineState = nullptr;

	/// The vertices. Its layout must agree with the pipeline's vertexLayout.
	const I_VertexBuffer* vertexBuffer = nullptr;

	/// The indices to read. Non-indexed drawing needs a second command type, once a backend needs one.
	const I_IndexBuffer* indexBuffer = nullptr;

	/// How many indices to read. 0 means "all of them, from firstIndex on", which is what a whole-mesh draw wants.
	u32 indexCount = 0;

	/// Index to start reading at, as an offset into the index buffer.
	u32 firstIndex = 0;

	/// Added to every index before it fetches a vertex, so several meshes can share one vertex buffer.
	u32 baseVertex = 0;
};

}

#endif //CE_CORE_RENDER_RENDERER_DRAWCOMMAND_HPP
