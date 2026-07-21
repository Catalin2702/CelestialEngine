//
// Module: CelestialEngine/Engine/Modules/Events
// File: MouseEvent.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#pragma once

#ifndef CE_EVENTS_MOUSEEVENT_HPP
#define CE_EVENTS_MOUSEEVENT_HPP

#include "Events/I_Event.hpp"

#include "Define/DynamicLinker.hpp"
#include "Define/Event.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <string>

namespace CE::Events {

/**
 * @class I_MouseMovedEvent
 * @brief Base class for mouse moved events
 * @details Abstract base class that provides common functionality for mouse movement events.
 *			Stores the new cursor position and belongs to multiple mouse-related categories.
 */
class CE_API I_MouseMovedEvent: public I_Event {
protected:
	/**
	 * @brief Constructor
	 * @param x New X coordinate of the mouse cursor in window space
	 * @param y New Y coordinate of the mouse cursor in window space
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 * @details Creates a mouse moved event with the specified cursor position
	 */
	I_MouseMovedEvent(float x, float y, bool isMutable = true);

public:
	/**
	 * @brief Gets the X coordinate of the mouse cursor
	 * @return float X position in window coordinates
	 */
	[[nodiscard]] float GetX() const {return _x; }

	/**
	 * @brief Gets the Y coordinate of the mouse cursor
	 * @return float Y position in window coordinates
	 */
	[[nodiscard]] float GetY() const {return _y; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

protected:
	float _x, _y;									///< Mouse cursor position in window coordinates
};

/**
 * @class MouseMovedEvent
 * @brief Event triggered when the mouse cursor is moved
 * @details Contains the new cursor position in window coordinates. This event is generated
 *			whenever the user moves the mouse within the window, allowing the application to
 *			respond to cursor movement for tasks like camera control, UI interaction, etc.
 */
class CE_API MouseMovedEvent final: public I_MouseMovedEvent {
public:
	/**
	 * @brief Constructor
	 * @param x New X coordinate of the mouse cursor in window space
	 * @param y New Y coordinate of the mouse cursor in window space
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 * @details Creates a mouse moved event with the specified cursor position
	 */
	MouseMovedEvent(float x, float y, bool isMutable = true);

	/**
	 * @brief Converts the event to a string representation
	 * @return std::string String containing mouse coordinates
	 */
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseMoved)
};

/**
 * @class MouseWheelScrolledEvent
 * @brief Event triggered when the mouse scroll wheel is moved
 * @details Contains the scroll offset in both X and Y directions. Most mice only
 *			have vertical scrolling (Y offset), but some devices support horizontal
 *			scrolling (X offset) as well.
 */
class CE_API MouseWheelScrolledEvent final: public I_Event {
public:
	/**
	 * @brief Constructor
	 * @param xOffset Horizontal scroll offset (positive = right, negative = left)
	 * @param yOffset Vertical scroll offset (positive = up, negative = down)
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 * @details Creates a mouse scrolled event with the specified scroll offsets
	 */
	MouseWheelScrolledEvent(float xOffset, float yOffset, bool isMutable = true);

	/**
	 * @brief Gets the horizontal scroll offset
	 * @return float X scroll offset
	 */
	[[nodiscard]] float GetXOffset() const { return _xOffset; }

	/**
	 * @brief Gets the vertical scroll offset
	 * @return float Y scroll offset
	 */
	[[nodiscard]] float GetYOffset() const { return _yOffset; }

	/**
	 * @brief Converts the event to a string representation
	 * @return std::string String containing scroll offsets
	 */
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
	float _xOffset, _yOffset;						///< Scroll wheel offsets
};

/**
 * @class I_MouseButtonEvent
 * @brief Base class for mouse button events
 * @details Abstract base class that provides common functionality for mouse button
 *			press and release events. Stores the button code and belongs to multiple
 *			mouse-related categories.
 */
class CE_API I_MouseButtonEvent: public I_Event {
protected:
	/**
	 * @brief Protected constructor
	 * @param button Platform-specific mouse button code
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 * @details Only derived classes can construct a mouse button event
	 */
	I_MouseButtonEvent(Types::MouseButtonCode button, bool isMutable = true);

public:
	/**
	 * @brief Gets the mouse button code
	 * @return Types::MouseButtonCode Platform-specific mouse button code
	 */
	[[nodiscard]] Types::MouseButtonCode GetMouseButton() const { return _button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)

protected:
	Types::MouseButtonCode _button;									///< Platform-specific mouse button code
};

/**
 * @class MouseButtonPressedEvent
 * @brief Event triggered when a mouse button is pressed
 * @details Contains the button code of the pressed button. Common button codes
 *			are 0 (left), 1 (right), 2 (middle), but additional buttons may be
 *			available depending on the hardware.
 */
class CE_API MouseButtonPressedEvent final: public I_MouseButtonEvent {
public:
	/**
	 * @brief Constructor
	 * @param button Platform-specific mouse button code
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 * @details Creates a mouse button pressed event with the specified button
	 */
	MouseButtonPressedEvent(Types::MouseButtonCode button, bool isMutable = true);

	/**
	 * @brief Converts the event to a string representation
	 * @return std::string String containing the button code
	 */
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseButtonPressed)
};

/**
 * @class MouseButtonReleasedEvent
 * @brief Event triggered when a mouse button is released
 * @details Contains the button code of the released button. This event is generated
 *			when the user releases a previously pressed mouse button.
 */
class CE_API MouseButtonReleasedEvent final: public I_MouseButtonEvent {
public:
	/**
	 * @brief Constructor
	 * @param button Platform-specific mouse button code
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 * @details Creates a mouse button released event with the specified button
	 */
	MouseButtonReleasedEvent(Types::MouseButtonCode button, bool isMutable = true);

	/**
	 * @brief Converts the event to a string representation
	 * @return std::string String containing the button code
	 */
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseButtonReleased)
};

/**
 * @class MouseDraggedEvent
 * @brief Event triggered when the mouse is moved while a button is pressed
 * @details Contains the new cursor position and the button code of the button being dragged.
 *			This event is generated when the user moves the mouse while holding down a mouse button,
 *			allowing for drag-and-drop interactions or similar functionality.
 */
class CE_API MouseDraggedEvent final: public I_MouseMovedEvent {
public:
	/**
	 * @brief Constructor
	 * @param button Platform-specific mouse button code
	 * @param x New X coordinate of the mouse cursor in window space
	 * @param y New Y coordinate of the mouse cursor in window space
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 * @details Creates a mouse dragged event with the specified button and cursor position
	 */
	MouseDraggedEvent(Types::MouseButtonCode button, float x, float y, bool isMutable = true);

	/**
	 * @brief Gets the mouse button code for the button being dragged
	 * @return Types::MouseButtonCode Platform-specific mouse button code
	 */
	[[nodiscard]] Types::MouseButtonCode GetMouseButton() const { return _button; }

	/**
	 * @brief Converts the event to a string representation
	 * @return std::string String containing the button code and mouse coordinates
	 */
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseDragged)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)

private:
	Types::MouseButtonCode _button;									///< Platform-specific mouse button code for the button being dragged
};

}

#endif //CE_EVENTS_MOUSEEVENT_HPP
