//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Events/KeyEvent.hpp"

#include <sstream>


namespace CE::Events {

KeyPressedEvent::KeyPressedEvent(const int keycode, const int repeatCount):
	I_KeyEvent(keycode), _repeatCount(repeatCount) {}

std::string KeyPressedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
	return ss.str();
}

KeyReleasedEvent::KeyReleasedEvent(const int keycode): I_KeyEvent(keycode) {}

std::string KeyReleasedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyReleasedEvent: " << _keyCode;
	return ss.str();
}

KeyTypedEvent::KeyTypedEvent(const unsigned int keycode): I_KeyEvent(keycode) {}

std::string KeyTypedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyTypedEvent: " << _keyCode;
	return ss.str();
}

}
