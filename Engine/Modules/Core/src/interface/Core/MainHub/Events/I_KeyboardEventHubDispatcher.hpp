//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: I_KeyboardEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_KEYBOARDEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_KEYBOARDEVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Events/KeyEvent.hpp"


namespace CE::Core {

/**
 * @class I_KeyboardEventHubDispatcher
 * @brief Interface for the hub side that multicasts keyboard events
 * @details Implemented by the platform event hub dispatchers. Each Dispatch* hook forwards a keyboard event
 *			(pressed/released/typed) to every subscriber of the matching multicast dispatcher. Defaults are no-ops so an
 *			implementation only overrides the events it actually routes.
 */
class CE_CORE_API I_KeyboardEventHubDispatcher {
public:
	virtual ~I_KeyboardEventHubDispatcher() = default;

public:
	/**
	 * @brief Hook multicasting the key pressed event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchKeyPressedEvent(Events::KeyPressedEvent&) {}
	/**
	 * @brief Hook multicasting the key released event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchKeyReleasedEvent(Events::KeyReleasedEvent&) {}
	/**
	 * @brief Hook multicasting the key typed event to the hub subscribers (no-op by default)
	 */
	virtual void DispatchKeyTypedEvent(Events::KeyTypedEvent&) {}
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_KEYBOARDEVENTHUBDISPATCHER_HPP
