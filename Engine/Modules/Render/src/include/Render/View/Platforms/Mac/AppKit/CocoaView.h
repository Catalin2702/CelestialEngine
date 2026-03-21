//
// Module: CelestialEngine/Engine/Modules/Render/View/Platforms/Mac/AppKit
// File: CocoaView.h
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#pragma once

#ifndef CE_RENDER_VIEW_MAC_APPKIT_COCOAVIEW_H
#define CE_RENDER_VIEW_MAC_APPKIT_COCOAVIEW_H

#import <AppKit/AppKit.h>

#include "CocoaView.h"

#ifdef __cplusplus
struct CocoaViewImpl;
#endif

@interface CocoaView: NSView
	- (void)setImpl:(void*)impl;
@end

#endif //CE_RENDER_VIEW_MAC_APPKIT_COCOAVIEW_H

