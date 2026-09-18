//
// Module: CelestialEngine/Engine/Modules/Core/Command/Platforms/Common/OpenGl
// File: OpenGlCommandEncoder.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-30
// Updated by: Catalin Chirosca
// Updated: 2026-09-18
//

#include "Core/Render/Command/Platforms/Common/OpenGl/OpenGlCommandEncoder.hpp"
#include "Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp"
#include "Core/Render/Command/Viewport.hpp"
#include "Core/Render/Texture/Platforms/Common/OpenGl/OpenGlTexture.hpp"
#include "Tools/Tools.hpp"

#include <glad/glad.h>

#include <cassert>
#include <utility>


namespace CE::Core {

Viewport ToOpenGlViewport(const Viewport& viewport, const u32 targetHeight) {
	// Without the target's height there is nothing to measure against, and the only choices are a wrong answer and
	// the caller's own numbers. It keeps the caller's, and says so - a pass is never opened on a zero-sized area, so
	// this is a programming error rather than a state the engine reaches on its own.
	if (targetHeight == 0) [[unlikely]] {
		CE_CORE_WARN("ToOpenGlViewport: an OpenGL viewport needs the render target's height to flip the origin, but none was given; the y coordinate is left unflipped.");
		return viewport;
	}

	auto flipped = viewport;
	flipped.y = static_cast<f32>(targetHeight) - viewport.y - viewport.height;

	return flipped;
}

OpenGlCommandEncoder::OpenGlCommandEncoder(const u32 width, const u32 height): _targetHeight(height) {
	glGenVertexArrays(1, &_vaoID);

	// Bound for the encoder's whole life: every buffer binding below is recorded into it, and glDrawElements reads it.
	glBindVertexArray(_vaoID);

	glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	glDepthRangef(0.f, 1.f);
}

OpenGlCommandEncoder::OpenGlCommandEncoder(OpenGlCommandEncoder&& other) noexcept:
	_topology(other._topology), _ended(other._ended), _vaoID(other._vaoID), _targetHeight(other._targetHeight),
	_indexCount(other._indexCount) {
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
	_targetHeight = other._targetHeight;
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
	assert(not _ended and "OpenGlCommandEncoder::SetPipelineState: The pass has already been ended!");
	assert(pipelineState.GetGraphicApi() == Types::GraphicsApi::OpenGL and "OpenGlCommandEncoder::SetPipelineState: The pipeline is not OpenGL based!");

	const auto& openGlPipeline = static_cast<const OpenGlPipelineState&>(pipelineState);

	// No command buffer to record into: Apply() issues the GL calls now and the driver keeps the state.
	openGlPipeline.Apply();

	// Kept as the engine's own enum rather than the GL constant: the conversion is one switch away and this keeps the
	// member meaningful in a debugger.
	_topology = openGlPipeline.GetDescriptor().topology;
}

void OpenGlCommandEncoder::SetIndexBuffer(const I_IndexBuffer& indexBuffer) {
	assert(not _ended and "OpenGlCommandEncoder::SetIndexBuffer: The pass has already been ended!");
	assert(indexBuffer.GetGraphicApi() == Types::GraphicsApi::OpenGL and "OpenGlCommandEncoder::SetIndexBuffer: The index buffer is not OpenGL based!");

	const auto& openGlIndexBuffer = static_cast<const OpenGlIndexBuffer&>(indexBuffer);

	// GL_ELEMENT_ARRAY_BUFFER is vertex array state, so this binding is recorded into _vaoID rather than being global.
	openGlIndexBuffer.BindBuffer();

	_indexCount = openGlIndexBuffer.GetCount();
}

void OpenGlCommandEncoder::SetVertexBuffer(const I_VertexBuffer& vertexBuffer) {
	assert(not _ended and "OpenGlCommandEncoder::SetVertexBuffer: The pass has already been ended!");
	assert(vertexBuffer.GetGraphicApi() == Types::GraphicsApi::OpenGL and "OpenGlCommandEncoder::SetVertexBuffer: The vertex buffer is not OpenGL based!");

	const auto& openGlVertexBuffer = static_cast<const OpenGlVertexBuffer&>(vertexBuffer);

	// glVertexAttribPointer reads whatever is bound to GL_ARRAY_BUFFER, so the buffer has to come first.
	openGlVertexBuffer.BindBuffer();
	std::ignore = openGlVertexBuffer.BindLayout(0);
}

void OpenGlCommandEncoder::SetViewport(const Viewport& viewport) {
	assert(not _ended and "OpenGlCommandEncoder::SetViewport: The pass has already been ended!");

	// The caller states the rectangle from the top of the target, which is the one convention every backend is given.
	// Turning it round is this backend's own business, and the height it needs came in with the pass.
	const auto [x, y, width, height, minDepth, maxDepth] = ToOpenGlViewport(viewport, _targetHeight);

	glViewport(
		static_cast<GLint>(x),
		static_cast<GLint>(y),
		static_cast<GLsizei>(width),
		static_cast<GLsizei>(height)
	);

	glDepthRangef(minDepth, maxDepth);
}

void OpenGlCommandEncoder::SetFragmentTexture(const u32 slot, const I_Texture& texture) {
	assert(texture.GetGraphicApi() == Types::GraphicsApi::OpenGL and "OpenGlCommandEncoder::SetFragmentTexture: The texture is not OpenGl based!");

	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, static_cast<const OpenGlTexture&>(texture).GetTexture());

	// GLSL 330 has no binding layout qualifier for samplers, so the uniform has to be told which unit to read. The
	// composite shader declares exactly one, and its location is looked up by name once per bind - cheap enough at
	// one call per frame, and it keeps the encoder from having to know the program's uniform table.
	GLint program = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &program);
	if (program != 0) {
		if (const auto location = glGetUniformLocation(program, "sceneColor"); location >= 0)
			glUniform1i(location, static_cast<GLint>(slot));
	}
}

void OpenGlCommandEncoder::DrawIndexed(const u32 indexCount, const u32 firstIndex, const u32 baseVertex) {
	assert(not _ended and "OpenGlCommandEncoder::DrawIndexed: The pass has already been ended!");
	assert(_topology != Types::PrimitiveTopology::None and "OpenGlCommandEncoder::DrawIndexed: No pipeline was set, so there is no primitive topology to draw with!");
	assert(_indexCount != 0 and "OpenGlCommandEncoder::DrawIndexed: No index buffer was set!");
	assert(static_cast<size_t>(firstIndex) + indexCount <= _indexCount and "OpenGlCommandEncoder::DrawIndexed: The draw reads past the end of the index buffer!");

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
