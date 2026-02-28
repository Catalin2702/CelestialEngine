//
// Module: MetalBridge/Cocoa
// File: MetalCocoaBridge.h
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-19
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_APPLE_METAL_METALBRIDGE_METALCOCOABRIDGE_H
#define CE_APPLE_METAL_METALBRIDGE_METALCOCOABRIDGE_H


namespace CE::Bridge {

// Get Cocoa window's content view
void* GetCocoaContentView(void* cocoaWindow);

// Set Metal layer as content layer for GLFW
void SetCocoaViewLayer(void* cocoaView, void* metalLayer);

// Set the content view of a Cocoa window
void SetCocoaWindowContentView(void* cocoaWindow, void* cocoaView);

// Add a subview to the content view
void AddSubviewToContentView(void* cocoaWindow, void* subview);

// Get the refresh rate of the display containing the window
int GetDisplayRefreshRate(void* cocoaWindow);

}

#endif //CE_APPLE_METAL_METALBRIDGE_METALCOCOABRIDGE_H
