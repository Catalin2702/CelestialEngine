//
// Module: MetalBridge/Cocoa
// File: CocoaBridge.h
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-19
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_APPLE_METAL_METALBRIDGE_COCOABRIDGE_H
#define CE_APPLE_METAL_METALBRIDGE_COCOABRIDGE_H

/**
 * @namespace CE::Bridge
 * @brief Bridge functions for integrating Metal with Cocoa windows
 * @details Provides C++ functions to interact with Objective-C/Cocoa APIs for setting up
 *			Metal rendering in GLFW windows on macOS. These functions handle the conversion
 *			between C++ and Objective-C objects.
 */
namespace CE::Bridge {

/**
 * @brief Gets the content view of a Cocoa window
 * @param cocoaWindow Pointer to NSWindow (as void*)
 * @return void* Pointer to the content NSView
 * @details Retrieves the main content view of a Cocoa window where rendering occurs
 */
void* GetCocoaContentView(void* cocoaWindow);

/**
 * @brief Sets a Metal layer as the backing layer for a Cocoa view
 * @param cocoaView Pointer to NSView (as void*)
 * @param metalLayer Pointer to CAMetalLayer (as void*)
 * @details Configures the view to use the Metal layer for rendering, replacing
 *			the default Core Animation layer
 */
void SetCocoaViewLayer(void* cocoaView, void* metalLayer);

/**
 * @brief Sets the content view of a Cocoa window
 * @param cocoaWindow Pointer to NSWindow (as void*)
 * @param cocoaView Pointer to NSView to set as content view (as void*)
 * @details Replaces the window's content view with the specified view
 */
void SetCocoaWindowContentView(void* cocoaWindow, void* cocoaView);

/**
 * @brief Adds a subview to the window's content view
 * @param cocoaWindow Pointer to NSWindow (as void*)
 * @param subview Pointer to NSView to add as subview (as void*)
 * @details Adds a subview to the content view's view hierarchy
 */
void AddSubviewToContentView(void* cocoaWindow, void* subview);

/**
 * @brief Gets the refresh rate of the display containing the window
 * @param cocoaWindow Pointer to NSWindow (as void*)
 * @return int Refresh rate in Hz (e.g., 60, 120)
 * @details Queries the display's nominal refresh rate, useful for VSync settings
 */
int GetDisplayRefreshRate(void* cocoaWindow);

}

#endif //CE_APPLE_METAL_METALBRIDGE_COCOABRIDGE_H
