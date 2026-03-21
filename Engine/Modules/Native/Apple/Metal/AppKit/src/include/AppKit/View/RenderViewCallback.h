//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/View
// File: RenderViewCallback.h
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCALLBACK_H
#define CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCALLBACK_H

struct RenderViewCallbacks {
	void (*KeyPressedEventCallback)(void* userData, int keyCode, bool isRepeat);
	void (*KeyReleasedEventCallback)(void* userData, int keyCode);

	void (*MouseMovedEventCallback)(void* userData, float x, float y);
	void (*MouseScrolledEventCallback)(void* userData, float xOffset, float yOffset);
	void (*MouseButtonPressedEventCallback)(void* userData, int buttonCode);
	void (*MouseButtonReleasedEventCallback)(void* userData, int buttonCode);
	void (*MouseDraggedEventCallback)(void* userData, int buttonCode, float x, float y);
};

#endif //CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCALLBACK_H