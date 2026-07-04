//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: WindowDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#include "Apple/MetalCpp/AppKit/WindowEventDispatcher.hpp"

namespace NS {

void WindowEventDispatcher::DispatchWindowDidResize(Notification* notification) {
	windowDidResizeMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidMove(Notification* notification) {
	windowDidMoveMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidBecomeKey(Notification* notification) {
	windowDidBecomeKeyMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidResignKey(Notification* notification) {
	windowDidResignKeyMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidMiniaturize(Notification* notification) {
	windowDidMiniaturizeMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidDeminiaturize(Notification* notification) {
	windowDidDeminiaturizeMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidEnterFullScreen(Notification* notification) {
	windowDidEnterFullScreenMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidExitFullScreen(Notification* notification) {
	windowDidExitFullScreenMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowWillClose(Notification* notification) {
	windowWillCloseMulticastDispatcher.Dispatch(notification);
}

}
