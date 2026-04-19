//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/CoreAnimation/DisplayLink
// File: DisplayLink.h
// Created by: Catalin Chirosca
// Created: 2026-04-19
// Updated by: Catalin Chirosca
// Updated: 2026-04-19
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_H
#define CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_H

#import <Foundation/Foundation.h>

typedef void (*DisplayLinkCallbackFn)(void* userData);

@interface DisplayLink: NSObject
	@property (nonatomic, assign) void* userData;
	@property (nonatomic, assign) DisplayLinkCallbackFn callback;

	- (instancetype)init;
	- (void)start;
	- (void)stop;
	- (void)setCallback:(DisplayLinkCallbackFn)callback userData:(void*)userData;

@end

// C wrapper functions for C++ interop
#ifdef __cplusplus
extern "C" {
#endif

void* CE_DisplayLink_Create(void);
void CE_DisplayLink_Destroy(void* displayLink);
void CE_DisplayLink_SetCallback(void* displayLink, DisplayLinkCallbackFn callback, void* userData);
void CE_DisplayLink_Start(void* displayLink);
void CE_DisplayLink_Stop(void* displayLink);

#ifdef __cplusplus
}
#endif

#endif //CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_H
