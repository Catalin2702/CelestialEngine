//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer/Platforms/Mac/Metal
// File: MetalBuffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_METALBUFFER_HPP
#define CE_CORE_RENDER_BUFFER_METALBUFFER_HPP

#include "Core/Render/Buffer/I_Buffer.hpp"
#include "Define/DynamicLinker.hpp"

#include <Foundation/Foundation.hpp>


namespace MTL {
	class Buffer;
	class Device;
}

namespace CE::Core {

#pragma region MetalVertexBuffer

/**
 * @class MetalVertexBuffer
 * @brief A block of vertex data living in an MTL::Buffer
 * @details Thinner than its OpenGL twin, because Metal splits what OpenGL fuses: the *data* is here, the *format* is
 *			in the pipeline's MTLVertexDescriptor. There is nothing to bind at buffer level either - the encoder binds
 *			it to a vertex-stage slot at draw time - so this class is little more than the allocation plus the layout
 *			the pipeline was built from, kept for validation.
 *
 *			Storage is ResourceStorageModeShared: on Apple silicon CPU and GPU address the same physical memory, so a
 *			Managed or Private buffer would only add a copy. That stops being true for large static geometry on an
 *			Intel Mac with a discrete GPU, which is when a Private buffer plus a blit upload becomes worth writing.
 */
class CE_CORE_API MetalVertexBuffer final: public I_VertexBufferBase<Types::GraphicsApi::Metal> {
public:
	MetalVertexBuffer() = default;

	/**
	 * @brief Allocates a GPU buffer and copies the vertices into it
	 * @param nativeDevice The device the buffer is allocated on
	 * @param vertices The vertex data; copied, so the caller's array can go away right after
	 * @param count How many floats are in that array - not how many vertices
	 * @param layout How those floats are grouped into attributes
	 * @details Throws std::runtime_error when the device refuses the allocation, or when count is 0: a zero-sized
	 *			MTL::Buffer is not allowed and would be a null return with no error attached.
	 */
	MetalVertexBuffer(MTL::Device* nativeDevice, const f32* vertices, size_t count, const BufferLayout& layout);
	MetalVertexBuffer(MTL::Device* nativeDevice, const f32* vertices, size_t count, BufferLayout&& layout);

	MetalVertexBuffer(const MetalVertexBuffer&) = delete;
	MetalVertexBuffer(MetalVertexBuffer&&) noexcept = default;

	/**
	 * @brief Implemented by default in the .cpp file because of the forward declarations
	 * @details Implemented by default in the .cpp file because otherwise the destructor calls made by `NS::SharedPtr`
	 *			for the forward declared classes would cause the linker to fail.
	 */
	~MetalVertexBuffer() override;

public:
	MetalVertexBuffer& operator = (const MetalVertexBuffer&) = delete;
	MetalVertexBuffer& operator = (MetalVertexBuffer&&) noexcept = default;

public:
	/**
	 * @brief Reports how many attribute slots this buffer feeds, and records nothing
	 * @details A no-op with a return value. The interface method exists for OpenGL, where the attribute format has to
	 *			be recorded into a bound vertex array; under Metal the same information was already baked into the
	 *			pipeline's vertex descriptor when the pipeline was created, so re-recording it per draw is neither
	 *			possible nor needed. It stays honest about the slot count so shared call sites keep working.
	 */
	[[nodiscard]] u32 BindLayout(u32 firstAttributeIndex) const override;

public:
	void SetLayout(const BufferLayout& layout) override { _vertexBufferLayout = layout; }
	void SetLayout(BufferLayout&& layout) override { _vertexBufferLayout = std::move(layout); }

public:
	[[nodiscard]] const BufferLayout& GetLayout() const override { return _vertexBufferLayout; }

	/**
	 * @brief Gets the underlying Metal buffer, for MetalCommandEncoder to bind
	 * @details Backend-specific on purpose, hence absent from I_VertexBuffer - the encoder downcasts to reach it,
	 *			exactly as OpenGlCommandEncoder downcasts for BindBuffer().
	 */
	[[nodiscard]] MTL::Buffer* GetBuffer() const { return _nativeBuffer.get(); }

	/**
	 * @brief Gets the size of the allocation, in bytes
	 */
	[[nodiscard]] size_t GetSizeInBytes() const { return _sizeInBytes; }

private:
	BufferLayout _vertexBufferLayout;
	NS::SharedPtr<MTL::Buffer> _nativeBuffer;
	size_t _sizeInBytes = 0;
};

#pragma endregion

#pragma region MetalIndexBuffer

/**
 * @class MetalIndexBuffer
 * @brief A block of 32-bit indices living in an MTL::Buffer
 * @details Metal has no element-buffer binding point: the buffer is an argument of drawIndexedPrimitives, so this
 *			holds the allocation and the count and nothing else. The index type is fixed at UInt32 to match the
 *			interface's std::span<const u32>; halving it to UInt16 is a real memory win on big meshes and is what the
 *			index type would become encoder state for.
 */
class CE_CORE_API MetalIndexBuffer final: public I_IndexBufferBase<Types::GraphicsApi::Metal> {
public:
	MetalIndexBuffer() = default;

	/**
	 * @brief Allocates a GPU buffer and copies the indices into it
	 * @param nativeDevice The device the buffer is allocated on
	 * @param indices The index data; copied
	 * @param count How many indices are in that array
	 * @details Throws std::runtime_error on a failed allocation or a zero count, as MetalVertexBuffer does.
	 */
	MetalIndexBuffer(MTL::Device* nativeDevice, const u32* indices, size_t count);

	MetalIndexBuffer(const MetalIndexBuffer&) = delete;
	MetalIndexBuffer(MetalIndexBuffer&&) noexcept = default;

public:
	MetalIndexBuffer& operator = (const MetalIndexBuffer&) = delete;
	MetalIndexBuffer& operator = (MetalIndexBuffer&&) noexcept = default;

public:
	[[nodiscard]] size_t GetCount() const override { return _count; }

	/**
	 * @brief Gets the underlying Metal buffer, for MetalCommandEncoder to hand to the draw call
	 */
	[[nodiscard]] MTL::Buffer* GetBuffer() const { return _nativeBuffer.get(); }

private:
	NS::SharedPtr<MTL::Buffer> _nativeBuffer;
	size_t _count = 0;
};

#pragma endregion

}

#endif //CE_CORE_RENDER_BUFFER_METALBUFFER_HPP
