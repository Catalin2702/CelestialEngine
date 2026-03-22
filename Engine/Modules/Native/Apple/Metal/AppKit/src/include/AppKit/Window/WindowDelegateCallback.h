//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/Window
// File: WindowDelegateCallback.h
// Created by: Catalin Chirosca
// Created: 2026-03-22
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATECALLBACK_H
#define CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATECALLBACK_H


struct WindowDelegateCallbacks {
	void (*WindowWillCloseEventCallback)(void* userData);

	void (*WindowWillEnterFullScreenEventCallback)(void* userData);
	void (*WindowDidEnterFullScreenEventCallback)(void* userData, unsigned int width, unsigned int height);
	void (*WindowWillExitFullScreenEventCallback)(void* userData);
	void (*WindowDidExitFullScreenEventCallback)(void* userData, unsigned int width, unsigned int height);
	void (*WindowDidResizeEventCallback)(void* userData, unsigned int width, unsigned int height);

	void (*WindowWillMiniaturizeEventCallback)(void* userData);
	void (*WindowDidMiniaturizeEventCallback)(void* userData);
	void (*WindowDidDeminiaturizeEventCallback)(void* userData, unsigned int width, unsigned int height);

	void (*WindowDidMoveEventCallback)(void* userData, unsigned int x, unsigned int y);

	void (*WindowDidBecomeKeyEventCallback)(void* userData);
	void (*WindowDidResignKeyEventCallback)(void* userData);
};


#endif //CE_NATIVE_APPLE_METAL_APPKIT_WINDOW_WINDOWDELEGATECALLBACK_H
