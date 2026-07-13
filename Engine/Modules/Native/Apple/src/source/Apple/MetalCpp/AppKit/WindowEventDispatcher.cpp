//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: WindowDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Apple/MetalCpp/AppKit/WindowEventDispatcher.hpp"

namespace CE::Native {

void WindowEventDispatcher::DispatchWindowDidResize(NS::Notification* notification) {
	windowDidResizeMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidMove(NS::Notification* notification) {
	windowDidMoveMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidBecomeKey(NS::Notification* notification) {
	windowDidBecomeKeyMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidResignKey(NS::Notification* notification) {
	windowDidResignKeyMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidMiniaturize(NS::Notification* notification) {
	windowDidMiniaturizeMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidDeminiaturize(NS::Notification* notification) {
	windowDidDeminiaturizeMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidEnterFullScreen(NS::Notification* notification) {
	windowDidEnterFullScreenMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidExitFullScreen(NS::Notification* notification) {
	windowDidExitFullScreenMulticastDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowWillClose(NS::Notification* notification) {
	windowWillCloseMulticastDispatcher.Dispatch(notification);
}

}
