//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer/Platforms/Mac/Metal
// File: MetalBuffer.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-03
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include "Core/Render/Buffer/Platforms/Mac/Metal/MetalBuffer.hpp"
#include "Tools/Log/Log.hpp"

#include <Metal/Metal.hpp>

#include <format>
#include <stdexcept>


namespace CE::Core {

namespace {

constexpr u32 F32_SIZE = sizeof(f32);

NS::SharedPtr<MTL::Buffer> MakeSharedBuffer(MTL::Device* nativeDevice, const void* data, const size_t sizeInBytes, std::string_view what) {
	if (not nativeDevice) [[unlikely]] {
		const auto error = std::format("MakeSharedBuffer: The {} was given no device to allocate on!", what);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	if (sizeInBytes == 0 or not data) [[unlikely]] {
		const auto error = std::format("MakeSharedBuffer: The {} was given no data. Metal has no zero-sized buffers!", what);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	auto buffer = NS::TransferPtr(nativeDevice->newBuffer(data, sizeInBytes, MTL::ResourceStorageModeShared));
	if (not buffer) [[unlikely]] {
		const auto error = std::format("MakeSharedBuffer: The device refused a {}-byte allocation for the {}!", sizeInBytes, what);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	return buffer;
}

}

#pragma region MetalVertexBuffer

MetalVertexBuffer::MetalVertexBuffer(MTL::Device* nativeDevice, const f32* vertices, const size_t count, const BufferLayout& layout):
	_vertexBufferLayout(layout), _sizeInBytes(count * F32_SIZE) {
	_nativeBuffer = MakeSharedBuffer(nativeDevice, vertices, count, "MetalVertexBuffer");
}

MetalVertexBuffer::MetalVertexBuffer(MTL::Device* nativeDevice, const f32* vertices, const size_t count, BufferLayout&& layout):
	_vertexBufferLayout(std::move(layout)), _sizeInBytes(count * F32_SIZE) {
	_nativeBuffer = MakeSharedBuffer(nativeDevice, vertices, count, "MetalVertexBuffer");
}

u32 MetalVertexBuffer::BindLayout(const u32 firstAttributeIndex) const {
	// Nothing to record: see the note in the header. The count is still truthful, so a caller walking several buffers
	// keeps handing out the right slots.
	return firstAttributeIndex + static_cast<u32>(_vertexBufferLayout.GetElements().size());
}

#pragma endregion

#pragma region MetalIndexBuffer

MetalIndexBuffer::MetalIndexBuffer(MTL::Device* nativeDevice, const u32* indices, const size_t count): _count(count) {
	_nativeBuffer = MakeSharedBuffer(nativeDevice, indices, count, "MetalIndexBuffer");
}

#pragma endregion

}
