//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/Window
// File: WindowDelegate.h
// Created by: Catalin Chirosca
// Created: 2026-03-22
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATE_H
#define CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATE_H

#include "Types/EventHandlers/I_WindowDelegateEventHandler.hpp"

#import <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>

@interface WindowDelegate: NSObject<NSWindowDelegate>

	- (void)setEventHandler:(I_WindowDelegateEventHandler*)handler;

@end


#endif //CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATE_H
