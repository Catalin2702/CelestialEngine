//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer
// File: I_Buffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_I_BUFFER_HPP
#define CE_CORE_RENDER_BUFFER_I_BUFFER_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <string_view>
#include <vector>


namespace CE::Core {

enum class CE_CORE_API ShaderDataType: u8 {
	None = 0,
	Float,
	Float2,
	Float3,
	Float4,
	Mat3,
	Mat4,
	Int,
	Int2,
	Int3,
	Int4,
	Bool
};

constexpr u32 ShaderDataTypeSize(const ShaderDataType type) {
	constexpr u32 FLOAT_SIZE = sizeof(f32);
	constexpr u32 INT_SIZE = sizeof(u32);
	constexpr u32 BOOL_SIZE = sizeof(bool);
	switch (type) {
		case ShaderDataType::Float: return FLOAT_SIZE;
		case ShaderDataType::Float2: return FLOAT_SIZE * 2;
		case ShaderDataType::Float3: return FLOAT_SIZE * 3;
		case ShaderDataType::Float4: return FLOAT_SIZE * 4;
		case ShaderDataType::Mat3: return FLOAT_SIZE * 3 * 3;
		case ShaderDataType::Mat4: return FLOAT_SIZE * 4 * 4;
		case ShaderDataType::Int: return INT_SIZE;
		case ShaderDataType::Int2: return INT_SIZE * 2;
		case ShaderDataType::Int3: return INT_SIZE * 3;
		case ShaderDataType::Int4: return INT_SIZE * 4;
		case ShaderDataType::Bool: return BOOL_SIZE;
		case ShaderDataType::None:
		default:
			return 0;
	}
}

constexpr u32 ShaderDataTypeComponentCount(const ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float: return 1;
		case ShaderDataType::Float2: return 2;
		case ShaderDataType::Float3: return 3;
		case ShaderDataType::Float4: return 4;
		case ShaderDataType::Mat3: return 3 * 3;
		case ShaderDataType::Mat4: return 4 * 4;
		case ShaderDataType::Int: return 1;
		case ShaderDataType::Int2: return 2;
		case ShaderDataType::Int3: return 3;
		case ShaderDataType::Int4: return 4;
		case ShaderDataType::Bool: return 1;
		case ShaderDataType::None:
		default:
			return 0;
	}
}

struct CE_CORE_API BufferElement {
	BufferElement(const ShaderDataType type, const char* name, const bool normalized = false):
		type(type), size(ShaderDataTypeSize(type)), componentCount(ShaderDataTypeComponentCount(type)), normalized(normalized), name(name) {}

	ShaderDataType type = ShaderDataType::None;
	u32 size = 0;
	u32 offset = 0;
	u32 componentCount = 0;
	bool normalized = false;
	std::string_view name = {};
};

class BufferLayout {
public:
	BufferLayout() = default;

	BufferLayout(const std::initializer_list<BufferElement>& elements);

public:
	[[nodiscard]] const std::vector<BufferElement>& GetElements() const { return _elements; }
	[[nodiscard]] u32 GetStride() const { return _stride; }

public:
	std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
	std::vector<BufferElement>::iterator end() { return _elements.end(); }

private:
	void _CalculateOffsetsAndStride();

private:
	std::vector<BufferElement> _elements;
	u32 _stride = 0;
};

class I_VertexBuffer {
public:
	virtual ~I_VertexBuffer() = default;
public:
	virtual void SetLayout(const BufferLayout& layout) = 0;
	virtual void SetLayout(BufferLayout&& layout) = 0;

public:
	[[nodiscard]] virtual const BufferLayout& GetLayout() const = 0;
	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;

public:
	/**
	 * @brief Records this buffer's attribute layout into the currently bound vertex array
	 * @param firstAttributeIndex The first attribute slot to use, so several buffers can share one vertex array
	 * @return The number of attribute slots consumed, i.e. the first free slot after this buffer
	 */
	[[nodiscard]] virtual u32 BindLayout(u32 firstAttributeIndex) = 0;
};

template<Types::GraphicsApi Api>
class I_VertexBufferBase: public I_VertexBuffer {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return _st_Api; }

private:
	static constexpr Types::GraphicsApi _st_Api = Api;
};

class I_IndexBuffer {
public:
	virtual ~I_IndexBuffer() = default;
public:
	[[nodiscard]] virtual size_t GetCount() const = 0;
	[[nodiscard]] virtual Types::GraphicsApi GetGraphicApi() const = 0;
};

template<Types::GraphicsApi Api>
class I_IndexBufferBase: public I_IndexBuffer {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return _st_Api; }

private:
	static constexpr Types::GraphicsApi _st_Api = Api;
};

}

#endif //CE_CORE_RENDER_BUFFER_I_BUFFER_HPP
