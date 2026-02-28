//
// Module: Events
// File: I_Event.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#include "Events/I_Event.hpp"


namespace CE::Events {

EventDispatcher::EventDispatcher(I_Event &event): _event(event) {}

}
