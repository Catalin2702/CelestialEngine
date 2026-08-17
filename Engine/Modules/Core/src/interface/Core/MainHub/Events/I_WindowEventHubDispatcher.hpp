//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: I_WindowEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_WINDOWEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_WINDOWEVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Events/GenericEvent.hpp"
#include "Events/WindowEvent.hpp"


namespace CE::Core {

/**
 * @class I_WindowEventHubDispatcher
 * @brief Interface for the hub side that multicasts window events
 * @details Implemented by the platform event hub dispatchers. Each Dispatch* hook forwards a window event
 *			(resize/close/error/focus) to every subscriber of the matching multicast dispatcher. Defaults are no-ops so an
 *			implementation only overrides the events it actually routes.
 */
class CE_CORE_API I_WindowEventHubDispatcher {
public:
	virtual ~I_WindowEventHubDispatcher() = default;

public:
	/**
	 * @brief Hook multicasting the window resize event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchWindowResizeEvent(Events::WindowResizeEvent&) {}
	/**
	 * @brief Hook multicasting the window close event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchWindowCloseEvent(Events::WindowCloseEvent&) {}
	/**
	 * @brief Hook multicasting the window error event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchWindowErrorEvent(Events::ErrorEvent&) {}
	/**
	 * @brief Hook multicasting the window focus event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchWindowFocusEvent(Events::WindowFocusEvent&) {}
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_WINDOWEVENTHUBDISPATCHER_HPP
