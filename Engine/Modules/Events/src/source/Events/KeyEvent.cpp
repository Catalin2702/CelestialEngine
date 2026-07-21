//
// Module: CelestialEngine/Engine/Modules/Events
// File: KeyEvent.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#include "Events/KeyEvent.hpp"

#include <format>

namespace CE::Events {

KeyPressedEvent::KeyPressedEvent(const KeyType keycode, const int repeatCount, const bool isMutable):
	I_KeyEvent(keycode, isMutable), _repeatCount(repeatCount) {}

std::string KeyPressedEvent::ToString() const {
	return std::format("{0}: {1} ({2} repeats)", GetName(), _keyCode, _repeatCount);
}

KeyReleasedEvent::KeyReleasedEvent(const KeyType keycode, const bool isMutable):
	I_KeyEvent(keycode, isMutable) {}

std::string KeyReleasedEvent::ToString() const {
	return std::format("{0}: {1}", GetName(), _keyCode);
}

KeyTypedEvent::KeyTypedEvent(const KeyCharType keycode, const bool isMutable):
	I_KeyEvent(keycode, isMutable) {}

std::string KeyTypedEvent::ToString() const {
	return std::format("{0}: {1}", GetName(), _keyCode);
}

}
