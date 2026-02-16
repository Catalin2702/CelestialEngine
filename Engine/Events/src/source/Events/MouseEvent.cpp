//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Events/MouseEvent.hpp"

#include <sstream>

CE::Events::MouseMovedEvent::MouseMovedEvent(const float x, const float y): _x(x), _y(y) {}

std::string CE::Events::MouseMovedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseMovedEvent: " << _x << ", " << _y;
	return ss.str();
}

CE::Events::MouseButtonEvent::MouseButtonEvent(const int button): _button(button) {}

CE::Events::MouseButtonPressedEvent::MouseButtonPressedEvent(const int button): MouseButtonEvent(button) {}

std::string CE::Events::MouseButtonPressedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseButtonPressedEvent: " << _button;
	return ss.str();
}

CE::Events::MouseButtonReleasedEvent::MouseButtonReleasedEvent(const int button): MouseButtonEvent(button) {}

std::string CE::Events::MouseButtonReleasedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseButtonReleasedEvent: " << _button;
	return ss.str();
}
