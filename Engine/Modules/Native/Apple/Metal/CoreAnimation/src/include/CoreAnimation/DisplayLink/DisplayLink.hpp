//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/CoreAnimation/DisplayLink
// File: DisplayLink.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-19
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_HPP
#define CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_HPP

#include <Foundation/Foundation.hpp>
#include <QuartzCore/QuartzCore.hpp>


namespace CA {

class DisplayLink: public NS::Referencing<DisplayLink> {
public:
	using TickFn = void(*)(void*);

	static DisplayLink* alloc();
	[[nodiscard]] DisplayLink* init() const;

	void setCallback(TickFn callback, void* userData) const;
	void start() const;
	void stop() const;
};

}

_NS_INLINE CA::DisplayLink* CA::DisplayLink::alloc() {
	return sendMessage<DisplayLink*>(objc_getClass("DisplayLink"), _CA_PRIVATE_SEL(alloc));
}

_NS_INLINE CA::DisplayLink* CA::DisplayLink::init() const {
	return sendMessage<DisplayLink*>(this, _CA_PRIVATE_SEL(init));
}

_NS_INLINE void CA::DisplayLink::setCallback(TickFn callback, void* userData) const {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setCallback_userData_), callback, userData);
}

_NS_INLINE void CA::DisplayLink::start() const {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(start));
}

_NS_INLINE void CA::DisplayLink::stop() const {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(stop));
}

#endif //CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_HPP
