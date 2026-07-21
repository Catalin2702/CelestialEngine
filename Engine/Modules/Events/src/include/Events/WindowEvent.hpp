//
// Module: CelestialEngine/Engine/Modules/Events
// File: WindowEvent.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#pragma once

#ifndef CE_EVENTS_WINDOWEVENT_HPP
#define CE_EVENTS_WINDOWEVENT_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Event.hpp"

#include "Events/I_Event.hpp"

#include <string>

namespace CE::Events {


/**
 * @class WindowResizeEvent
 * @brief Event triggered when the application window is resized
 * @details Contains the new width and height of the window. This event is generated
 *			by the windowing system when the user resizes the window or when the window
 *			size changes programmatically.
 */
class CE_API WindowResizeEvent: public I_Event {
public:
	/**
	 * @brief Constructor
	 * @param width New width of the window in pixels
	 * @param height New height of the window in pixels
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 * @details Creates a window resize event with the specified dimensions
	 */
	WindowResizeEvent(unsigned int width, unsigned int height, bool isMutable = true);

public:
	/**
	 * @brief Gets the new width of the window
	 * @return unsigned int Width in pixels
	 */
	[[nodiscard]] unsigned int GetWidth() const { return _width;}

	/**
	 * @brief Gets the new height of the window
	 * @return unsigned int Height in pixels
	 */
	[[nodiscard]] unsigned int GetHeight() const { return _height; }

	/**
	 * @brief Converts the event to a string representation
	 * @return std::string String containing event details (width and height)
	 */
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(WindowResize)
	EVENT_CLASS_CATEGORY(EventCategoryWindow)

private:
	unsigned int _width, _height;					///< New window dimensions in pixels
};

/**
 * @class WindowCloseEvent
 * @brief Event triggered when the application window is closed
 * @details This event is generated when the user clicks the close button or
 *			when the window is closed programmatically. Handlers can use this
 *			event to perform cleanup before the application exits.
 */
class CE_API WindowCloseEvent: public I_Event {
public:
	/**
	 * @brief Default constructor
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 */
	WindowCloseEvent(const bool isMutable = true): I_Event(isMutable) {}

public:
	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryWindow)
};

}

#endif //CE_EVENTS_WINDOWEVENT_HPP
