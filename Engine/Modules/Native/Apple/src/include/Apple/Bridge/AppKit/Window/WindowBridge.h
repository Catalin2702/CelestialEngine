//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Bridge/AppKit/Window
// File: WindowBridge.h
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-03
//

#pragma once

#ifndef CE_NATIVE_APPLE_BRIDGE_APPKIT_WINDOW_WINDOWBRIDGE_H
#define CE_NATIVE_APPLE_BRIDGE_APPKIT_WINDOW_WINDOWBRIDGE_H

/**
 * @namespace CE::Apple::Bridge
 * @brief Bridge functions for integrating Metal with Cocoa windows
 * @details Provides C++ functions to interact with Objective-C/Cocoa APIs for setting up
 *			Metal rendering in GLFW windows on macOS. These functions handle the conversion
 *			between C++ and Objective-C objects.
 */
namespace CE::Apple::Bridge {
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

#endif //CE_NATIVE_APPLE_BRIDGE_APPKIT_WINDOW_WINDOWBRIDGE_H
