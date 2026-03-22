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

#include "WindowDelegateCallback.h"

#import <AppKit/AppKit.h>
#import <Cocoa/Cocoa.h>

@interface WindowDelegate: NSObject<NSWindowDelegate>

@property (nonatomic, assign) WindowDelegateCallbacks callbacks;
@property (nonatomic, assign) void* userData;

- (void)setCallbacks:(WindowDelegateCallbacks)callbacks userData:(void*)userData;

- (void*)getUserData;

@end


#endif //CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATE_H
