//
// Module: CelestialEngine/Engine/Modules/Events
// File: ApplicationEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Events/ApplicationEvent.hpp"

#include <sstream>

namespace CE::Events {

WindowResizeEvent::WindowResizeEvent(const unsigned int width, const unsigned int height, const bool isMutable):
	I_Event(isMutable), _width(width), _height(height) {}

std::string WindowResizeEvent::ToString() const {
	std::stringstream ss;
	ss << GetName() << ": " << _width << ", " << _height;
	return ss.str();
}

AppErrorEvent::AppErrorEvent(int errorCode, const char* description, bool isMutable):
	I_Event(isMutable), _errorCode(errorCode), _description(description) {}

std::string AppErrorEvent::ToString() const {
	std::stringstream ss;
	ss << GetName() << ": " << "ErrorCode: " << _errorCode << ". Description: " << _description;
	return ss.str();
}

}
