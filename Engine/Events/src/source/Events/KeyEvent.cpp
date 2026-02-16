//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Events/KeyEvent.hpp"

#include <sstream>

CE::Events::KeyEvent::KeyEvent(const int keycode): _keyCode(keycode) {}

CE::Events::KeyPressedEvent::KeyPressedEvent(const int keycode, const int repeatCount):
	KeyEvent(keycode), _repeatCount(repeatCount) {}

std::string CE::Events::KeyPressedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyPressedEvent: " << _keyCode << " (" << _repeatCount << " repeats)";
	return ss.str();
}

CE::Events::KeyReleasedEvent::KeyReleasedEvent(const int keycode): KeyEvent(keycode) {}

std::string CE::Events::KeyReleasedEvent::ToString() const {
	std::stringstream ss;
	ss << "KeyReleasedEvent: " << _keyCode;
	return ss.str();
}
