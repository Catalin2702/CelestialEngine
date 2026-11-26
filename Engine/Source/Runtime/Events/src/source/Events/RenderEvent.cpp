//
// Module: CelestialEngine/Engine/Source/Runtime/Events
// File: RenderEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-21
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include "Events/RenderEvent.hpp"

#include <format>

namespace CE::Events {

std::string VSyncEvent::ToString() const {
	return std::format("{0}: {1}", GetName(), _state);
}

}
