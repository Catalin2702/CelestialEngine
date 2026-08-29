//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer
// File: I_VertexArray.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-28
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_I_VERTEXARRAY_HPP
#define CE_CORE_RENDER_BUFFER_I_VERTEXARRAY_HPP

#include "Types/Var/Vars.hpp"

#include <memory>


namespace CE::Core {

class I_IndexBuffer;
class I_VertexBuffer;

class I_VertexArray {
public:
	virtual ~I_VertexArray() = default;

public:
	[[nodiscard]] virtual u32 GetRenderID() const = 0;

public:
	/// A vertex array holds exactly one element buffer, so this replaces the previous one rather than adding to it.
	virtual void SetIndexBuffer(const std::shared_ptr<I_IndexBuffer>& indexBuffer) = 0;
	virtual void SetIndexBuffer(std::shared_ptr<I_IndexBuffer>&& indexBuffer) = 0;

	/// Vertex buffers accumulate: each one contributes its own attributes to the same vertices, starting after the slots
	/// the previously added buffers took. They describe one mesh together, they are not one mesh each.
	virtual void AddVertexBuffer(const std::shared_ptr<I_VertexBuffer>& vertexBuffer) = 0;
	virtual void AddVertexBuffer(std::shared_ptr<I_VertexBuffer>&& vertexBuffer) = 0;
	virtual void AddVertexBuffers(const std::initializer_list<std::shared_ptr<I_VertexBuffer>>& vertexBuffers) = 0;
};

}

#endif //CE_CORE_RENDER_BUFFER_I_VERTEXARRAY_HPP
