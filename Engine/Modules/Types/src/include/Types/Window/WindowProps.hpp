//
// Module: Types/Window
// File: WindowProps.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-21
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_TYPES_WINDOW_WINDOWPROPS_HPP
#define CE_TYPES_WINDOW_WINDOWPROPS_HPP

#include <functional>
#include <string>
#include <sstream>

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

enum class GraphicsApi: uint8_t {
	None,											///< No graphics API specified
	OpenGL,											///< OpenGL graphics API
	Metal,											///< Metal graphics API (macOS)
	Vulkan,											///< Vulkan graphics API
	DirectX11,										///< DirectX 11 graphics API (Windows)
	DirectX12,										///< DirectX 12 graphics API (Windows)
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
	 * @details Initializes the WindowProps structure with the provided values.
	 *			This structure is used to pass configuration parameters when creating a window.
	 */
	WindowProps(const std::string& title, unsigned int width, unsigned int height, bool VSync, GraphicsApi graphicsApi);
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
	CallbackFn<T> eventCallback;					///< Callback function for event handling

	WindowData() = default;
	/**
	 * @brief Constructor
	 * @param title Window title string
	 * @param width Window width in pixels
	 * @param height Window height in pixels
	 * @param VSync Enable or disable vertical synchronization
	 * @param graphicsApi Graphics API to use for rendering
	 */
	WindowData(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const GraphicsApi graphicsApi)
		: WindowProps(title, width, height, VSync, graphicsApi) {}
	/**
	 * @brief Constructor from WindowProps
	 * @param props WindowProps structure to initialize from
	 */
	WindowData(const WindowProps& props): WindowProps(props) {}
};

inline std::ostream& operator<<(std::ostream& os, const GraphicsApi& event) {
	switch (event) {
		case GraphicsApi::None:
			return os << "None";
		case GraphicsApi::OpenGL:
			return os << "OpenGL";
		case GraphicsApi::Metal:
			return os << "Metal";
		case GraphicsApi::Vulkan:
			return os << "Vulkan";
		case GraphicsApi::DirectX11:
			return os << "DirectX11";
		case GraphicsApi::DirectX12:
			return os << "DirectX12";
		default:
			return os << "Unknown Graphics API";
	}
}

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

}

#endif //CE_TYPES_WINDOW_WINDOWPROPS_HPP
