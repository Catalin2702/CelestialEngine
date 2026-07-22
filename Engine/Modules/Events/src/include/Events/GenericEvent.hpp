//
// Module: CelestialEngine/Engine/Modules/Events
// File: GenericEvent.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_EVENTS_GENERICEVENT_HPP
#define CE_EVENTS_GENERICEVENT_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Event.hpp"

#include "Events/I_Event.hpp"

#include <string>

namespace CE::Events {

/**
 * @class ErrorEvent
 * @brief Event triggered on each error
 * @details Carries a numeric error code and a human-readable description. Raised by the platform layers (window,
 *			application) and multicast through the event hub; immutable by default so no handler can consume it and
 *			hide the error from later subscribers.
 */
class CE_API ErrorEvent: public I_Event {
public:
	/**
	 * @brief Constructor
	 * @param errorCode Numeric error code (platform-specific)
	 * @param description Human-readable error description (non-owning; must outlive the event)
	 * @param isMutable Flag indicating whether the event is mutable. If not it can't be consumed
	 */
	ErrorEvent(int errorCode, const char* description, bool isMutable = false);

public:
	/**
	 * @brief Gets the numeric error code
	 */
	[[nodiscard]] int GetErrorCode() const { return _errorCode; }

	/**
	 * @brief Gets the human-readable error description
	 */
	[[nodiscard]] const char* GetDescription() const { return _description; }

	/**
	 * @brief Converts the event to a string representation
	 * @return std::string String containing the error code and description
	 */
	[[nodiscard]] std::string ToString() const override;

public:
	EVENT_CLASS_TYPE(Error)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	int _errorCode;									///< Numeric error code (platform-specific)
	const char* _description;						///< Human-readable error description (non-owning)
};

}

#endif //CE_EVENTS_GENERICEVENT_HPP
