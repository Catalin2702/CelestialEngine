//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: Render.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_TYPES_RENDER_RENDER_HPP
#define CE_TYPES_RENDER_RENDER_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Type.hpp"
#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>


namespace CE::Types {

/**
 * @enum GraphicsApi
 * @brief Enumeration of supported graphics APIs
 * @details Defines the graphics APIs that can be used for rendering in the window.
 *			Includes options for OpenGL, Metal, Vulkan, and DirectX (Windows).
 */
enum class GraphicsApi: u8 {
	None = 0,											///< No graphics API specified
	OpenGL = BIT(0),									///< OpenGL graphics API
	Metal = BIT(1),										///< Metal graphics API (macOS)
	DirectX11 = BIT(2),									///< DirectX 11 graphics API (Windows)
	DirectX12 = BIT(3),									///< DirectX 12 graphics API (Windows)
	Vulkan = BIT(4),									///< Vulkan graphics API
};

constexpr GraphicsApi operator & (GraphicsApi x, GraphicsApi y) {
	return static_cast<GraphicsApi>(static_cast<u8>(x) & static_cast<u8>(y));
}

constexpr GraphicsApi operator | (GraphicsApi x, GraphicsApi y) {
	return static_cast<GraphicsApi>(static_cast<u8>(x) | static_cast<u8>(y));
}

constexpr GraphicsApi operator ^ (GraphicsApi x, GraphicsApi y) {
	return static_cast<GraphicsApi>(static_cast<u8>(x) ^ static_cast<u8>(y));
}

constexpr bool HasAnyFlags(const GraphicsApi x, const GraphicsApi y) {
	return (x & y) != GraphicsApi::None;
}

enum class CE_TYPES_API ShaderDataType: u8 {
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

/**
 * @brief Checks if the specified graphics API is supported on the current platform
 * @param api Graphics API to check
 * @return bool True if the graphics API is supported, false otherwise
 * @details This function checks if the given graphics API is supported on the current platform.
 *			For example, Metal is only supported on macOS, while DirectX is only supported on Windows.
 */
CE_TYPES_API bool IsGraphicsApiSupported(const GraphicsApi& api);

/**
 * @brief Colour render targets a single render pass can write to at once
 * @details A hard ceiling on Direct3D 11/12 (D3D1x_SIMULTANEOUS_RENDER_TARGET_COUNT) and on Metal, and the minimum
 *			the OpenGL spec requires of GL_MAX_COLOR_ATTACHMENTS. Vulkan is the exception: its guaranteed
 *			maxColorAttachments is only 4, so that limit has to be queried and the attachment count clamped to it.
 */
inline constexpr u32 MAX_COLOR_ATTACHMENTS = 8;

}

template <>
struct std::formatter<CE::Types::GraphicsApi> : std::formatter<std::string_view> {
	auto format(const CE::Types::GraphicsApi value, std::format_context& ctx) const {
		using CE::Types::GraphicsApi;
		switch (value) {
			case GraphicsApi::None: return std::formatter<std::string_view>::format("None", ctx);
			case GraphicsApi::OpenGL: return std::formatter<std::string_view>::format("OpenGL", ctx);
			case GraphicsApi::Metal: return std::formatter<std::string_view>::format("Metal", ctx);
			case GraphicsApi::Vulkan: return std::formatter<std::string_view>::format("Vulkan", ctx);
			case GraphicsApi::DirectX11: return std::formatter<std::string_view>::format("DirectX11", ctx);
			case GraphicsApi::DirectX12: return std::formatter<std::string_view>::format("DirectX12", ctx);
			default: return std::formatter<std::string_view>::format("Unknown Graphics API", ctx);
		}
	}
};

#endif //CE_TYPES_RENDER_RENDER_HPP
