//
// Module: CelestialEngine/Engine/Modules/Events
// File: WindowEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#include "Events/WindowEvent.hpp"

#include <format>

namespace CE::Events {

WindowResizeEvent::WindowResizeEvent(const unsigned int width, const unsigned int height, const bool isMutable):
	I_Event(isMutable), _width(width), _height(height) {}

std::string WindowResizeEvent::ToString() const {
	return std::format("{0}: {1}, {2}", GetName(), _width, _height);
}

WindowFocusEvent::WindowFocusEvent(const bool focused, const bool isMutable):
	I_Event(isMutable), _focused(focused) {}

std::string WindowFocusEvent::ToString() const {
	return std::format("{0}: {1}", GetName(), _focused ? "gained" : "lost");
}

}