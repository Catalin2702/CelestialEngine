//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: I_MouseEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_MOUSEEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_MOUSEEVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Events/MouseEvent.hpp"

namespace CE::Core {

class CE_API I_MouseEventHubDispatcher {
public:
	virtual ~I_MouseEventHubDispatcher() = default;

public:
	virtual void DispatchMouseMovedEvent(Events::MouseMovedEvent&) {}
	virtual void DispatchMouseButtonPressedEvent(Events::MouseButtonPressedEvent&) {}
	virtual void DispatchMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent&) {}
	virtual void DispatchMouseDraggedEvent(Events::MouseDraggedEvent&) {}
	virtual void DispatchMouseWheelScrolledEvent(Events::MouseWheelScrolledEvent&) {}
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_MOUSEEVENTHUBDISPATCHER_HPP
