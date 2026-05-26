//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/Bridge/Cocoa
// File: WindowBridge.h
// Created by: Catalin Chirosca
// Created: 2026-02-19
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_BRIDGE_APPKIT_WINDOW_WINDOWBRIDGE_H
#define CE_NATIVE_APPLE_METAL_BRIDGE_APPKIT_WINDOW_WINDOWBRIDGE_H

/**
 * @namespace CE::Apple::Bridge
 * @brief Bridge functions for integrating Metal with Cocoa windows
 * @details Provides C++ functions to interact with Objective-C/Cocoa APIs for setting up
 *			Metal rendering in GLFW windows on macOS. These functions handle the conversion
 *			between C++ and Objective-C objects.
 */
namespace CE::Apple::Bridge {

/**
 * @brief Callback function signature for window events
 * @param userData User data pointer passed when creating the delegate
 * @param eventType Type of event (0=resize, 1=close, 2=focus, 3=unfocus, 4=minimize, 5=restore)
 * @param width Window width (for resize events)
 * @param height Window height (for resize events)
 */
typedef void (*WindowEventCallback)(void* userData, int eventType, unsigned int width, unsigned int height);

/**
 * @brief Callback function signature for NSEvent processing
 * @param userData User data pointer
 * @param event Opaque pointer to NSEvent
 */
typedef void (*EventProcessCallback)(void* userData, void* event);

/**
 * @brief Creates a Cocoa window delegate for handling window events
 * @param callback Function to call when window events occur
 * @param userData User data to pass to the callback
 * @return void* Opaque pointer to the delegate object
 * @details Creates an NSWindowDelegate that intercepts window events and forwards them
 *          to the provided C++ callback function
 */
void* CreateCocoaWindowDelegate(WindowEventCallback callback, void* userData);

/**
 * @brief Destroys a Cocoa window delegate
 * @param delegate Opaque pointer to the delegate object created by CreateCocoaWindowDelegate
 * @details Releases the NSWindowDelegate object and cleans up associated resources
 */
void DestroyCocoaWindowDelegate(void* delegate);

/**
 * @brief Sets a delegate for a Cocoa window
 * @param cocoaWindow Pointer to NSWindow (as void*)
 * @param delegate Delegate object created by CreateCocoaWindowDelegate
 * @details Assigns the delegate to the window to receive window events
 */
void SetCocoaWindowDelegate(void* cocoaWindow, void* delegate);

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

/**
 * @brief Sets the autosave name for the window's frame
 * @param cocoaWindow Pointer to NSWindow (as void*)
 * @param name Name to use for autosaving the window frame (position and size)
 * @details Enables automatic saving and restoration of window position/size.
 *          The window will remember its position between application launches.
 */
void SetWindowFrameAutosaveName(void* cocoaWindow, const char* name);

/**
 * @brief Processes pending Cocoa events
 * @param processCallback Function to call for each event (optional, can be nullptr)
 * @param userData User data to pass to callback
 * @details Polls all pending NSEvents and processes them. If processCallback is provided,
 *          it will be called for each event before the event is sent to the application.
 */
void ProcessCocoaEvents(EventProcessCallback processCallback, void* userData);

/**
 * @brief Checks if a keyboard key is currently pressed
 * @param keyCode Virtual key code (macOS virtual key code)
 * @return bool True if the key is pressed, false otherwise
 * @details Uses CGEventSourceKeyState to check the current state of a keyboard key
 */
bool IsKeyPressed(unsigned short keyCode);

/**
 * @brief Checks if a mouse button is currently pressed
 * @param buttonNumber Mouse button number (0=left, 1=right, 2=middle, etc.)
 * @return bool True if the button is pressed, false otherwise
 * @details Uses NSEvent.pressedMouseButtons to check mouse button state
 */
bool IsMouseButtonPressed(int buttonNumber);

/**
 * @brief Gets the current mouse position in window coordinates
 * @param cocoaWindow Pointer to NSWindow (as void*)
 * @param outX Pointer to store X coordinate
 * @param outY Pointer to store Y coordinate (flipped to top-left origin)
 * @details Retrieves the mouse position relative to the window's content view
 */
void GetMousePosition(void* cocoaWindow, float* outX, float* outY);

}

#endif //CE_NATIVE_APPLE_METAL_BRIDGE_APPKIT_WINDOW_WINDOWBRIDGE_H
