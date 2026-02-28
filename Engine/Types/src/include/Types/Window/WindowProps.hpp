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


/**
 * @namespace CE::Types::Window
 * @brief Types and utilities related to window management
 * @details Contains type definitions and helper structures for managing windows,
 *          including properties for window creation and event handling.
 */
namespace CE::Types::Window {

namespace Events {
class Event;
}

/**
 * @struct WindowProps
 * @brief Structure holding window creation properties
 * @details Contains all necessary information to create and configure a window,
 *          including title, dimensions, and VSync setting.
 */
struct WindowProps {
	std::string title;								///< Window title displayed in the title bar
	unsigned int width;								///< Window width in pixels
	unsigned int height;							///< Window height in pixels
	bool VSync;										///< Vertical synchronization enabled/disabled

	/**
	 * @brief Constructor
	 * @param title Window title string
	 * @param width Window width in pixels
	 * @param height Window height in pixels
	 * @param VSync Enable or disable vertical synchronization
	 */
	WindowProps(const std::string& title, unsigned int width, unsigned int height, bool VSync);
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
 *          by window implementations to store window state and event handling.
 */
template<class T>
struct WindowData: WindowProps {
	CallbackFn<T> eventCallback;					///< Callback function for event handling

	/**
	 * @brief Constructor
	 * @param title Window title string
	 * @param width Window width in pixels
	 * @param height Window height in pixels
	 * @param VSync Enable or disable vertical synchronization
	 */
	WindowData(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync)
		: WindowProps(title, width, height, VSync) {}
};

}

#endif //CE_TYPES_WINDOW_WINDOWPROPS_HPP
