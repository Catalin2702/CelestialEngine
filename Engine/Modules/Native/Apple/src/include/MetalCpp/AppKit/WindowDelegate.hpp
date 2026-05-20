//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/Window
// File: WindowDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-22
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATE_HPP
#define CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATE_HPP

#include "MetalCpp/AppKit/AppKitPrivate.hpp"
#include "MetalCpp/Foundation/NSObject.hpp"
#include "Types/EventHandlers/I_WindowDelegateEventHandler.hpp"

namespace NS {

class WindowDelegate: public Referencing<WindowDelegate> {
public:
	static WindowDelegate* alloc();
	[[nodiscard]] WindowDelegate* init() const;
	void SetEventHandler(I_WindowDelegateEventHandler* handler) const;
};

_NS_INLINE WindowDelegate* WindowDelegate::alloc() {
	return sendMessage< WindowDelegate* >( _APPKIT_PRIVATE_CLS(WindowDelegate), _APPKIT_PRIVATE_SEL( alloc ) );
}

_NS_INLINE WindowDelegate* WindowDelegate::init() const {
	return sendMessage< WindowDelegate* >( this, _APPKIT_PRIVATE_SEL( init ) );
}

_NS_INLINE void WindowDelegate::SetEventHandler(I_WindowDelegateEventHandler* handler) const {
	return sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setEventHandler_ ), handler);
}

}

#endif //CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATE_HPP
