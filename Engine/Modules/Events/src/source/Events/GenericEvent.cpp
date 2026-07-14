//
// Module: CelestialEngine/Engine/Modules/Events
// File: GenericEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Events/GenericEvent.hpp"

#include <sstream>

namespace CE::Events {

ErrorEvent::ErrorEvent(const int errorCode, const char* description, const bool isMutable):
	I_Event(isMutable), _errorCode(errorCode), _description(description) {}

std::string ErrorEvent::ToString() const {
	std::stringstream ss;
	ss << GetName() << ": " << "ErrorCode: " << _errorCode << ". Description: " << _description;
	return ss.str();
}

}
