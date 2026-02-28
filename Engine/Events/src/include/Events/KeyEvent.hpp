//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#ifndef CE_EVENTS_KEYEVENT_HPP
#define CE_EVENTS_KEYEVENT_HPP

#include "Define/DynamicLinker.hpp"
#include "Events/I_Event.hpp"

#include <string>


namespace CE::Events {

template<typename T>
class CE_API I_KeyEvent : public I_Event {
public:
	[[nodiscard]] T GetKeyCode() const { return _keyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
	I_KeyEvent(T keycode): _keyCode(keycode) {}

protected:
	T _keyCode;
};


class CE_API KeyPressedEvent : public I_KeyEvent<int> {
public:
	KeyPressedEvent(int keycode, int repeatCount);

public:
	[[nodiscard]] int GetRepeatCount() const { return _repeatCount; }
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(KeyPressed)
private:
	int _repeatCount;
};

class CE_API KeyReleasedEvent : public I_KeyEvent<int> {
public:
	KeyReleasedEvent(int keycode);

public:
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(KeyReleased)
};

class CE_API KeyTypedEvent : public I_KeyEvent<unsigned int> {
public:
	KeyTypedEvent(unsigned int keycode);

public:
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(KeyTyped)
};

}

#endif //CE_EVENTS_KEYEVENT_HPP
