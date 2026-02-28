//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Events/I_Event.hpp"


namespace CE::Events {

EventDispatcher::EventDispatcher(I_Event &event): _event(event) {}

}
