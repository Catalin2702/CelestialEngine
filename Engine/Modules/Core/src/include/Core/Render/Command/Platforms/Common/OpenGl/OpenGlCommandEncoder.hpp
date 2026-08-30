//
// Module: CelestialEngine/Engine/Modules/Core/Command/Platforms/Common/OpenGl
// File: OpenGlCommandEncoder.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#pragma once

#ifndef CE_CORE_RENDER_COMMAND_OPENGLCOMMANDENCODER_HPP
#define CE_CORE_RENDER_COMMAND_OPENGLCOMMANDENCODER_HPP

#include "Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp"
#include "Core/Render/Command/I_CommandEncoder.hpp"
#include "Core/Render/Command/Viewport.hpp"
#include "Core/Render/Pipeline/Platforms/Common/OpenGl/OpenGlPipelineState.hpp"


namespace CE::Core {

/**
 * @class OpenGlCommandEncoder
 * @brief Records one render pass' worth of OpenGL calls
 * @details OpenGL has no command buffer, so nothing is actually recorded: every method issues its GL call straight
 *			away and the driver holds the state. The class exists so the renderer can be written once against
 *			I_CommandEncoder and still run on the backends that do have command buffers.
 *
 *			It owns exactly one thing, a vertex array. That is a GL wart, not a design choice: before GL 4.3 the
 *			attribute format and the buffers it reads from live together in a VAO, and macOS caps at 4.1, so binding a
 *			vertex buffer on its own would record nothing. The other backends need no such object - the format lives in
 *			the pipeline there - which is why it stays here rather than in I_CommandEncoder.
 *
 *			It owns nothing else: the resources handed to the setters are borrowed for the duration of the call, and
 *			the caller must keep them alive until the pass ends, exactly as a DX12 command list or a Vulkan command
 *			buffer requires.
 */
class OpenGlCommandEncoder final: public I_CommandEncoderBase<Types::GraphicsApi::OpenGL> {
public:
	/**
	 * @brief Creates and binds the encoder's vertex array
	 */
	OpenGlCommandEncoder();

	OpenGlCommandEncoder(const OpenGlCommandEncoder&) = delete;
	OpenGlCommandEncoder(OpenGlCommandEncoder&& other) noexcept;

	~OpenGlCommandEncoder() override;

public:
	OpenGlCommandEncoder& operator=(const OpenGlCommandEncoder&) = delete;
	OpenGlCommandEncoder& operator=(OpenGlCommandEncoder&& other) noexcept;

public:
	/**
	 * @brief Makes the pipeline's program and fixed-function state current, and adopts its topology
	 */
	void SetPipelineState(const I_PipelineState& pipelineState) override;

	/**
	 * @brief Binds the element buffer into the encoder's vertex array
	 */
	void SetIndexBuffer(const I_IndexBuffer& indexBuffer) override;

	/**
	 * @brief Binds the vertex buffer and records its attribute layout into the encoder's vertex array
	 * @details The attributes always start at slot 0, so a second call replaces the first rather than adding to it:
	 *			one vertex buffer per draw. Feeding a draw from several buffers needs the binding slot that
	 *			IASetVertexBuffers and vkCmdBindVertexBuffers take and I_CommandEncoder does not expose yet.
	 */
	void SetVertexBuffer(const I_VertexBuffer& vertexBuffer) override;

	/**
	 * @brief Sets the viewport rectangle and the depth range
	 */
	void SetViewport(const Viewport& viewport) override;

public:
	/**
	 * @brief Draws indexed geometry with the currently bound pipeline, vertex array and element buffer
	 * @param indexCount How many indices to read
	 * @param firstIndex Index to start reading at, as an offset into the element buffer
	 * @param baseVertex Value added to every index before it is used to fetch a vertex
	 */
	void DrawIndexed(u32 indexCount, u32 firstIndex, u32 baseVertex) override;

	/**
	 * @brief Ends the pass, leaving the global GL state as it was found
	 */
	void End() override;

private:
	/// Taken from the pipeline, not from the draw call: OpenGL wants the primitive mode as an argument of glDraw*,
	/// while the other backends bake it into the pipeline object.
	Types::PrimitiveTopology _topology = Types::PrimitiveTopology::None;

	u32 _vaoID = 0;			///< The vertex array the attribute layout is recorded into

	size_t _indexCount = 0;	///< Indices held by the bound element buffer, to bounds-check the draws in Debug
	bool _ended = false;	///< Guards against recording after End()
};

}

#endif //CE_CORE_RENDER_COMMAND_OPENGLCOMMANDENCODER_HPP
