//
// Module: CelestialEngine/Engine/Modules/Render/View/Platforms/Mac/Bridge
// File: ViewBridge.h
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#pragma once

#ifndef CE_RENDER_VIEW_BRIDGE_VIEWBRIDGE_H
#define CE_RENDER_VIEW_BRIDGE_VIEWBRIDGE_H

struct CocoaViewImpl;

#ifdef __OBJC__
@class NSEvent;
#else
class NSEvent;
#endif

typedef void* CocoaViewPtr;
typedef void (*CocoaViewEventCallback)(void* owner, void* event);


void HandleMouseDown(CocoaViewImpl* impl, NSEvent* event);

void HandleMouseUp(CocoaViewImpl* impl, NSEvent* event);

void HandleMouseMoved(CocoaViewImpl* impl, NSEvent* event);

void HandleMouseDragged(CocoaViewImpl* impl, NSEvent* event);

void HandleWheelScrolled(CocoaViewImpl* impl, NSEvent* event);

void HandleKeyDown(CocoaViewImpl* impl, NSEvent* event);

void HandleKeyUp(CocoaViewImpl* impl, NSEvent* event);


namespace CE::Apple::Bridge {


}

#endif //CE_RENDER_VIEW_BRIDGE_VIEWBRIDGE_H
