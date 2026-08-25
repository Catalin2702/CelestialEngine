//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer
// File: I_Buffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_I_BUFFER_HPP
#define CE_CORE_RENDER_BUFFER_I_BUFFER_HPP

#include <string_view>
#include <vector>

namespace CE::Core {

enum class ShaderDataType: uint8_t {
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

constexpr uint32_t ShaderDataTypeSize(const ShaderDataType type) {
	constexpr uint32_t FLOAT_SIZE = sizeof(float);
	constexpr uint32_t INT_SIZE = sizeof(int32_t);
	constexpr uint32_t BOOL_SIZE = sizeof(bool);
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

constexpr uint32_t ShaderDataTypeComponentCount(const ShaderDataType type) {
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

struct BufferElement {
	BufferElement(const ShaderDataType type, const char* name, const bool normalized = false):
		type(type), size(ShaderDataTypeSize(type)), componentCount(ShaderDataTypeComponentCount(type)), normalized(normalized), name(name) {}

	ShaderDataType type = ShaderDataType::None;
	uint32_t size = 0;
	uint32_t offset = 0;
	uint32_t componentCount = 0;
	bool normalized = false;
	std::string_view name = {};
};

class BufferLayout {
public:
	BufferLayout() = default;

	BufferLayout(const std::initializer_list<BufferElement>& elements);

public:
	[[nodiscard]] const std::vector<BufferElement>& GetElements() const { return _elements; }
	[[nodiscard]] uint32_t GetStride() const { return _stride; }

public:
	std::vector<BufferElement>::iterator begin() { return _elements.begin(); }
	std::vector<BufferElement>::iterator end() { return _elements.end(); }

private:
	void _CalculateOffsetsAndStride();

private:
	std::vector<BufferElement> _elements;
	uint32_t _stride = 0;
};

/**
 * @class I_Buffer
 * @brief Interface for GPU buffer objects that can be bound to the render pipeline
 * @details Minimal contract shared by API-specific buffers (vertex, index, ...): make the buffer active
 *			for subsequent draw calls (Bind) or deactivate it (Unbind).
 */
class I_Buffer {
public:
	virtual ~I_Buffer() = default;

public:
	/**
	 * @brief Makes the buffer active for subsequent render commands
	 */
	virtual void Bind() const = 0;
	/**
	 * @brief Deactivates the buffer
	 */
	virtual void Unbind() const = 0;
};

class I_VertexBuffer: public I_Buffer {
public:
	virtual void SetLayout(const BufferLayout& layout) = 0;
	virtual void SetLayout(BufferLayout&& layout) = 0;

	[[nodiscard]] virtual const BufferLayout& GetLayout() const = 0;
};

class I_IndexBuffer: public I_Buffer {
public:
	[[nodiscard]] virtual size_t GetCount() const = 0;
};

}

#endif //CE_CORE_RENDER_BUFFER_I_BUFFER_HPP
