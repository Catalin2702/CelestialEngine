//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: I_WindowEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_WINDOWEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_WINDOWEVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Events/GenericEvent.hpp"
#include "Events/WindowEvent.hpp"

namespace CE::Core {

class CE_API I_WindowEventHubDispatcher {
public:
	virtual ~I_WindowEventHubDispatcher() = default;

public:
	virtual void DispatchWindowResizeEvent(Events::WindowResizeEvent&) {}
	virtual void DispatchWindowCloseEvent(Events::WindowCloseEvent&) {}
	virtual void DispatchWindowErrorEvent(Events::ErrorEvent&) {}
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_WINDOWEVENTHUBDISPATCHER_HPP
