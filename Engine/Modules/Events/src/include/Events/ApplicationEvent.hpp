//
// Module: CelestialEngine/Engine/Modules/Events
// File: ApplicationEvent.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_EVENTS_APPLICATIONEVENT_HPP
#define CE_EVENTS_APPLICATIONEVENT_HPP

#include "Events/I_Event.hpp"

#include "Define/DynamicLinker.hpp"
#include "Define/Event.hpp"

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
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

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
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

/**
 * @class AppTickEvent
 * @brief Event triggered on each application tick
 * @details This event can be used for fixed-rate updates or game logic that needs
 *			to run at regular intervals. The tick rate is typically independent of
 *			the frame rate.
 */
class CE_API AppTickEvent: public I_Event {
public:
	/**
	 * @brief Default constructor
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 */
	AppTickEvent(const bool isMutable = true): I_Event(isMutable) {}

public:
	EVENT_CLASS_TYPE(AppTick)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

/**
 * @class AppUpdateEvent
 * @brief Event triggered on each application update cycle
 * @details This event is generated every frame and can be used for general
 *			update logic, animations, and state management. Update frequency
 *			is typically tied to the frame rate.
 */
class CE_API AppUpdateEvent: public I_Event {
public:
	/**
	 * @brief Default constructor
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 */
	AppUpdateEvent(const bool isMutable = true): I_Event(isMutable) {}

public:
	EVENT_CLASS_TYPE(AppUpdate)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

/**
 * @class AppRenderEvent
 * @brief Event triggered on each render cycle
 * @details This event is generated when the application is ready to render a new frame.
 *			Handlers can use this event to submit rendering commands or update
 *			rendering-related state.
 */
class CE_API AppRenderEvent: public I_Event {
public:
	/**
	 * @brief Default constructor
	* @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 */
	AppRenderEvent(const bool isMutable = true): I_Event(isMutable) {}

public:
	EVENT_CLASS_TYPE(AppRender)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

/**
 * @class AppErrorEvent
 * @brief Event triggered on each error
 */
class CE_API AppErrorEvent: public I_Event {
public:
	AppErrorEvent(int errorCode, const char* description, bool isMutable = false);

public:
	[[nodiscard]] int GetErrorCode() const { return _errorCode; }

	[[nodiscard]] const char* GetDescription() const { return _description; }

	[[nodiscard]] std::string ToString() const override;

public:
	EVENT_CLASS_TYPE(AppError)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	int _errorCode;
	const char* _description;
};

}

#endif //CE_EVENTS_APPLICATIONEVENT_HPP