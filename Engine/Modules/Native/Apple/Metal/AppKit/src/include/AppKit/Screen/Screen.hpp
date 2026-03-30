//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/Screen
// File: Screen.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-30
// Updated by: Catalin Chirosca
// Updated: 2026-03-30
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_SCREEN_SCREEN_HPP
#define CE_NATIVE_APPLE_METAL_APPKIT_SCREEN_SCREEN_HPP

#include "AppKit/AppKitPrivate.hpp"

#include <Foundation/Foundation.hpp>


namespace NS {

class Screen: public Referencing<Screen> {
public:
	static Array* screens();
	static Screen* mainScreen();

public:
	[[nodiscard]] CGFloat backingScaleFactor() const;
	[[nodiscard]] CGRect frame() const;
	[[nodiscard]] int maximumFramesPerSecond() const;
	[[nodiscard]] CGRect visibleFrame() const;
};

_NS_INLINE Array* Screen::screens() {
	return sendMessage< Array* >( objc_getClass("NSScreen"), _APPKIT_PRIVATE_SEL( screens ) );
}

_NS_INLINE Screen* Screen::mainScreen() {
	return sendMessage< Screen* >( objc_getClass("NSScreen"), _APPKIT_PRIVATE_SEL( mainScreen ) );
}

_NS_INLINE CGFloat Screen::backingScaleFactor() const {
	return sendMessage< CGFloat >( this, _APPKIT_PRIVATE_SEL( backingScaleFactor ) );
}

_NS_INLINE CGRect Screen::frame() const {
	return sendMessage< CGRect >( this, _APPKIT_PRIVATE_SEL( frame ) );
}

_NS_INLINE int Screen::maximumFramesPerSecond() const {
	return sendMessage< int >( this, _APPKIT_PRIVATE_SEL( maximumFramesPerSecond ) );
}

_NS_INLINE CGRect Screen::visibleFrame() const {
	return sendMessage< CGRect >( this, _APPKIT_PRIVATE_SEL( visibleFrame ) );
}

}


#endif // CE_NATIVE_APPLE_METAL_APPKIT_SCREEN_SCREEN_HPP
