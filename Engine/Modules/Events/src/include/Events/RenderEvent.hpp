//
// Module: CelestialEngine/Engine/Modules/Events
// File: RenderEvent.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-21
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_EVENTS_RENDEREVENT_HPP
#define CE_EVENTS_RENDEREVENT_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Event.hpp"

#include "Events/I_Event.hpp"

#include <string>


namespace CE::Events {

/**
 * @class VSyncEvent
 * @brief Event triggered when the render context's VSync state changes
 * @details Fired by the render context (Metal or OpenGL) after the swap behaviour has actually been switched, and
 *			flows context → event hub → subscribers. The application reacts by reconciling its frame pacing with the
 *			new state (Cocoa: tick loop vs CAMetalDisplayLink; GLFW: frame-limiter target).
 */
class CE_EVENTS_API VSyncEvent: public I_Event {
public:
	/**
	 * @brief Constructor
	 * @param state New VSync state (true = enabled, false = disabled)
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 */
	VSyncEvent(const bool state, const bool isMutable = true): I_Event(isMutable), _state(state) {}

public:
	/**
	 * @brief Gets the new VSync state
	 * @return bool True if VSync was enabled, false if it was disabled
	 */
	[[nodiscard]] bool GetState() const { return _state; }

	/**
	 * @brief Converts the event to a string representation
	 * @return std::string String containing the new VSync state
	 */
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(VSyncChange)
	EVENT_CLASS_CATEGORY(EventCategoryRender)

private:
	bool _state;									///< New VSync state (true = enabled)
};

}

#endif //CE_EVENTS_RENDEREVENT_HPP
