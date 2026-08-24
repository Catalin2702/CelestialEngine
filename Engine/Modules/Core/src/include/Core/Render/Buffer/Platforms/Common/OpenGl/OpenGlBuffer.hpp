//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer/Platforms/Common/OpenGl
// File: OpenGlBuffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-24
//

#pragma once

#ifndef BUFFER_OPENGLBUFFER_HPP
#define CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP

#include "Define/DynamicLinker.hpp"

#include "Core/Render/Buffer/I_Buffer.hpp"

#include <cstddef>
#include <cstdint>


namespace CE::Core {

#pragma region OpenGlVertexBuffer
/**
 * @class OpenGlVertexBuffer
 * @brief OpenGL implementation of a vertex buffer object (VBO)
 * @details Owns an OpenGL buffer object created on construction and deleted on destruction; Bind/Unbind
 *			attach/detach it to GL_ARRAY_BUFFER.
 */
class CE_CORE_API OpenGlVertexBuffer final: public I_Buffer {
public:
	OpenGlVertexBuffer(const float* vertices, size_t count);

	OpenGlVertexBuffer() = default;
	OpenGlVertexBuffer(const OpenGlVertexBuffer&) = delete;
	OpenGlVertexBuffer(OpenGlVertexBuffer&& other) noexcept;

	~OpenGlVertexBuffer() override;

public:
	OpenGlVertexBuffer& operator=(const OpenGlVertexBuffer&) = delete;
	OpenGlVertexBuffer& operator=(OpenGlVertexBuffer&& other) noexcept;

public:
	void Bind() const override;
	void Unbind() const override;

private:
	uint32_t _renderID = 0;
};
#pragma endregion

#pragma region OpenGlIndexBuffer
class CE_CORE_API OpenGlIndexBuffer final: public I_IndexBuffer {
public:
	OpenGlIndexBuffer(const uint32_t* indices, size_t count);

	OpenGlIndexBuffer() = default;
	OpenGlIndexBuffer(const OpenGlIndexBuffer&) = delete;
	OpenGlIndexBuffer(OpenGlIndexBuffer&& other) noexcept;

	~OpenGlIndexBuffer() override;

public:
	OpenGlIndexBuffer& operator=(const OpenGlIndexBuffer&) = delete;
	OpenGlIndexBuffer& operator=(OpenGlIndexBuffer&& other) noexcept;

public:
	void Bind() const override;
	void Unbind() const override;
	[[nodiscard]] size_t GetCount() const override { return _count; }

private:
	uint32_t _renderID = 0;
	size_t _count = 0;
};
#pragma endregion

}

#endif //CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP
