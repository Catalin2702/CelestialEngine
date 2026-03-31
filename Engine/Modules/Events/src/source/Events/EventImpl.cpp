//
// Module: CelestialEngine/Engine/Modules/Events
// File: I_Event.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-02-28
//

#include "Events/I_Event.hpp"


namespace CE::Events {

/**
 * @brief EventDispatcher constructor implementation
 * @param event Reference to the event to be dispatched
 */
EventDispatcher::EventDispatcher(I_Event& event): _event(event) {}

}
