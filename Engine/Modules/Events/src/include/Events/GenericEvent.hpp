//
// Module: CelestialEngine/Engine/Modules/Events
// File: GenericEvent.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
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
 */
class CE_API ErrorEvent: public I_Event {
public:
	ErrorEvent(int errorCode, const char* description, bool isMutable = false);

public:
	[[nodiscard]] int GetErrorCode() const { return _errorCode; }

	[[nodiscard]] const char* GetDescription() const { return _description; }

	[[nodiscard]] std::string ToString() const override;

public:
	EVENT_CLASS_TYPE(Error)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	int _errorCode;
	const char* _description;
};

}

#endif //CE_EVENTS_GENERICEVENT_HPP
