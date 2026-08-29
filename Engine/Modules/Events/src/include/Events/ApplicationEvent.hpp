//
// Module: CelestialEngine/Engine/Modules/Events
// File: ApplicationEvent.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_EVENTS_APPLICATIONEVENT_HPP
#define CE_EVENTS_APPLICATIONEVENT_HPP

#include "Define/Event.hpp"
#include "Events/I_Event.hpp"


namespace CE::Events {

/**
 * @class AppTickEvent
 * @brief Event triggered on each application tick
 * @details This event can be used for fixed-rate updates or game logic that needs
 *			to run at regular intervals. The tick rate is typically independent of
 *			the frame rate.
 */
class AppTickEvent: public I_Event {
public:
	/**
	 * @brief Default constructor
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 */
	AppTickEvent(const bool isMutable = true): I_Event(isMutable) {}

public:
	EVENT_CLASS_TYPE(AppTick)
	EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

/**
 * @class AppUpdateEvent
 * @brief Event triggered on each application update cycle
 * @details This event is generated every frame and can be used for general
 *			update logic, animations, and state management. Update frequency
 *			is typically tied to the frame rate.
 */
class AppUpdateEvent: public I_Event {
public:
	/**
	 * @brief Default constructor
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 */
	AppUpdateEvent(const bool isMutable = true): I_Event(isMutable) {}

public:
	EVENT_CLASS_TYPE(AppUpdate)
	EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

/**
 * @class AppRenderEvent
 * @brief Event triggered on each render cycle
 * @details This event is generated when the application is ready to render a new frame.
 *			Handlers can use this event to submit rendering commands or update
 *			rendering-related state.
 */
class AppRenderEvent: public I_Event {
public:
	/**
	 * @brief Default constructor
	* @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 */
	AppRenderEvent(const bool isMutable = true): I_Event(isMutable) {}

public:
	EVENT_CLASS_TYPE(AppRender)
	EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

}

#endif //CE_EVENTS_APPLICATIONEVENT_HPP