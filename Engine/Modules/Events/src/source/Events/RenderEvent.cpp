//
// Module: CelestialEngine/Engine/Modules/Events
// File: RenderEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#include "Events/RenderEvent.hpp"

#include <format>

namespace CE::Events {

std::string VSyncChangeEvent::ToString() const {
	return std::format("{0}: {1}", GetName(), _state);
}

}
