//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Events/Event.hpp"

#include <print>

namespace CE::Events {

EventDispatcher::EventDispatcher(Event &event): _event(event) {}

}
