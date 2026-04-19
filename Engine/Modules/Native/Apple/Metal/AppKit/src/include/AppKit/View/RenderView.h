//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/View
// File: RenderView.h
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-04-19
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_H
#define CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_H

#import "RenderViewCallback.h"

#import <AppKit/AppKit.h>


@interface RenderView: NSView

	@property (nonatomic, assign) RenderViewCallbacks callbacks;
	@property (nonatomic, assign) void* userData;

	- (void)setCallbacks:(RenderViewCallbacks)callbacks userData:(void*)userData;

	- (void*)getUserData;

@end

#endif //CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_H
