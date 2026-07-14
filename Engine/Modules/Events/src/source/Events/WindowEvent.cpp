//
// Module: CelestialEngine/Engine/Modules/Events
// File: WindowEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Events/WindowEvent.hpp"

#include <sstream>

namespace CE::Events {

WindowResizeEvent::WindowResizeEvent(const unsigned int width, const unsigned int height, const bool isMutable):
	I_Event(isMutable), _width(width), _height(height) {}

std::string WindowResizeEvent::ToString() const {
	std::stringstream ss;
	ss << GetName() << ": " << _width << ", " << _height;
	return ss.str();
}

}