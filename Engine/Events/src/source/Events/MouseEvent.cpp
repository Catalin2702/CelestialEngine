//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Events/MouseEvent.hpp"

#include <sstream>


namespace CE::Events {

MouseMovedEvent::MouseMovedEvent(const float x, const float y):
	_x(x), _y(y) {}

std::string MouseMovedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseMovedEvent: " << _x << ", " << _y;
	return ss.str();
}

MouseScrolledEvent::MouseScrolledEvent(const float xOffset, const float yOffset):
	_xOffset(xOffset), _yOffset(yOffset) {}

std::string MouseScrolledEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseScrolledEvent: " << _xOffset << ", " << _yOffset;
	return ss.str();
}

MouseButtonEvent::MouseButtonEvent(const int button):
	_button(button) {}

MouseButtonPressedEvent::MouseButtonPressedEvent(const int button): MouseButtonEvent(button) {}

std::string MouseButtonPressedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseButtonPressedEvent: " << _button;
	return ss.str();
}

MouseButtonReleasedEvent::MouseButtonReleasedEvent(const int button): MouseButtonEvent(button) {}

std::string MouseButtonReleasedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseButtonReleasedEvent: " << _button;
	return ss.str();
}

}
