//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer/Platforms/Common/OpenGl
// File: Buffer.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include "Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp"
#include "Utility/Range/Enumerate.hpp"

#include <algorithm>
#include <ranges>
#include <glad/glad.h>


namespace CE::Core {

constexpr uint32_t FLOAT_SIZE = sizeof(float);
constexpr uint32_t UINT32_SIZE = sizeof(uint32_t);

static constexpr GLenum ShaderDataTypeToOpenGlBaseType(const ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float:
		case ShaderDataType::Float2:
		case ShaderDataType::Float3:
		case ShaderDataType::Float4:
		case ShaderDataType::Mat3:
		case ShaderDataType::Mat4:
			return GL_FLOAT;
		case ShaderDataType::Int:
		case ShaderDataType::Int2:
		case ShaderDataType::Int3:
		case ShaderDataType::Int4:
			return GL_INT;
		case ShaderDataType::Bool:
			return GL_BOOL;
		case ShaderDataType::None:
		default:
			return GL_NONE;
	}
}

const void* OpenGlBufferOffset(const uint32_t offset) {
	return reinterpret_cast<const void*>(static_cast<uintptr_t>(offset));
}

#pragma region OpenGlVertexBuffer
OpenGlVertexBuffer::OpenGlVertexBuffer(const float* vertices, const size_t count) {
	glGenBuffers(1, &this->_renderID);
	glBindBuffer(GL_ARRAY_BUFFER, this->_renderID);
	glBufferData(GL_ARRAY_BUFFER, static_cast<long>(FLOAT_SIZE * count), vertices, GL_STATIC_DRAW);
}

OpenGlVertexBuffer::OpenGlVertexBuffer(OpenGlVertexBuffer&& other) noexcept: _renderID(other._renderID) {
	// The moved-from object must forget the name, otherwise its destructor deletes the buffer we just took.
	other._renderID = 0;
}

OpenGlVertexBuffer& OpenGlVertexBuffer::operator=(OpenGlVertexBuffer&& other) noexcept {
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

void OpenGlVertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, this->_renderID);
}

void OpenGlVertexBuffer::Unbind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGlVertexBuffer::SetLayout(const BufferLayout& layout) {
	_layout = layout;
	const auto stride = static_cast<GLsizei>(_layout.GetStride());

	for (auto const [index, element]: Utility::Enumerate(_layout)) {
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(
			index,
			static_cast<GLint>(element.componentCount),
			ShaderDataTypeToOpenGlBaseType(element.type),
			element.normalized,
			stride,
			OpenGlBufferOffset(element.offset)
		);
	}
}

void OpenGlVertexBuffer::SetLayout(BufferLayout&& layout) {
	_layout = std::move(layout);
}

#pragma endregion

#pragma region OpenGlIndexBuffer
OpenGlIndexBuffer::OpenGlIndexBuffer(const uint32_t* indices, const size_t count): _count(count) {
	glGenBuffers(1, &this->_renderID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_renderID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long>(UINT32_SIZE * this->_count), indices, GL_STATIC_DRAW);
}

OpenGlIndexBuffer::OpenGlIndexBuffer(OpenGlIndexBuffer&& other) noexcept: _renderID(other._renderID), _count(other._count) {
	// The moved-from object must forget the name, otherwise its destructor deletes the buffer we just took.
	other._renderID = 0;
	other._count = 0;
}

OpenGlIndexBuffer& OpenGlIndexBuffer::operator=(OpenGlIndexBuffer&& other) noexcept {
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

void OpenGlIndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->_renderID);
}

void OpenGlIndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

#pragma endregion

}
