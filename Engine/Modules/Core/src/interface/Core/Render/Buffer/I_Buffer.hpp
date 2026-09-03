//
// Module: CelestialEngine/Engine/Modules/Core/Render/Buffer
// File: I_Buffer.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#pragma once

#ifndef CE_CORE_RENDER_BUFFER_I_BUFFER_HPP
#define CE_CORE_RENDER_BUFFER_I_BUFFER_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"

#include <string_view>
#include <vector>


namespace CE::Core {

struct CE_CORE_API BufferElement {
	BufferElement(const Types::ShaderDataType type, const char* name, const bool normalized = false):
		type(type), size(Types::ShaderDataTypeSize(type)), componentCount(Types::ShaderDataTypeComponentCount(type)), normalized(normalized), name(name) {}

	Types::ShaderDataType type = Types::ShaderDataType::None;
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

	// A layout is read-only once built, so the const overloads are what the render path actually uses.
	[[nodiscard]] std::vector<BufferElement>::const_iterator begin() const { return _elements.begin(); }
	[[nodiscard]] std::vector<BufferElement>::const_iterator end() const { return _elements.end(); }

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
	[[nodiscard]] virtual u32 BindLayout(u32 firstAttributeIndex) const = 0;
};

template<Types::GraphicsApi Api>
class I_VertexBufferBase: public I_VertexBuffer {
public:
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return _api; }

private:
	static constexpr Types::GraphicsApi _api = Api;
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
	[[nodiscard]] Types::GraphicsApi GetGraphicApi() const override { return _api; }

private:
	static constexpr Types::GraphicsApi _api = Api;
};

}

#endif //CE_CORE_RENDER_BUFFER_I_BUFFER_HPP
