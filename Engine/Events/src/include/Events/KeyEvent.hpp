//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#ifndef CE_EVENTS_KEYEVENT_HPP
#define CE_EVENTS_KEYEVENT_HPP

#include "Define/DynamicLinker.hpp"
#include "Events/Event.hpp"

#include <string>


namespace CE::Events {

class CE_API KeyEvent : public Event {
public:
	[[nodiscard]] inline int GetKeyCode() const { return _keyCode; }

	EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
	KeyEvent(int keycode);

protected:
	int _keyCode;
};


class CE_API KeyPressedEvent : public KeyEvent {
public:
	KeyPressedEvent(int keycode, int repeatCount);

public:
	[[nodiscard]] inline int GetRepeatCount() const { return _repeatCount; }
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(KeyPressed)
private:
	int _repeatCount;
};

class CE_API KeyReleasedEvent : public KeyEvent {
public:
	KeyReleasedEvent(int keycode);

public:
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(KeyReleased)
};

}

#endif //CE_EVENTS_KEYEVENT_HPP