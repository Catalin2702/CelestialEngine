//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer
// File: OpenGlVertexArray.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-28
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_OPENGLVERTEXARRAY_HPP
#define CE_CORE_RENDER_BUFFER_OPENGLVERTEXARRAY_HPP

#include "Core/Render/Buffer/I_VertexArray.hpp"
#include "Define/DynamicLinker.hpp"

#include <vector>


namespace CE::Core {

class OpenGlIndexBuffer;
class OpenGlVertexBuffer;

class CE_CORE_API OpenGlVertexArray final: public I_VertexArray {
public:
	OpenGlVertexArray();

	OpenGlVertexArray(const OpenGlVertexArray&) = delete;
	OpenGlVertexArray(OpenGlVertexArray&& other) noexcept;

	~OpenGlVertexArray() override;

public:
	OpenGlVertexArray& operator=(const OpenGlVertexArray&) = delete;
	OpenGlVertexArray& operator=(OpenGlVertexArray&& other) noexcept;

public:
	void Bind() const;
	void Unbind() const;

	[[nodiscard]] uint32_t GetRenderID() const override { return _renderID; }

public:
	void SetIndexBuffer(const std::shared_ptr<I_IndexBuffer>& indexBuffer) override;
	void SetIndexBuffer(std::shared_ptr<I_IndexBuffer>&& indexBuffer) override;

	void SetIndexBuffer(const std::shared_ptr<OpenGlIndexBuffer>& indexBuffer);
	void SetIndexBuffer(std::shared_ptr<OpenGlIndexBuffer>&& indexBuffer);

	void AddVertexBuffer(const std::shared_ptr<I_VertexBuffer>& vertexBuffer) override;
	void AddVertexBuffer(std::shared_ptr<I_VertexBuffer>&& vertexBuffer) override;
	void AddVertexBuffers(const std::initializer_list<std::shared_ptr<I_VertexBuffer>>& vertexBuffers) override;

	void AddVertexBuffer(const std::shared_ptr<OpenGlVertexBuffer>& vertexBuffer);
	void AddVertexBuffer(std::shared_ptr<OpenGlVertexBuffer>&& vertexBuffer);
	void AddVertexBuffers(const std::initializer_list<std::shared_ptr<OpenGlVertexBuffer>>& vertexBuffers);

public:
	[[nodiscard]] const std::shared_ptr<OpenGlIndexBuffer>& GetIndexBuffer() const { return _indexBuffer; }
	[[nodiscard]] const std::vector<std::shared_ptr<OpenGlVertexBuffer>>& GetVertexBuffers() const { return _vertexBuffers; }

private:
	/// Attaches the element buffer to this vertex array; it must already be bound.
	static void _AttachIndexBuffer(const std::shared_ptr<OpenGlIndexBuffer>& indexBuffer);
	/// Records the buffer's attribute layout into this vertex array, after the slots already taken; it must already be bound.
	static void _AttachVertexBuffer(const std::shared_ptr<OpenGlVertexBuffer>& vertexBuffer, uint32_t& attributeCount);

private:
	/// A vertex array has a single element buffer binding: setting a new one replaces whatever was bound before.
	std::shared_ptr<OpenGlIndexBuffer> _indexBuffer;
	std::vector<std::shared_ptr<OpenGlVertexBuffer>> _vertexBuffers;

	/// First free attribute slot; each vertex buffer's layout starts where the previous one ended.
	uint32_t _attributeCount = 0;
	uint32_t _renderID = 0;
};

}

#endif //CE_CORE_RENDER_BUFFER_OPENGLVERTEXARRAY_HPP
