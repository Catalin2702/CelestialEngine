//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: Render.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-05-07
//

#pragma once

#ifndef CE_TYPES_RENDER_RENDER_HPP
#define CE_TYPES_RENDER_RENDER_HPP

// ReSharper disable CppUnusedIncludeDirective
#include <cstdint>
#include <ostream>
#include <string>

/**
 * @namespace CE::Types::Render
 * @brief Contains type definitions related to rendering, including shader types and graphics APIs
 * @details This namespace encapsulates the various type definitions used in the rendering module of the engine.
 *			It includes enumerations for shader types (e.g., vertex, fragment) and graphics APIs (e.g., OpenGL, Metal).
 *			These types are used throughout the rendering system to ensure consistency and clarity when working with shaders and graphics contexts.
 */
namespace CE::Types::Render {

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
bool IsGraphicsApiSupported(const GraphicsApi& api);

inline std::string format_as(const GraphicsApi& event) {
	switch (event) {
		case GraphicsApi::None:
			return "None";
		case GraphicsApi::OpenGL:
			return "OpenGL";
		case GraphicsApi::Metal:
			return "Metal";
		case GraphicsApi::Vulkan:
			return "Vulkan";
		case GraphicsApi::DirectX11:
			return "DirectX11";
		case GraphicsApi::DirectX12:
			return "DirectX12";
		default:
			return "Unknown Graphics API";
	}
}

inline std::ostream& operator<<(std::ostream& os, const GraphicsApi& event) {
	return os << format_as(event);
}

}

#endif //CE_TYPES_RENDER_RENDER_HPP
