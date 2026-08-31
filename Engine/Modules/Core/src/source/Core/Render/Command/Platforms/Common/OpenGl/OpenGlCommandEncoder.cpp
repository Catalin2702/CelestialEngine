//
// Module: CelestialEngine/Engine/Modules/Core/Command/Platforms/Common/OpenGl
// File: OpenGlCommandEncoder.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#include "Core/Render/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.hpp"
#include "Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp"
#include "Core/Render/Command/Viewport.hpp"
#include "Tools/Log/Log.hpp"

#include <glad/glad.h>

#include <cassert>
#include <utility>


namespace CE::Core {

OpenGlCommandEncoder::OpenGlCommandEncoder(const u32 width, const u32 height) {
	glGenVertexArrays(1, &_vaoID);

	// Bound for the encoder's whole life: every buffer binding below is recorded into it, and glDrawElements reads it.
	glBindVertexArray(_vaoID);

	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glDepthRangef(0.f, 1.f);
}

OpenGlCommandEncoder::OpenGlCommandEncoder(OpenGlCommandEncoder&& other) noexcept:
	_topology(other._topology), _vaoID(other._vaoID), _indexCount(other._indexCount), _ended(other._ended) {
	// The moved-from encoder must forget the name, otherwise its destructor deletes the vertex array we just took.
	other._vaoID = 0;
	other._ended = true;
}

OpenGlCommandEncoder& OpenGlCommandEncoder::operator = (OpenGlCommandEncoder&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	if (_vaoID != 0)
		glDeleteVertexArrays(1, &_vaoID);

	_topology = other._topology;
	_vaoID = other._vaoID;
	_indexCount = other._indexCount;
	_ended = other._ended;

	other._vaoID = 0;
	other._ended = true;

	return *this;
}

OpenGlCommandEncoder::~OpenGlCommandEncoder() {
	if (_vaoID == 0) [[unlikely]]
		return;

	// A pass that was never ended still has to leave the global state clean, or the next one inherits this VAO.
	if (not _ended)
		OpenGlCommandEncoder::End();

	glDeleteVertexArrays(1, &_vaoID);
}

void OpenGlCommandEncoder::SetPipelineState(const I_PipelineState& pipelineState) {
	assert(not _ended && "OpenGlCommandEncoder::SetPipelineState: The pass has already been ended.");
	assert(pipelineState.GetGraphicApi() == Types::GraphicsApi::OpenGL && "OpenGlCommandEncoder::SetPipelineState: The pipeline is not OpenGL based.");

	const auto& openGlPipeline = static_cast<const OpenGlPipelineState&>(pipelineState);

	// No command buffer to record into: Apply() issues the GL calls now and the driver keeps the state.
	openGlPipeline.Apply();

	// Kept as the engine's own enum rather than the GL constant: the conversion is one switch away and this keeps the
	// member meaningful in a debugger.
	_topology = openGlPipeline.GetDescriptor().topology;
}

void OpenGlCommandEncoder::SetIndexBuffer(const I_IndexBuffer& indexBuffer) {
	assert(not _ended && "OpenGlCommandEncoder::SetIndexBuffer: The pass has already been ended.");
	assert(indexBuffer.GetGraphicApi() == Types::GraphicsApi::OpenGL && "OpenGlCommandEncoder::SetIndexBuffer: The index buffer is not OpenGL based.");

	const auto& openGlIndexBuffer = static_cast<const OpenGlIndexBuffer&>(indexBuffer);

	// GL_ELEMENT_ARRAY_BUFFER is vertex array state, so this binding is recorded into _vaoID rather than being global.
	openGlIndexBuffer.BindBuffer();

	_indexCount = openGlIndexBuffer.GetCount();
}

void OpenGlCommandEncoder::SetVertexBuffer(const I_VertexBuffer& vertexBuffer) {
	assert(not _ended && "OpenGlCommandEncoder::SetVertexBuffer: The pass has already been ended.");
	assert(vertexBuffer.GetGraphicApi() == Types::GraphicsApi::OpenGL && "OpenGlCommandEncoder::SetVertexBuffer: The vertex buffer is not OpenGL based.");

	const auto& openGlVertexBuffer = static_cast<const OpenGlVertexBuffer&>(vertexBuffer);

	// glVertexAttribPointer reads whatever is bound to GL_ARRAY_BUFFER, so the buffer has to come first.
	openGlVertexBuffer.BindBuffer();
	std::ignore = openGlVertexBuffer.BindLayout(0);
}

void OpenGlCommandEncoder::SetViewport(const Viewport& viewport) {
	assert(not _ended && "OpenGlCommandEncoder::SetViewport: The pass has already been ended.");
	assert(viewport.GetGraphicsApi() == Types::GraphicsApi::OpenGL && "OpenGlCommandEncoder::SetViewport: The viewport was not built for OpenGl, so its origin is not flipped.");

	glViewport(
		static_cast<GLint>(viewport.x),
		static_cast<GLint>(viewport.y),
		static_cast<GLsizei>(viewport.width),
		static_cast<GLsizei>(viewport.height)
	);

	glDepthRangef(viewport.minDepth, viewport.maxDepth);
}

void OpenGlCommandEncoder::DrawIndexed(const u32 indexCount, const u32 firstIndex, const u32 baseVertex) {
	assert(not _ended && "OpenGlCommandEncoder::DrawIndexed: The pass has already been ended.");
	assert(_topology != Types::PrimitiveTopology::None && "OpenGlCommandEncoder::DrawIndexed: No pipeline was set, so there is no primitive topology to draw with.");
	assert(_indexCount != 0 && "OpenGlCommandEncoder::DrawIndexed: No index buffer was set.");
	assert(static_cast<size_t>(firstIndex) + indexCount <= _indexCount && "OpenGlCommandEncoder::DrawIndexed: The draw reads past the end of the index buffer.");

	if (indexCount == 0) [[unlikely]]
		return;

	glDrawElementsBaseVertex(
		Types::ToOpenGl(_topology),
		static_cast<GLsizei>(indexCount),
		// The index buffers are built from u32, so the type is fixed; it becomes encoder state the day u16 is allowed.
		GL_UNSIGNED_INT,
		OpenGlBufferOffset(firstIndex * static_cast<u32>(sizeof(u32))),
		static_cast<GLint>(baseVertex)
	);
}

void OpenGlCommandEncoder::End() {
	if (_ended) [[unlikely]] {
		CE_CORE_WARN("OpenGlCommandEncoder::End: The pass has already been ended.");
		return;
	}

	// Everything here is global state that outlives the pass, and the ImGui layer draws right after us: leaving our
	// vertex array or program bound would silently become its starting state.
	glBindVertexArray(0);
	glUseProgram(0);

	_ended = true;
}

}
