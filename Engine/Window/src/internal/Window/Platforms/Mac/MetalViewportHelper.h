//
// Created by Catalin Chirosca on 2026-02-19.
//

#pragma once

#ifndef CE_WINDOW_MAC_METALVIEWPORTHELPER_H
#define CE_WINDOW_MAC_METALVIEWPORTHELPER_H

#ifdef __cplusplus
extern "C" {
#endif

// Get Cocoa window's content view
void* GetCocoaContentView(void* cocoaWindow);

// Set Metal layer as content layer for GLFW
void SetCocoaViewLayer(void* cocoaView, void* metalLayer);

// Set the content view of a Cocoa window
void SetCocoaWindowContentView(void* cocoaWindow, void* cocoaView);

// Get the refresh rate of the display containing the window
int GetDisplayRefreshRate(void* cocoaWindow);

#ifdef __cplusplus
}
#endif

#endif //CE_WINDOW_MAC_METALVIEWPORTHELPER_H
