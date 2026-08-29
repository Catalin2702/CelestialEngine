//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer
// File: OpenGlVertexArray.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-28
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include "Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlVertexArray.hpp"
#include "Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp"
#include "Types/Var/Vars.hpp"

#include <cassert>
#include <utility>
#include <glad/glad.h>


namespace CE::Core {

namespace {

/// Downcasts a buffer coming from the API-agnostic overloads; a non-OpenGl buffer is a programming error.
template<typename Derived, typename Base>
std::shared_ptr<Derived> AsOpenGlBuffer(std::shared_ptr<Base> buffer) {
	auto openGlBuffer = std::dynamic_pointer_cast<Derived>(std::move(buffer));
	assert(openGlBuffer && "OpenGlVertexArray: the buffer is not an OpenGl buffer.");

	return openGlBuffer;
}

}

void OpenGlVertexArray::_AttachIndexBuffer(const std::shared_ptr<OpenGlIndexBuffer>& indexBuffer) {
	if (not indexBuffer) [[unlikely]]
		return;

	indexBuffer->BindBuffer();
}

void OpenGlVertexArray::_AttachVertexBuffer(const std::shared_ptr<OpenGlVertexBuffer>& vertexBuffer, u32& attributeCount) {
	if (not vertexBuffer) [[unlikely]]
		return;

	// glVertexAttribPointer reads the currently bound GL_ARRAY_BUFFER, so the buffer has to be bound first.
	vertexBuffer->BindBuffer();
	attributeCount += vertexBuffer->BindLayout(attributeCount);
}

OpenGlVertexArray::OpenGlVertexArray() {
	glGenVertexArrays(1, &_renderID);
}

OpenGlVertexArray::OpenGlVertexArray(OpenGlVertexArray&& other) noexcept:
	_indexBuffer(std::move(other._indexBuffer)),
	_vertexBuffers(std::move(other._vertexBuffers)),
	_attributeCount(other._attributeCount),
	_renderID(other._renderID) {
	// The moved-from object must forget the name, otherwise its destructor deletes the vertex array we just took.
	other._attributeCount = 0;
	other._renderID = 0;
}

OpenGlVertexArray& OpenGlVertexArray::operator=(OpenGlVertexArray&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	if (this->_renderID) [[likely]]
		glDeleteVertexArrays(1, &this->_renderID);

	this->_indexBuffer = std::move(other._indexBuffer);
	this->_vertexBuffers = std::move(other._vertexBuffers);
	this->_attributeCount = other._attributeCount;
	this->_renderID = other._renderID;
	other._attributeCount = 0;
	other._renderID = 0;

	return *this;
}

OpenGlVertexArray::~OpenGlVertexArray() {
	if (not this->_renderID) [[unlikely]]
		return;

	glDeleteVertexArrays(1, &this->_renderID);
}

void OpenGlVertexArray::Bind() const {
	glBindVertexArray(_renderID);
}

void OpenGlVertexArray::Unbind() const {
	glBindVertexArray(0);
}

#pragma region SetIndexBufferBase

void OpenGlVertexArray::SetIndexBuffer(const std::shared_ptr<I_IndexBuffer>& indexBuffer) {
	// Named, so the lvalue (void-returning) overload is picked rather than the [[nodiscard]] rvalue one.
	const auto openGlBuffer = AsOpenGlBuffer<OpenGlIndexBuffer>(indexBuffer);
	SetIndexBuffer(openGlBuffer);
}

void OpenGlVertexArray::SetIndexBuffer(std::shared_ptr<I_IndexBuffer>&& indexBuffer) {
	SetIndexBuffer(AsOpenGlBuffer<OpenGlIndexBuffer>(std::move(indexBuffer)));
}

#pragma endregion

#pragma region SetIndexBufferDerived

void OpenGlVertexArray::SetIndexBuffer(const std::shared_ptr<OpenGlIndexBuffer>& indexBuffer) {
	Bind();
	_AttachIndexBuffer(indexBuffer);

	_indexBuffer = indexBuffer;
}

void OpenGlVertexArray::SetIndexBuffer(std::shared_ptr<OpenGlIndexBuffer>&& indexBuffer) {
	Bind();
	_AttachIndexBuffer(indexBuffer);

	_indexBuffer = std::move(indexBuffer);
}

#pragma endregion

#pragma region AddVertexBufferBase

void OpenGlVertexArray::AddVertexBuffer(const std::shared_ptr<I_VertexBuffer>& vertexBuffer) {
	// Named, so the lvalue (void-returning) overload is picked rather than the [[nodiscard]] rvalue one.
	const auto openGlBuffer = AsOpenGlBuffer<OpenGlVertexBuffer>(vertexBuffer);
	AddVertexBuffer(openGlBuffer);
}

void OpenGlVertexArray::AddVertexBuffer(std::shared_ptr<I_VertexBuffer>&& vertexBuffer) {
	AddVertexBuffer(AsOpenGlBuffer<OpenGlVertexBuffer>(std::move(vertexBuffer)));
}

void OpenGlVertexArray::AddVertexBuffers(const std::initializer_list<std::shared_ptr<I_VertexBuffer>>& vertexBuffers) {
	Bind();
	_vertexBuffers.reserve(_vertexBuffers.size() + vertexBuffers.size());

	for (const auto& vertexBuffer: vertexBuffers) {
		auto openGlBuffer = AsOpenGlBuffer<OpenGlVertexBuffer>(vertexBuffer);
		_AttachVertexBuffer(openGlBuffer, _attributeCount);

		_vertexBuffers.push_back(std::move(openGlBuffer));
	}
}

#pragma endregion

#pragma region AddVertexBufferDerived

void OpenGlVertexArray::AddVertexBuffer(const std::shared_ptr<OpenGlVertexBuffer>& vertexBuffer) {
	Bind();
	_AttachVertexBuffer(vertexBuffer, _attributeCount);

	_vertexBuffers.push_back(vertexBuffer);
}

void OpenGlVertexArray::AddVertexBuffer(std::shared_ptr<OpenGlVertexBuffer>&& vertexBuffer) {
	Bind();
	_AttachVertexBuffer(vertexBuffer, _attributeCount);

	_vertexBuffers.emplace_back(std::move(vertexBuffer));
}

void OpenGlVertexArray::AddVertexBuffers(const std::initializer_list<std::shared_ptr<OpenGlVertexBuffer>>& vertexBuffers) {
	Bind();
	for (const auto& vertexBuffer: vertexBuffers)
		_AttachVertexBuffer(vertexBuffer, _attributeCount);

	// An initializer_list only ever hands out const elements, so these are copies no matter what: no point moving.
	_vertexBuffers.reserve(_vertexBuffers.size() + vertexBuffers.size());
	_vertexBuffers.insert(_vertexBuffers.end(), vertexBuffers.begin(), vertexBuffers.end());
}

#pragma endregion

}
