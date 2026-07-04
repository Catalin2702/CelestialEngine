//
// Module: CelestialEngine/Engine/Modules/Events
// File: KeyEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-09
//

#include "Events/KeyEvent.hpp"

#include <sstream>


namespace CE::Events {

KeyPressedEvent::KeyPressedEvent(const KeyType keycode, const int repeatCount, const bool isMutable):
	I_KeyEvent(keycode, isMutable), _repeatCount(repeatCount) {}

std::string KeyPressedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
	return ss.str();
}

KeyReleasedEvent::KeyReleasedEvent(const KeyType keycode, const bool isMutable):
	I_KeyEvent(keycode, isMutable) {}

std::string KeyReleasedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyReleasedEvent: " << _keyCode;
	return ss.str();
}

KeyTypedEvent::KeyTypedEvent(const KeyCharType keycode, const bool isMutable):
	I_KeyEvent(keycode, isMutable) {}

std::string KeyTypedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyTypedEvent: " << _keyCode;
	return ss.str();
}

}
