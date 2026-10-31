//
// Module: CelestialEngine/Engine/Modules/Core/Input
// File: InputState.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-22
// Updated by: Catalin Chirosca
// Updated: 2026-10-31
//

#include "Core/Input/InputState.hpp"

#include <utility>


namespace CE::Core {

// Map a key/button code to its bitset index; returns false for Unknown or out-of-range codes so
// bogus platform codes can never touch (or read) the state.
static bool KeyIndex(const Types::KeyboardKeyCode keyCode, std::size_t& outIndex) {
	if (keyCode == Types::KeyboardKeyCode::Unknown) [[unlikely]]
		return false;

	const auto index = static_cast<std::size_t>(Types::ToUInt(keyCode));
	if (index >= InputState::stMaxKeys) [[unlikely]]
		return false;

	outIndex = index;
	return true;
}

static bool ButtonIndex(const Types::MouseButtonCode buttonCode, std::size_t& outIndex) {
	if (buttonCode == Types::MouseButtonCode::Unknown) [[unlikely]]
		return false;

	const auto index = static_cast<std::size_t>(Types::ToUInt(buttonCode));
	if (index >= InputState::stMaxMouseButtons) [[unlikely]]
		return false;

	outIndex = index;
	return true;
}

bool InputState::IsKeyDown(const Types::KeyboardKeyCode keyCode) const {
	std::size_t index = 0;
	return KeyIndex(keyCode, index) and _keysDown[index];
}

bool InputState::IsKeyJustPressed(const Types::KeyboardKeyCode keyCode) const {
	std::size_t index = 0;
	return KeyIndex(keyCode, index) and _keysJustPressed[index];
}

bool InputState::IsKeyJustReleased(const Types::KeyboardKeyCode keyCode) const {
	std::size_t index = 0;
	return KeyIndex(keyCode, index) and _keysJustReleased[index];
}

bool InputState::IsMouseButtonDown(const Types::MouseButtonCode buttonCode) const {
	std::size_t index = 0;
	return ButtonIndex(buttonCode, index) and _buttonsDown[index];
}

bool InputState::IsMouseButtonJustPressed(const Types::MouseButtonCode buttonCode) const {
	std::size_t index = 0;
	return ButtonIndex(buttonCode, index) and _buttonsJustPressed[index];
}

bool InputState::IsMouseButtonJustReleased(const Types::MouseButtonCode buttonCode) const {
	std::size_t index = 0;
	return ButtonIndex(buttonCode, index) and _buttonsJustReleased[index];
}

void InputState::EndFrame() {
	_keysJustPressed.reset();
	_keysJustReleased.reset();
	_buttonsJustPressed.reset();
	_buttonsJustReleased.reset();
	_scrollDeltaX = 0.0f;
	_scrollDeltaY = 0.0f;

	// The position the next frame's delta is measured from.
	_previousMouseX = _mouseX;
	_previousMouseY = _mouseY;
}

void InputState::Reset() {
	// Report the force-released keys/buttons as just-released so subscribers still see the transition.
	_keysJustReleased |= _keysDown;
	_keysDown.reset();

	_buttonsJustReleased |= _buttonsDown;
	_buttonsDown.reset();

	// The cursor moves elsewhere while the window is unfocused: its first position back must not count as motion.
	_hasMousePosition = false;
}

void InputState::_OnKeyPressed(const Events::KeyPressedEvent& event) {
	std::size_t index = 0;
	if (not KeyIndex(event.GetKeyCode(), index))
		return;

	// OS key repeats re-fire KeyPressed while the key is held: they must not re-trigger "just pressed".
	if (event.GetRepeatCount() == 0 and not _keysDown[index])
		_keysJustPressed[index] = true;

	_keysDown[index] = true;
}

void InputState::_OnKeyReleased(const Events::KeyReleasedEvent& event) {
	std::size_t index = 0;
	if (not KeyIndex(event.GetKeyCode(), index))
		return;

	_keysDown[index] = false;
	_keysJustReleased[index] = true;
}

void InputState::_OnMouseMoved(const Events::MouseMovedEvent& event) {
	_SetMousePosition(event.GetX(), event.GetY());
}

void InputState::_OnMouseButtonPressed(const Events::MouseButtonPressedEvent& event) {
	std::size_t index = 0;
	if (not ButtonIndex(event.GetMouseButton(), index))
		return;

	if (not _buttonsDown[index])
		_buttonsJustPressed[index] = true;

	_buttonsDown[index] = true;
}

void InputState::_OnMouseButtonReleased(const Events::MouseButtonReleasedEvent& event) {
	std::size_t index = 0;
	if (not ButtonIndex(event.GetMouseButton(), index))
		return;

	_buttonsDown[index] = false;
	_buttonsJustReleased[index] = true;
}

void InputState::_OnMouseDragged(const Events::MouseDraggedEvent& event) {
	// Some backends deliver no move events while a button is held, so drags are the only position source during one.
	_SetMousePosition(event.GetX(), event.GetY());
}

void InputState::_OnMouseWheelScrolled(const Events::MouseWheelScrolledEvent& event) {
	_scrollDeltaX += event.GetXOffset();
	_scrollDeltaY += event.GetYOffset();
}

void InputState::_OnWindowFocus(const Events::WindowFocusEvent& event) {
	if (not event.IsFocused())
		Reset();
}

void InputState::_SetMousePosition(const f32 x, const f32 y) {
	_mouseX = x;
	_mouseY = y;

	if (not std::exchange(_hasMousePosition, true)) {
		_previousMouseX = x;
		_previousMouseY = y;
	}
}

}
