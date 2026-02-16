//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#ifndef WORKSPACE_MOUSEEVENT_HPP
#define WORKSPACE_MOUSEEVENT_HPP

#include "Events/Event.hpp"

#include <string>


namespace CE::Events {

class CE_API MouseMovedEvent : public Event {
public:
	MouseMovedEvent(float x, float y);

	[[nodiscard]] inline float GetX() const {return _x; }
	[[nodiscard]] inline float GetY() const {return _y; }
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
	float _x, _y;
};


class CE_API MouseScrolledEvent : public Event {
public:
	MouseScrolledEvent(float xOffset, float yOffset);

	[[nodiscard]] inline float GetXOffset() const { return _xOffset; }
	[[nodiscard]] inline float GetYOffset() const { return _yOffset; }
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
	float _xOffset, _yOffset;
};


class CE_API MouseButtonEvent : public Event {
public:
	[[nodiscard]] inline int GetMouseButton() const { return _button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)

protected:
	MouseButtonEvent(int button);

protected:
	int _button;
};


class CE_API MouseButtonPressedEvent : public MouseButtonEvent {
public:
	MouseButtonPressedEvent(int button);

	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseButtonPressed)
};


class CE_API MouseButtonReleasedEvent : public MouseButtonEvent {
public:
	MouseButtonReleasedEvent(int button);

	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseButtonReleased)
};


}

#endif //WORKSPACE_MOUSEEVENT_HPP