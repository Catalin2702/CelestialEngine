//
// Module: Events
// File: KeyEvent.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#include "Events/KeyEvent.hpp"

#include <sstream>


namespace CE::Events {

/**
 * @brief KeyPressedEvent constructor implementation
 * @param keycode Platform-specific key code
 * @param repeatCount Number of times the key has repeated (0 for initial press)
 */
KeyPressedEvent::KeyPressedEvent(const int keycode, const int repeatCount):
	I_KeyEvent(keycode), _repeatCount(repeatCount) {}

/**
 * @brief Converts KeyPressedEvent to string representation
 * @return std::string String in format "KeyPressedEvent: keycode (N repeats)"
 */
std::string KeyPressedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
	return ss.str();
}

/**
 * @brief KeyReleasedEvent constructor implementation
 * @param keycode Platform-specific key code
 */
KeyReleasedEvent::KeyReleasedEvent(const int keycode): I_KeyEvent(keycode) {}

/**
 * @brief Converts KeyReleasedEvent to string representation
 * @return std::string String in format "KeyReleasedEvent: keycode"
 */
std::string KeyReleasedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyReleasedEvent: " << _keyCode;
	return ss.str();
}

/**
 * @brief KeyTypedEvent constructor implementation
 * @param keycode Unicode code point of the typed character
 */
KeyTypedEvent::KeyTypedEvent(const unsigned int keycode): I_KeyEvent(keycode) {}

/**
 * @brief Converts KeyTypedEvent to string representation
 * @return std::string String in format "KeyTypedEvent: keycode"
 */
std::string KeyTypedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyTypedEvent: " << _keyCode;
	return ss.str();
}

}
