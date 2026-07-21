//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: WindowProps.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#pragma once

#ifndef CE_TYPES_WINDOW_WINDOWPROPS_HPP
#define CE_TYPES_WINDOW_WINDOWPROPS_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Render/Render.hpp"

#include <format>
#include <functional>
#include <ostream>
#include <string>
#include <string_view>


namespace CE::Events {
	class Event;
}

namespace CE::Types {

/**
 * @enum WindowApi
 * @brief Enumeration of supported windowing APIs
 * @details Defines the windowing APIs that can be used for creating and managing windows.
 *			Includes options for GLFW (cross-platform), Win32 (Windows), X11 (Linux), and Cocoa (macOS).
 */
enum class WindowApi: uint8_t {
	None,											///< No window API specified
	GLFW,											///< GLFW windowing library
	Win32,											///< Win32 API (Windows)
	X11,											///< X11 (Linux)
	Cocoa,											///< Cocoa (macOS)
};

/**
 * @struct WindowProps
 * @brief Structure holding window creation properties
 * @details Contains all necessary information to create and configure a window,
 *			including title, dimensions, and VSync setting.
 */
struct CE_API WindowProps {
	std::string title;								///< Window title displayed in the title bar
	unsigned int width = 0;							///< Window width in pixels
	unsigned int height = 0;						///< Window height in pixels
	bool VSync = false;								///< Vertical synchronization enabled/disabled
	GraphicsApi graphicsApi = GraphicsApi::None;	///< Graphics API to be used for rendering (OpenGL, Metal, Vulkan, DirectX)
	WindowApi windowApi = WindowApi::None;			///< Windowing API to be used for window management (GLFW, Win32, X11, Cocoa)

	/**
	 * @brief Default constructor
	 * @details Initializes the WindowProps structure with default values:
	 *			title = "", width = 0, height = 0, VSync = false, graphicsApi = None.
	 */
	WindowProps() = default;
	/**
	 * @brief Constructor
	 * @param title Window title string
	 * @param width Window width in pixels
	 * @param height Window height in pixels
	 * @param VSync Enable or disable vertical synchronization
	 * @param graphicsApi Graphics API to use for rendering
	 * @param windowApi Windowing API to use for window management
	 * @details Initializes the WindowProps structure with the provided values.
	 *			This structure is used to pass configuration parameters when creating a window.
	 */
	WindowProps(const std::string& title, unsigned int width, unsigned int height, bool VSync, GraphicsApi graphicsApi, WindowApi windowApi);
};

}

template <>
struct std::formatter<CE::Types::WindowApi> : std::formatter<std::string_view> {
	auto format(const CE::Types::WindowApi value, std::format_context& ctx) const {
		using CE::Types::WindowApi;
		switch (value) {
			case WindowApi::None:  return std::formatter<std::string_view>::format("None", ctx);
			case WindowApi::GLFW:  return std::formatter<std::string_view>::format("GLFW", ctx);
			case WindowApi::Win32: return std::formatter<std::string_view>::format("Win32", ctx);
			case WindowApi::X11:   return std::formatter<std::string_view>::format("X11", ctx);
			case WindowApi::Cocoa: return std::formatter<std::string_view>::format("Cocoa", ctx);
			default:               return std::formatter<std::string_view>::format("Unknown Window API", ctx);
		}
	}
};

namespace CE::Types {

inline std::ostream& operator<<(std::ostream& os, const WindowApi& event) {
	return os << std::format("{}", event);
}

/**
 * @brief Checks if the specified window API is supported on the current platform
 * @param api Window API to check
 * @return bool True if the window API is supported, false otherwise
 * @details This function checks if the given window API is supported on the current platform.
 *			For example, Cocoa is only supported on macOS, while Win32 is only supported on Windows.
 */
CE_API bool IsWindowApiSupported(const WindowApi& api);

/**
 * @brief Checks if the specified graphics API is compatible with the specified window API
 * @param graphicsApi Graphics API to check
 * @param windowApi Window API to check
 * @return bool True if the graphics API is compatible with the window API, false otherwise
 * @details This function checks if the given graphics API can be used with the given window API.
 *			For example, OpenGL and Vulkan are generally compatible with GLFW, while Metal is only compatible with Cocoa on macOS.
 */
CE_API bool IsGraphicsApiCompatibleWithWindowApi(const GraphicsApi& graphicsApi, const WindowApi& windowApi);

}

#endif //CE_TYPES_WINDOW_WINDOWPROPS_HPP
