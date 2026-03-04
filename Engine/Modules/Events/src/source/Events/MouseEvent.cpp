//
// Module: CelestialEngine/Engine/Modules/Events
// File: MouseEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-02-28
//

#include "Events/MouseEvent.hpp"

#include <sstream>


namespace CE::Events {

/**
 * @brief MouseMovedEvent constructor implementation
 * @param x X coordinate of the mouse cursor
 * @param y Y coordinate of the mouse cursor
 */
MouseMovedEvent::MouseMovedEvent(const float x, const float y): _x(x), _y(y) {}

/**
 * @brief Converts MouseMovedEvent to string representation
 * @return std::string String in format "MouseMovedEvent: x, y"
 */
std::string MouseMovedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseMovedEvent: " << _x << ", " << _y;
	return ss.str();
}

/**
 * @brief MouseScrolledEvent constructor implementation
 * @param xOffset Horizontal scroll offset
 * @param yOffset Vertical scroll offset
 */
MouseScrolledEvent::MouseScrolledEvent(const float xOffset, const float yOffset):
	_xOffset(xOffset), _yOffset(yOffset) {}

/**
 * @brief Converts MouseScrolledEvent to string representation
 * @return std::string String in format "MouseScrolledEvent: xOffset, yOffset"
 */
std::string MouseScrolledEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseScrolledEvent: " << _xOffset << ", " << _yOffset;
	return ss.str();
}

/**
 * @brief I_MouseButtonEvent constructor implementation
 * @param button Platform-specific mouse button code
 */
I_MouseButtonEvent::I_MouseButtonEvent(const int button): _button(button) {}

/**
 * @brief MouseButtonPressedEvent constructor implementation
 * @param button Platform-specific mouse button code
 */
MouseButtonPressedEvent::MouseButtonPressedEvent(const int button): I_MouseButtonEvent(button) {}

/**
 * @brief Converts MouseButtonPressedEvent to string representation
 * @return std::string String in format "MouseButtonPressedEvent: button"
 */
std::string MouseButtonPressedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseButtonPressedEvent: " << _button;
	return ss.str();
}

/**
 * @brief MouseButtonReleasedEvent constructor implementation
 * @param button Platform-specific mouse button code
 */
MouseButtonReleasedEvent::MouseButtonReleasedEvent(const int button): I_MouseButtonEvent(button) {}

/**
 * @brief Converts MouseButtonReleasedEvent to string representation
 * @return std::string String in format "MouseButtonReleasedEvent: button"
 */
std::string MouseButtonReleasedEvent::ToString() const {
	std::stringstream ss;
	ss << "MouseButtonReleasedEvent: " << _button;
	return ss.str();
}

}
