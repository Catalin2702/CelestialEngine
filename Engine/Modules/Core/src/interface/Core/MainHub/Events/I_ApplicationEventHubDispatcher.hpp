//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: I_ApplicationEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_APPLICATIONEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_APPLICATIONEVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/GenericEvent.hpp"

namespace CE::Core {

/**
 * @class I_ApplicationEventHubDispatcher
 * @brief Interface for the hub side that multicasts application lifecycle events
 * @details Implemented by the platform event hub dispatchers. Each Dispatch* hook forwards an application event
 *			(tick/update/render, plus errors) to every subscriber of the matching multicast dispatcher. Defaults are
 *			no-ops so an implementation only overrides the events it actually routes.
 */
class CE_API I_ApplicationEventHubDispatcher {
public:
	virtual ~I_ApplicationEventHubDispatcher() = default;

public:
	/**
	 * @brief Hook multicasting the app error event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchAppErrorEvent(Events::ErrorEvent&) {}
	/**
	 * @brief Hook multicasting the app render event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchAppRenderEvent(Events::AppRenderEvent&) {}
	/**
	 * @brief Hook multicasting the app tick event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchAppTickEvent(Events::AppTickEvent&) {}
	/**
	 * @brief Hook multicasting the app update event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchAppUpdateEvent(Events::AppUpdateEvent&) {}
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_APPLICATIONEVENTHUBDISPATCHER_HPP
