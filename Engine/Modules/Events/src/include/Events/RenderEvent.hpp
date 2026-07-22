//
// Module: CelestialEngine/Engine/Modules/Events
// File: RenderEvent.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_EVENTS_RENDEREVENT_HPP
#define CE_EVENTS_RENDEREVENT_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Event.hpp"

#include "Events/I_Event.hpp"

#include <string>

namespace CE::Events {

class CE_API VSyncEvent: public I_Event {
public:
	VSyncEvent(const bool state, const bool isMutable = true): I_Event(isMutable), _state(state) {}

public:
	[[nodiscard]] bool GetState() const { return _state; }

	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(VSyncChange)
	EVENT_CLASS_CATEGORY(EventCategoryRender)

private:
	bool _state;
};

}

#endif //CE_EVENTS_RENDEREVENT_HPP
