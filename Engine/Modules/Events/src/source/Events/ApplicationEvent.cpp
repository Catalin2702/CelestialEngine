//
// Module: CelestialEngine/Engine/Modules/Events
// File: ApplicationEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Events/ApplicationEvent.hpp"

#include <format>


namespace CE::Events {

std::string AppTickEvent::ToString() const {
	return std::format("{0}: DeltaTime: {1}", GetName(), _deltaTime);
}

}
