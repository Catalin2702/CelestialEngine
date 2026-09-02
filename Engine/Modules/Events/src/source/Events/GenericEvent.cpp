//
// Module: CelestialEngine/Engine/Modules/Events
// File: GenericEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Events/GenericEvent.hpp"

#include <format>


namespace CE::Events {

ErrorEvent::ErrorEvent(const int errorCode, const char* description, const bool isMutable):
	I_Event(isMutable), _errorCode(errorCode), _description(description) {}

std::string ErrorEvent::ToString() const {
	return std::format("{0}: ErrorCode: {1}. Description: {2}", GetName(), _errorCode, _description);
}

}
