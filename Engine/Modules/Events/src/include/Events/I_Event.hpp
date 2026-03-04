//
// Module: CelestialEngine/Engine/Modules/Events
// File: I_Event.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-03
//

#pragma once

#ifndef CE_EVENTS_EVENT_HPP
#define CE_EVENTS_EVENT_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Type.hpp"

#include <functional>
#include <string>


/**
 * @namespace CE::Events
 * @brief Event system for handling various types of events in the engine
 * @details Provides the I_Event interface and EventDispatcher class for defining
 *			and dispatching events. Events are categorized by type and category
 *			to allow for efficient filtering and handling by different layers
 *			and systems within the engine.
 */
namespace CE::Events {

class I_Event;
class EventDispatcher;

/**
 * @enum EventType
 * @brief Enumeration of all possible event types in the engine
 * @details Each event type represents a specific kind of event that can occur.
 *			Events are categorized into application events, window events, input events, etc.
 */
enum class EventType: uint8_t {
	None = 0,

	WindowClose,									///< Window close event
	WindowResize,									///< Window resize event
	WindowFocus,									///< Window gained focus event
	WindowLostFocus,								///< Window lost focus event
	WindowMoved,									///< Window moved event

	AppTick,										///< Application tick event
	AppUpdate,										///< Application update event
	AppRender,										///< Application render event

	KeyPressed,										///< Keyboard key pressed event
	KeyReleased,									///< Keyboard key released event
	KeyTyped,										///< Keyboard character typed event

	MouseButtonPressed,								///< Mouse button pressed event
	MouseButtonReleased,							///< Mouse button released event
	MouseMoved,										///< Mouse moved event
	MouseScrolled,									///< Mouse scroll wheel event
};

/**
 * @enum EventCategory
 * @brief Bit flags for event categories
 * @details Categories are used to filter and route events efficiently.
 *			Multiple categories can be combined using bitwise OR.
 *			Each category is a single bit, allowing for efficient filtering.
 */
enum EventCategory {
	None = 0,
	EventCategoryApplication = BIT(0),				///< Application-related events (tick, update, render)
	EventCategoryInput = BIT(1),					///< Input-related events (keyboard, mouse)
	EventCategoryKeyboard = BIT(2),					///< Keyboard-specific events
	EventCategoryMouse = BIT(3),					///< Mouse-specific events
	EventCategoryMouseButton = BIT(4),				///< Mouse button-specific events
};

/**
 * @class I_Event
 * @brief Base interface class for all events in the engine
 * @details Abstract base class that defines the interface for all event types.
 *			Events are immutable once created and can be dispatched to handlers.
 *			The event system uses a polling mechanism where events are processed immediately.
 */
class CE_API I_Event {
	friend class EventDispatcher;

public:
	/**
	 * @brief Virtual destructor
	 * @details Ensures proper cleanup of derived event classes
	 */
	virtual ~I_Event() = default;

public:
	/**
	 * @brief Gets the runtime type of the event
	 * @return EventType The type of this event
	 * @details Pure virtual method that must be implemented by derived classes
	 */
	[[nodiscard]] virtual EventType GetEventType() const = 0;

	/**
	 * @brief Gets the name of the event type as a string
	 * @return const char* String name of the event type
	 * @details Pure virtual method that must be implemented by derived classes
	 */
	[[nodiscard]] virtual const char* GetName() const = 0;

	/**
	 * @brief Gets the category flags for this event
	 * @return int Bitwise OR combination of EventCategory flags
	 * @details Pure virtual method that must be implemented by derived classes
	 */
	[[nodiscard]] virtual int GetCategoryFlags() const = 0;

	/**
	 * @brief Converts the event to a string representation
	 * @return std::string String representation of the event
	 * @details Default implementation returns the event name. Can be overridden
	 *			for more detailed string representations.
	 */
	[[nodiscard]] virtual std::string ToString() const { return GetName(); }

public:
	/**
	 * @brief Checks if the event belongs to a specific category
	 * @param category The category to check against
	 * @return bool True if the event is in the specified category
	 * @details Uses bitwise AND to check category membership
	 */
	[[nodiscard]] bool IsInCategory(const EventCategory category) const { return GetCategoryFlags() & category; }

	/**
	 * @brief Checks if the event has been handled
	 * @return bool True if the event has been handled by a handler
	 * @details Once an event is handled, it typically stops propagating to other layers
	 */
	[[nodiscard]] bool IsHandled() const { return _handled; }

	/**
	 * @brief Marks the event as handled
	 * @details Sets the handled flag to true, indicating that the event has been processed
	 */
	void Consume() const {_handled = true; }

protected:
	mutable bool _handled = false;							///< Flag indicating whether the event has been handled
};


/**
 * @class EventDispatcher
 * @brief Helper class for dispatching events to type-specific handlers
 * @details The EventDispatcher uses the visitor pattern to dispatch events to
 *			appropriate handlers based on the event's concrete type. It ensures
 *			type safety by checking the event type at runtime before casting.
 */
class EventDispatcher {

public:
	/**
	 * @brief Constructor
	 * @param event Reference to the event to be dispatched
	 * @details Stores a reference to the event for later dispatching
	 */
	EventDispatcher(I_Event& event);

public:
	/**
	 * @brief Dispatches the event to a type-specific handler
	 * @tparam T The concrete event type to dispatch to
	 * @param func Handler function that takes a reference to the event type
	 * @return bool True if the event type matched and was dispatched
	 * @details Checks if the event is of type T, and if so, casts it and calls
	 *			the handler function. The event's handled flag is updated based
	 *			on the handler's return value.
	 */
	template<typename T>
	bool Dispatch(std::function<bool(T&)> func) {
		if (_event.GetEventType() == T::GetStaticType()) {
			if (func(static_cast<T&>(_event)))
				_event.Consume();
			return true;
		}
		return false;
	}

private:
	I_Event& _event;								///< Reference to the event being dispatched
};

/**
 * @brief Stream output operator for events
 * @param os Output stream
 * @param event Event to output
 * @return std::ostream& Reference to the output stream
 * @details Allows events to be printed to output streams using the ToString() method
 */
inline std::ostream& operator<<(std::ostream& os, const I_Event& event) {
	return os << event.ToString();
}

/**
 * @brief Format function for events (for use with formatting libraries)
 * @param event Event to format
 * @return std::string Formatted string representation of the event
 * @details Enables events to be used with modern formatting libraries like fmt
 */
inline std::string format_as(const I_Event& event) {
	return event.ToString();
}

}

#endif //CE_EVENTS_EVENT_HPP
