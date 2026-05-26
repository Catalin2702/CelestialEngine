//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/QuartzCore
// File: DisplayLink.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-19
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_HPP
#define CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_HPP

#include "Apple/MetalCpp/Foundation/Foundation.hpp"
#include "Apple/MetalCpp/QuartzCore/CAPrivate.hpp"
#include "Apple/Types/EventHandlers/I_DisplayLinkEventHandler.hpp"

namespace CA {

class DisplayLink: public NS::Referencing<DisplayLink> {
public:
	using TickFn = void(*)(void*);

	static DisplayLink* alloc();
	[[nodiscard]] DisplayLink* init() const;

	void SetEventHandler(I_DisplayLinkEventHandler* handler) const;
	void start() const;
	void stop() const;
};

}

_NS_INLINE CA::DisplayLink* CA::DisplayLink::alloc() {
	return sendMessage<DisplayLink*>(_CA_PRIVATE_CLS(DisplayLink), _CA_PRIVATE_SEL(alloc));
}

_NS_INLINE CA::DisplayLink* CA::DisplayLink::init() const {
	return sendMessage<DisplayLink*>(this, _CA_PRIVATE_SEL(init));
}

_NS_INLINE void CA::DisplayLink::SetEventHandler(I_DisplayLinkEventHandler* handler) const {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(setEventHandler_), handler);
}

_NS_INLINE void CA::DisplayLink::start() const {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(start));
}

_NS_INLINE void CA::DisplayLink::stop() const {
	return sendMessage<void>(this, _CA_PRIVATE_SEL(stop));
}

#endif //CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_HPP
