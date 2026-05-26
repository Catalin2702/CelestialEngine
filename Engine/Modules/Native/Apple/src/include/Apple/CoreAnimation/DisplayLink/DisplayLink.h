//
// Module: CelestialEngine/Engine/Modules/Native/Apple/CoreAnimation/DisplayLink
// File: DisplayLink.h
// Created by: Catalin Chirosca
// Created: 2026-04-19
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_H
#define CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_H

#include "Apple/Types/EventHandlers/I_DisplayLinkEventHandler.hpp"

#import <Foundation/Foundation.h>

@interface DisplayLink: NSObject
	- (instancetype)init;
	- (void)start;
	- (void)stop;
	- (void)setEventHandler:(I_DisplayLinkEventHandler*)handler;
@end

#endif //CE_NATIVE_APPLE_METAL_COREANIMATION_DISPLAYLINK_DISPLAYLINK_H
