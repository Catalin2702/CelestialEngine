//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: I_MouseEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_MOUSEEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_MOUSEEVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"
#include "Events/MouseEvent.hpp"


namespace CE::Core {

/**
 * @class I_MouseEventHubDispatcher
 * @brief Interface for the hub side that multicasts mouse events
 * @details Implemented by the platform event hub dispatchers. Each Dispatch* hook forwards a mouse event
 *			(moved/button/dragged/wheel) to every subscriber of the matching multicast dispatcher. Defaults are no-ops so
 *			an implementation only overrides the events it actually routes.
 */
class CE_CORE_API I_MouseEventHubDispatcher {
public:
	virtual ~I_MouseEventHubDispatcher() = default;

public:
	/**
	 * @brief Hook multicasting the mouse moved event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchMouseMovedEvent(Events::MouseMovedEvent&) {}
	/**
	 * @brief Hook multicasting the mouse button pressed event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchMouseButtonPressedEvent(Events::MouseButtonPressedEvent&) {}
	/**
	 * @brief Hook multicasting the mouse button released event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent&) {}
	/**
	 * @brief Hook multicasting the mouse dragged event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchMouseDraggedEvent(Events::MouseDraggedEvent&) {}
	/**
	 * @brief Hook multicasting the mouse wheel scrolled event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchMouseWheelScrolledEvent(Events::MouseWheelScrolledEvent&) {}
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_MOUSEEVENTHUBDISPATCHER_HPP
