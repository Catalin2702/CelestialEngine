//
// Module: CelestialEngine/Engine/Source/Runtime/Events
// File: ApplicationEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include "Events/ApplicationEvent.hpp"

#include <format>


namespace CE::Events {

std::string AppTickEvent::ToString() const {
	return std::format("{0}: DeltaTime: {1}", GetName(), _deltaTime);
}

}
