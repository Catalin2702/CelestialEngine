//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: I_RenderContextEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-21
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_RENDERCONTEXTEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_RENDERCONTEXTEVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"
#include "Events/RenderEvent.hpp"


namespace CE::Core {

/**
 * @class I_RenderContextEventHubDispatcher
 * @brief Interface for the hub side that multicasts render context events
 * @details Implemented by the platform event hub dispatchers. Forwards render-context events (currently the VSync
 *			change fired by the context after switching swap behaviour) to every subscriber of the matching multicast
 *			dispatcher. Defaults are no-ops so an implementation only overrides the events it actually routes.
 */
class CE_CORE_API I_RenderContextEventHubDispatcher {
public:
	virtual ~I_RenderContextEventHubDispatcher() = default;

public:
	/**
	 * @brief Hook multicasting the render context change v sync event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchRenderContextChangeVSyncEvent(Events::VSyncEvent&) {}
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_RENDERCONTEXTEVENTHUBDISPATCHER_HPP
