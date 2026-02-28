//
// Module: Define
// File: Event.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-17
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_DEFINE_EVENT_HPP
#define CE_DEFINE_EVENT_HPP

/**
 * @def EVENT_CLASS_TYPE(type)
 * @brief Macro to implement event type methods for event classes
 * @param type The EventType enum value (e.g., WindowClose, KeyPressed)
 * @details Generates three methods:
 *          - GetStaticType(): Returns the static event type
 *          - GetEventType(): Virtual method returning the instance's event type
 *          - GetName(): Virtual method returning the event type name as a string
 *          This macro must be used in every concrete event class that inherits from I_Event.
 * @example
 *          class WindowCloseEvent: public I_Event {
 *              EVENT_CLASS_TYPE(WindowClose)
 *              // ...
 *          };
 */
#define EVENT_CLASS_TYPE(type)\
	static CE::Events::EventType GetStaticType() { return CE::Events::EventType::type; }\
	virtual CE::Events::EventType GetEventType() const override { return GetStaticType(); }\
	inline virtual const char* GetName() const override { return #type; }

/**
 * @def EVENT_CLASS_CATEGORY(category)
 * @brief Macro to implement event category method for event classes
 * @param category Bitwise OR combination of EventCategory flags
 * @details Generates the GetCategoryFlags() virtual method that returns the event's
 *          category flags. Categories are used to filter and route events.
 *          Multiple categories can be combined using bitwise OR operator.
 * @example
 *          class KeyPressedEvent: public I_Event {
 *              EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
 *              // ...
 *          };
 */
#define EVENT_CLASS_CATEGORY(category)\
	virtual int GetCategoryFlags() const override {return category; }

#endif //CE_DEFINE_EVENT_HPP
