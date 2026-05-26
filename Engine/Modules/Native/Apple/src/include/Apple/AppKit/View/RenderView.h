//
// Module: CelestialEngine/Engine/Modules/Native/Apple/AppKit/View
// File: RenderView.h
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_H
#define CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_H

#include "Apple/Types/EventHandlers/I_ViewEventHandler.hpp"

#import <MetalKit/MetalKit.h>


@interface RenderView: MTKView

	- (void)setEventHandler:(I_ViewEventHandler*)handler;

@end

#endif //CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_H
