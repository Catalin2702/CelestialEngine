//
// Created by Catalin Chirosca on 2026-02-17.
//

#pragma once

#ifndef CE_DEFINE_EVENT_HPP
#define CE_DEFINE_EVENT_HPP


#define EVENT_CLASS_TYPE(type)\
	static CE::Events::EventType GetStaticType() { return CE::Events::EventType::type; }\
	virtual CE::Events::EventType GetEventType() const override { return GetStaticType(); }\
	inline virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)\
	virtual int GetCategoryFlags() const override {return category; }

#endif //CE_DEFINE_EVENT_HPP
