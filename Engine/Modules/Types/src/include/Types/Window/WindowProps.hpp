//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: WindowProps.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-16
//

#pragma once

#ifndef CE_TYPES_WINDOW_WINDOWPROPS_HPP
#define CE_TYPES_WINDOW_WINDOWPROPS_HPP

#include <functional>
#include <sstream>
#include <string>

namespace CE::Events {
class Event;
}

/**
 * @namespace CE::Types::Window
 * @brief Types and utilities related to window management
 * @details Contains type definitions and helper structures for managing windows,
 *			including properties for window creation and event handling.
 */
namespace CE::Types::Window {

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
struct WindowProps {
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

/**
 * @typedef CallbackFn
 * @brief Type alias for event callback functions
 * @tparam T Event type
 * @details Function that takes an event reference and returns void
 */
template<class T>
using CallbackFn = std::function<void(T&)>;

/**
 * @struct WindowData
 * @brief Extended window properties including event callback
 * @tparam T Event type
 * @details Extends WindowProps with an event callback function. Used internally
 *			by window implementations to store window state and event handling.
 */
template<class T>
struct WindowData: WindowProps {
	CallbackFn<T> EventCallback;					///< Callback function for event handling

	/**
	 * @brief Default constructor
	 * @details Initializes the WindowData structure with default values by calling
	 *			the base WindowProps default constructor. The EventCallback is
	 *			default-initialized to an empty function.
	 */
	WindowData() = default;

	/**
	 * @brief Constructor
	 * @param title Window title string
	 * @param width Window width in pixels
	 * @param height Window height in pixels
	 * @param VSync Enable or disable vertical synchronization
	 * @param graphicsApi Graphics API to use for rendering
	 * @param windowApi Windowing API to use for window management
	 * @details Initializes the WindowData structure with the provided values by
	 *			calling the base WindowProps constructor.
	 *			This structure is used internally by window implementations to store both configuration and event callback information.
	 */
	WindowData(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const GraphicsApi graphicsApi, const WindowApi windowApi)
		: WindowProps(title, width, height, VSync, graphicsApi, windowApi) {}
	/**
	 * @brief Constructor from WindowProps
	 * @param props WindowProps structure to initialize from
	 * @details Initializes the WindowData structure with the provided values by
	 *			calling the base WindowProps constructor.
	 *			This structure is used internally by window implementations to store both configuration and event callback information.
	 */
	WindowData(const WindowProps& props): WindowProps(props) {}
};

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

inline std::string format_as(const WindowApi& event) {
	switch (event) {
	case WindowApi::None:
		return "None";
	case WindowApi::GLFW:
		return "GLFW";
	case WindowApi::Win32:
		return "Win32";
	case WindowApi::X11:
		return "X11";
	case WindowApi::Cocoa:
		return "Cocoa";
	default:
		return "Unknown Window API";
	}
}

inline std::ostream& operator<<(std::ostream& os, const WindowApi& event) {
	return os << format_as(event);
}

/**
 * @brief Checks if the specified graphics API is supported on the current platform
 * @param api Graphics API to check
 * @return bool True if the graphics API is supported, false otherwise
 * @details This function checks if the given graphics API is supported on the current platform.
 *			For example, Metal is only supported on macOS, while DirectX is only supported on Windows.
 */
bool IsGraphicsApiSupported(const GraphicsApi& api);

/**
 * @brief Checks if the specified window API is supported on the current platform
 * @param api Window API to check
 * @return bool True if the window API is supported, false otherwise
 * @details This function checks if the given window API is supported on the current platform.
 *			For example, Cocoa is only supported on macOS, while Win32 is only supported on Windows.
 */
bool IsWindowApiSupported(const WindowApi& api);

/**
 * @brief Checks if the specified graphics API is compatible with the specified window API
 * @param graphicsApi Graphics API to check
 * @param windowApi Window API to check
 * @return bool True if the graphics API is compatible with the window API, false otherwise
 * @details This function checks if the given graphics API can be used with the given window API.
 *			For example, OpenGL and Vulkan are generally compatible with GLFW, while Metal is only compatible with Cocoa on macOS.
 */
bool IsGraphicsApiCompatibleWithWindowApi(const GraphicsApi& graphicsApi, const WindowApi& windowApi);

}

#endif //CE_TYPES_WINDOW_WINDOWPROPS_HPP
