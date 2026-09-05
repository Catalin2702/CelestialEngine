//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: I_RenderEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-21
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_HUB_EVENTS_I_RENDEREVENTHUBDISPATCHER_HPP
#define CE_CORE_HUB_EVENTS_I_RENDEREVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"
#include "Events/RenderEvent.hpp"


namespace CE::Core {

/**
 * @class I_RenderEventHubDispatcher
 * @brief Interface for the hub side that multicasts the rendering events
 * @details Implemented by the platform event hub dispatchers. Forwards the events declared in Events/RenderEvent.hpp
 *			- currently the VSync change the swapchain reports after switching present behaviour - to every subscriber
 *			of the matching multicast dispatcher. Defaults are no-ops so an implementation only overrides the events it
 *			actually routes.
 *
 *			It was called I_RenderContextEventHubDispatcher while a context owned the swap behaviour. There is no
 *			context any more: the swapchain owns presentation and the surface owns the view, so the name now says what
 *			the events are about rather than which object used to raise them.
 */
class CE_CORE_API I_RenderEventHubDispatcher {
public:
	virtual ~I_RenderEventHubDispatcher() = default;

public:
	/**
	 * @brief Hook multicasting the VSync change to the hub subscribers (no-op by default)
	 */
	virtual void DispatchRenderChangeVSyncEvent(Events::VSyncEvent&) {}
};

}

#endif //CE_CORE_HUB_EVENTS_I_RENDEREVENTHUBDISPATCHER_HPP
