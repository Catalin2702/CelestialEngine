//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#ifndef CE_EVENTS_MOUSEEVENT_HPP
#define CE_EVENTS_MOUSEEVENT_HPP

#include "Define/DynamicLinker.hpp"
#include "Events/I_Event.hpp"

#include <string>


namespace CE::Events {

class CE_API MouseMovedEvent final : public I_Event {
public:
	MouseMovedEvent(float x, float y);

	[[nodiscard]] float GetX() const {return _x; }
	[[nodiscard]] float GetY() const {return _y; }
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseMoved)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
	float _x, _y;
};


class CE_API MouseScrolledEvent final : public I_Event {
public:
	MouseScrolledEvent(float xOffset, float yOffset);

	[[nodiscard]] float GetXOffset() const { return _xOffset; }
	[[nodiscard]] float GetYOffset() const { return _yOffset; }
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseScrolled)
	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

private:
	float _xOffset, _yOffset;
};


class CE_API I_MouseButtonEvent : public I_Event {
public:
	[[nodiscard]] int GetMouseButton() const { return _button; }

	EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput)

protected:
	I_MouseButtonEvent(int button);

protected:
	int _button;
};


class CE_API MouseButtonPressedEvent final : public I_MouseButtonEvent {
public:
	MouseButtonPressedEvent(int button);

	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseButtonPressed)
};


class CE_API MouseButtonReleasedEvent final : public I_MouseButtonEvent {
public:
	MouseButtonReleasedEvent(int button);

	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(MouseButtonReleased)
};


}

#endif //CE_EVENTS_MOUSEEVENT_HPP
