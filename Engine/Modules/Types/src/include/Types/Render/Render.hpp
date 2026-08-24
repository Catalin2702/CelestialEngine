//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: Render.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-08-24
//

#pragma once

#ifndef CE_TYPES_RENDER_RENDER_HPP
#define CE_TYPES_RENDER_RENDER_HPP

#include "Define/DynamicLinker.hpp"

// ReSharper disable CppUnusedIncludeDirective
#include <cstdint>
#include <format>
#include <string_view>


namespace CE::Types {

/**
 * @enum GraphicsApi
 * @brief Enumeration of supported graphics APIs
 * @details Defines the graphics APIs that can be used for rendering in the window.
 *			Includes options for OpenGL, Metal, Vulkan, and DirectX (Windows).
 */
enum class GraphicsApi: uint8_t {
	None,											///< No graphics API specified
	OpenGL,											///< OpenGL graphics API
	Metal,											///< Metal graphics API (macOS)
	Vulkan,											///< Vulkan graphics API
	DirectX11,										///< DirectX 11 graphics API (Windows)
	DirectX12,										///< DirectX 12 graphics API (Windows)
};

/**
 * @brief Checks if the specified graphics API is supported on the current platform
 * @param api Graphics API to check
 * @return bool True if the graphics API is supported, false otherwise
 * @details This function checks if the given graphics API is supported on the current platform.
 *			For example, Metal is only supported on macOS, while DirectX is only supported on Windows.
 */
CE_TYPES_API bool IsGraphicsApiSupported(const GraphicsApi& api);

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
