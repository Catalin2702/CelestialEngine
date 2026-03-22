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

#include <AppKit/AppKitPrivate.hpp>
#include <Foundation/NSObject.hpp>

#include "AppKit/Window/WindowDelegateCallback.h"

namespace NS {

class WindowDelegate: public Referencing<WindowDelegate> {
public:
	static WindowDelegate* alloc();
	[[nodiscard]] WindowDelegate* init() const;
	void setCallbacks(const WindowDelegateCallbacks& callbacks, void* userData) const;
	[[nodiscard]] void* getUserData() const;
};

_NS_INLINE WindowDelegate* NS::WindowDelegate::alloc() {
	return sendMessage< WindowDelegate* >( objc_getClass("WindowDelegate"), _APPKIT_PRIVATE_SEL( alloc ) );
}

_NS_INLINE WindowDelegate* NS::WindowDelegate::init() const {
	return sendMessage< WindowDelegate* >( this, _APPKIT_PRIVATE_SEL( init ) );
}

_NS_INLINE void NS::WindowDelegate::setCallbacks(const WindowDelegateCallbacks& callbacks, void* userData) const {
	return sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setCallbacks_userData_ ), callbacks, userData );
}

_NS_INLINE void* NS::WindowDelegate::getUserData() const {
	return sendMessage< void* >( this, _APPKIT_PRIVATE_SEL( getUserData_ ) );
}

}

#endif //CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATE_HPP
