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
	windowDidResizeDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidMove(Notification* notification) {
	windowDidMoveDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidBecomeKey(Notification* notification) {
	windowDidBecomeKeyDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidResignKey(Notification* notification) {
	windowDidResignKeyDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidMiniaturize(Notification* notification) {
	windowDidMiniaturizeDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidDeminiaturize(Notification* notification) {
	windowDidDeminiaturizeDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidEnterFullScreen(Notification* notification) {
	windowDidEnterFullScreenDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidExitFullScreen(Notification* notification) {
	windowDidExitFullScreenDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowWillClose(Notification* notification) {
	windowWillCloseDispatcher.Dispatch(notification);
}

}
