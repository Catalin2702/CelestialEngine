//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: GraphicsApi.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-09-06
//

#pragma once

#ifndef CE_TYPES_RENDER_GRAPHICSAPI_HPP
#define CE_TYPES_RENDER_GRAPHICSAPI_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Types.hpp"
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

/**
 * @brief Checks if the specified graphics API is supported on the current platform
 * @param api Graphics API to check
 * @return bool True if the graphics API is supported, false otherwise
 * @details This function checks if the given graphics API is supported on the current platform.
 *			For example, Metal is only supported on macOS, while DirectX is only supported on Windows.
 */
CE_TYPES_API bool IsGraphicsApiSupported(const GraphicsApi& api);

/**
 * @brief Names a GraphicsApi, for fmt/spdlog and - through the formatter below - for std::format
 * @param graphicsApi The value to name
 * @return std::string_view The enumerator's name, or "Unknown Graphics API" for a value outside the enum
 * @details Single values only. GraphicsApi is bit-based so several can be OR-ed together, but nothing
 *			combines them in practice - a device speaks one API - and a combination names itself "Unknown".
 */
constexpr std::string_view format_as(const GraphicsApi graphicsApi) {
	switch (graphicsApi) {
		case GraphicsApi::None: return "None";
		case GraphicsApi::OpenGL: return "OpenGL";
		case GraphicsApi::Metal: return "Metal";
		case GraphicsApi::DirectX11: return "DirectX11";
		case GraphicsApi::DirectX12: return "DirectX12";
		case GraphicsApi::Vulkan: return "Vulkan";
		default: return "Unknown Graphics API";
	}
}

}

template <>
struct std::formatter<CE::Types::GraphicsApi>: std::formatter<std::string_view> {
	auto format(const CE::Types::GraphicsApi value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_TYPES_RENDER_GRAPHICSAPI_HPP
