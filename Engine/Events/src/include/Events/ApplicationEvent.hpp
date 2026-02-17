//
// Created by Catalin Chirosca on 2026-02-16.
//

#pragma once

#ifndef CE_EVENTS_APPLICATIONEVENT_HPP
#define CE_EVENTS_APPLICATIONEVENT_HPP

#include "Define/DynamicLinker.hpp"
#include "Events/Event.hpp"

#include <string>


namespace CE::Events {

class CE_API WindowResizeEvent : public Event {
public:
	WindowResizeEvent(unsigned int width, unsigned int height);

public:
	[[nodiscard]] inline unsigned int GetWidth() const { return _width;}
	[[nodiscard]] inline unsigned int GetHeight() const { return _height; }
	[[nodiscard]] std::string ToString() const override;

	EVENT_CLASS_TYPE(WindowResize)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)

private:
	unsigned int _width, _height;
};


class CE_API WindowCloseEvent : public Event {
public:
	WindowCloseEvent() = default;

public:
	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};


class CE_API AppTickEvent : public Event {
public:
	AppTickEvent() = default;

public:
	EVENT_CLASS_TYPE(AppTick)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};


class CE_API AppUpdateEvent : public Event {
public:
	AppUpdateEvent() = default;

public:
	EVENT_CLASS_TYPE(AppUpdate)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};


class CE_API AppRenderEvent : public Event {
public:
	AppRenderEvent() = default;

public:
	EVENT_CLASS_TYPE(AppRender)
	EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

}

#endif //CE_EVENTS_APPLICATIONEVENT_HPP