//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer/Platforms/Common/OpenGl
// File: OpenGlBuffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-30
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP
#define CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP

#include "Core/Render/Buffer/I_Buffer.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"


namespace CE::Core {

CE_CORE_API const void* OpenGlBufferOffset(u32 offset);

#pragma region OpenGlVertexBuffer
/**
 * @class OpenGlVertexBuffer
 * @brief OpenGL implementation of a vertex buffer object (VBO)
 * @details Owns an OpenGL buffer object created on construction and deleted on destruction; Bind/Unbind
 *			attach/detach it to GL_ARRAY_BUFFER.
 */
class CE_CORE_API OpenGlVertexBuffer final: public I_VertexBufferBase<Types::GraphicsApi::OpenGL> {
public:
	OpenGlVertexBuffer(const f32* vertices, size_t count);

	OpenGlVertexBuffer(const f32* vertices, size_t count, const BufferLayout& layout);
	OpenGlVertexBuffer(const f32* vertices, size_t count, BufferLayout&& layout);

	OpenGlVertexBuffer() = default;
	OpenGlVertexBuffer(const OpenGlVertexBuffer&) = delete;
	OpenGlVertexBuffer(OpenGlVertexBuffer&& other) noexcept;

	~OpenGlVertexBuffer() override;

public:
	OpenGlVertexBuffer& operator=(const OpenGlVertexBuffer&) = delete;
	OpenGlVertexBuffer& operator=(OpenGlVertexBuffer&& other) noexcept;

public:
	void BindBuffer() const;
	void UnbindBuffer() const;
	[[nodiscard]] u32 BindLayout(u32 firstAttributeIndex) const override;

	void SetLayout(const BufferLayout& layout) override;
	void SetLayout(BufferLayout&& layout) override;
	[[nodiscard]] const BufferLayout& GetLayout() const override { return _layout; }

private:
	BufferLayout _layout;
	u32 _renderID = 0;
};
#pragma endregion

#pragma region OpenGlIndexBuffer
class CE_CORE_API OpenGlIndexBuffer final: public I_IndexBufferBase<Types::GraphicsApi::OpenGL> {
public:
	OpenGlIndexBuffer(const u32* indices, size_t count);

	OpenGlIndexBuffer() = default;
	OpenGlIndexBuffer(const OpenGlIndexBuffer&) = delete;
	OpenGlIndexBuffer(OpenGlIndexBuffer&& other) noexcept;

	~OpenGlIndexBuffer() override;

public:
	OpenGlIndexBuffer& operator=(const OpenGlIndexBuffer&) = delete;
	OpenGlIndexBuffer& operator=(OpenGlIndexBuffer&& other) noexcept;

public:
	void BindBuffer() const;
	void UnbindBuffer() const;
	[[nodiscard]] size_t GetCount() const override { return _count; }

private:
	u32 _renderID = 0;
	size_t _count = 0;
};
#pragma endregion

}

#endif //CE_CORE_RENDER_BUFFER_OPENGLBUFFER_HPP
