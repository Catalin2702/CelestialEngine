//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: I_KeyboardEventHubDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_MAINHUB_EVENTS_I_KEYBOARDEVENTHUBDISPATCHER_HPP
#define CE_CORE_MAINHUB_EVENTS_I_KEYBOARDEVENTHUBDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Events/KeyEvent.hpp"

namespace CE::Core {

class CE_API I_KeyboardEventHubDispatcher {
public:
	virtual ~I_KeyboardEventHubDispatcher() = default;

public:
	virtual void DispatchKeyPressedEvent(Events::KeyPressedEvent&) {}
	virtual void DispatchKeyReleasedEvent(Events::KeyReleasedEvent&) {}
	virtual void DispatchKeyTypedEvent(Events::KeyTypedEvent&) {}
};

}

#endif //CE_CORE_MAINHUB_EVENTS_I_KEYBOARDEVENTHUBDISPATCHER_HPP
