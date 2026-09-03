//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer/Platforms/Common/OpenGl
// File: Buffer.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#include "Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp"
#include "Utility/Range/Enumerate.hpp"

#include <algorithm>
#include <cassert>
#include <ranges>
#include <glad/glad.h>


namespace CE::Core {

constexpr u32 FLOAT_SIZE = sizeof(f32);
constexpr u32 UINT32_SIZE = sizeof(u32);

static constexpr GLenum ShaderDataTypeToOpenGlBaseType(const Types::ShaderDataType type) {
	switch (type) {
		case Types::ShaderDataType::Float:
		case Types::ShaderDataType::Float2:
		case Types::ShaderDataType::Float3:
		case Types::ShaderDataType::Float4:
		case Types::ShaderDataType::Mat3:
		case Types::ShaderDataType::Mat4:
			return GL_FLOAT;
		case Types::ShaderDataType::Int:
		case Types::ShaderDataType::Int2:
		case Types::ShaderDataType::Int3:
		case Types::ShaderDataType::Int4:
			return GL_INT;
		case Types::ShaderDataType::Bool:
			return GL_BOOL;
		case Types::ShaderDataType::None:
		default:
			return GL_NONE;
	}
}

const void* OpenGlBufferOffset(const u32 offset) {
	return reinterpret_cast<const void*>(static_cast<uintptr_t>(offset));
}


#pragma region OpenGlVertexBuffer
OpenGlVertexBuffer::OpenGlVertexBuffer(const f32* vertices, const size_t count) {
	glGenBuffers(1, &this->_renderID);
	glBindBuffer(GL_ARRAY_BUFFER, this->_renderID);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long>(FLOAT_SIZE * count), vertices, GL_STATIC_DRAW);
}

OpenGlVertexBuffer::OpenGlVertexBuffer(const f32* vertices, const size_t count, const BufferLayout& layout): OpenGlVertexBuffer(vertices, count) {
	_vertexBufferLayout = layout;
}

OpenGlVertexBuffer::OpenGlVertexBuffer(const f32* vertices, const size_t count, BufferLayout&& layout): OpenGlVertexBuffer(vertices, count) {
	_vertexBufferLayout = std::move(layout);
}

OpenGlVertexBuffer::OpenGlVertexBuffer(OpenGlVertexBuffer&& other) noexcept: _renderID(other._renderID) {
	// The moved-from object must forget the name, otherwise its destructor deletes the buffer we just took.
	other._renderID = 0;
}

OpenGlVertexBuffer& OpenGlVertexBuffer::operator = (OpenGlVertexBuffer&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	if (this->_renderID) [[likely]]
		glDeleteBuffers(1, &this->_renderID);

	this->_renderID = other._renderID;
	other._renderID = 0;

	return *this;
}

OpenGlVertexBuffer::~OpenGlVertexBuffer() {
	if (not this->_renderID) [[unlikely]]
		return;

	glDeleteBuffers(1, &this->_renderID);
}

void OpenGlVertexBuffer::BindBuffer() const {
	glBindBuffer(GL_ARRAY_BUFFER, this->_renderID);
}

void OpenGlVertexBuffer::UnbindBuffer() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

u32 OpenGlVertexBuffer::BindLayout(const u32 firstAttributeIndex) const {
	assert(!_vertexBufferLayout.GetElements().empty() && "OpenGlVertexBuffer::BindLayout: The layout is empty. Set it using  OpenGlVertexBuffer::SetLayout.");

	const auto stride = static_cast<GLsizei>(_vertexBufferLayout.GetStride());

	for (auto const [index, element]: Utility::Enumerate(_vertexBufferLayout)) {
		// Offset by the slots already taken, otherwise a second buffer would overwrite the first one's attributes.
		const auto attributeIndex = firstAttributeIndex + static_cast<u32>(index);

		glEnableVertexAttribArray(attributeIndex);
		glVertexAttribPointer(
			attributeIndex,
			static_cast<GLint>(element.componentCount),
			ShaderDataTypeToOpenGlBaseType(element.type),
			element.normalized,
			stride,
			OpenGlBufferOffset(element.offset)
		);
	}

	return static_cast<u32>(_vertexBufferLayout.GetElements().size());
}

void OpenGlVertexBuffer::SetLayout(const BufferLayout& layout) {
	assert(_vertexBufferLayout.GetElements().empty() && "OpenGlVertexBuffer::SetLayout: The layout is already set!");

	_vertexBufferLayout = layout;
}

void OpenGlVertexBuffer::SetLayout(BufferLayout&& layout) {
	assert(_vertexBufferLayout.GetElements().empty() && "OpenGlVertexBuffer::SetLayout: The layout is already set!");

	_vertexBufferLayout = std::move(layout);
}

#pragma endregion

#pragma region OpenGlIndexBuffer
OpenGlIndexBuffer::OpenGlIndexBuffer(const u32* indices, const size_t count): _count(count) {
	glGenBuffers(1, &this->_renderID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_renderID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(UINT32_SIZE * this->_count), indices, GL_STATIC_DRAW);
}

OpenGlIndexBuffer::OpenGlIndexBuffer(OpenGlIndexBuffer&& other) noexcept: _renderID(other._renderID), _count(other._count) {
	// The moved-from object must forget the name, otherwise its destructor deletes the buffer we just took.
	other._renderID = 0;
	other._count = 0;
}

OpenGlIndexBuffer& OpenGlIndexBuffer::operator = (OpenGlIndexBuffer&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	if (this->_renderID)
		glDeleteBuffers(1, &this->_renderID);

	this->_renderID = other._renderID;
	this->_count = other._count;
	other._renderID = 0;
	other._count = 0;

	return *this;
}

OpenGlIndexBuffer::~OpenGlIndexBuffer() {
	if (not this->_renderID) [[unlikely]]
		return;

	glDeleteBuffers(1, &this->_renderID);
}

void OpenGlIndexBuffer::BindBuffer() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_renderID);
}

void OpenGlIndexBuffer::UnbindBuffer() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

#pragma endregion

}
