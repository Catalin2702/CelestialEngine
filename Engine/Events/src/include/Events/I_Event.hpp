//
// Module: Events
// File: I_Event.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_EVENTS_EVENT_HPP
#define CE_EVENTS_EVENT_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Event.hpp"
#include "Define/Type.hpp"

#include <functional>
#include <string>


namespace CE::Events {

class I_Event;
class EventDispatcher;

enum class EventType: uint8_t {
	None = 0,

	WindowClose,
	WindowResize,
	WindowFocus,
	WindowLostFocus,
	WindowMoved,

	AppTick,
	AppUpdate,
	AppRender,

	KeyPressed,
	KeyReleased,
	KeyTyped,

	MouseButtonPressed,
	MouseButtonReleased,
	MouseMoved,
	MouseScrolled,
};


enum EventCategory {
	None = 0,
	EventCategoryApplication = BIT(0),
	EventCategoryInput = BIT(1),
	EventCategoryKeyboard = BIT(2),
	EventCategoryMouse = BIT(3),
	EventCategoryMouseButton = BIT(4),
};


class CE_API I_Event {
	friend class EventDispatcher;

public:
	virtual ~I_Event() = default;

public:
	[[nodiscard]] virtual EventType GetEventType() const = 0;
	[[nodiscard]] virtual const char* GetName() const = 0;
	[[nodiscard]] virtual int GetCategoryFlags() const = 0;
	[[nodiscard]] virtual std::string ToString() const { return GetName(); }

public:
	[[nodiscard]] bool IsInCategory(const EventCategory category) const { return GetCategoryFlags() & category; }
	[[nodiscard]] bool IsHandled() const { return _handled; }

protected:
	bool _handled = false;
};


class EventDispatcher {
	template<typename T>
	using EventFn = std::function<bool(T&)>;

public:
	EventDispatcher(I_Event& event);

public:
	template<typename T>
	bool Dispatch(EventFn<T> func);

private:
	I_Event& _event;
};

inline std::ostream & operator<<(std::ostream &os, const I_Event &event) {
	return os << event.ToString();
}

inline std::string format_as(const I_Event &event) {
	return event.ToString();
}

}

#include "../template/Events/I_Event.tpp"

#endif //CE_EVENTS_EVENT_HPP
