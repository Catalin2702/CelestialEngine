//
// Module: CelestialEngine/Engine/Modules/Events
// File: MouseEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Events/MouseEvent.hpp"

#include <sstream>

namespace CE::Events {

I_MouseMovedEvent::I_MouseMovedEvent(const float x, const float y, const bool isMutable):
	I_Event(isMutable), _x(x), _y(y)  {}

MouseMovedEvent::MouseMovedEvent(const float x, const float y, const bool isMutable):
	I_MouseMovedEvent(x, y, isMutable) {}

std::string MouseMovedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseMovedEvent: " << GetX() << ", " << GetY();
	return ss.str();
}

MouseScrolledEvent::MouseScrolledEvent(const float xOffset, const float yOffset, const bool isMutable):
	I_Event(isMutable), _xOffset(xOffset), _yOffset(yOffset) {}

std::string MouseScrolledEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseScrolledEvent: " << _xOffset << ", " << _yOffset;
	return ss.str();
}

I_MouseButtonEvent::I_MouseButtonEvent(const Types::MouseButtonCode button, const bool isMutable):
	I_Event(isMutable), _button(button) {}

MouseButtonPressedEvent::MouseButtonPressedEvent(const Types::MouseButtonCode button, const bool isMutable):
	I_MouseButtonEvent(button, isMutable) {}

std::string MouseButtonPressedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseButtonPressedEvent: " << _button;
	return ss.str();
}

MouseButtonReleasedEvent::MouseButtonReleasedEvent(const Types::MouseButtonCode button, const bool isMutable):
	I_MouseButtonEvent(button, isMutable) {}

std::string MouseButtonReleasedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseButtonReleasedEvent: " << _button;
	return ss.str();
}

MouseDraggedEvent::MouseDraggedEvent(const Types::MouseButtonCode button, const float x, const float y, const bool isMutable):
	I_MouseMovedEvent(x, y, isMutable), _button(button)  {}

std::string MouseDraggedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseDraggedEvent: " << GetX() << ", " << GetY() << " with button " << GetMouseButton();
	return ss.str();
}

}
