//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: I_ApplicationEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_APPLICATIONEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_APPLICATIONEVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/GenericEvent.hpp"

namespace CE::Core {

class CE_API I_ApplicationEventHubDispatcher {
public:
	virtual ~I_ApplicationEventHubDispatcher() = default;

public:
	virtual void DispatchAppErrorEvent(Events::ErrorEvent&) {}
	virtual void DispatchAppRenderEvent(Events::AppRenderEvent&) {}
	virtual void DispatchAppTickEvent(Events::AppTickEvent&) {}
	virtual void DispatchAppUpdateEvent(Events::AppUpdateEvent&) {}
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_APPLICATIONEVENTHUBDISPATCHER_HPP
