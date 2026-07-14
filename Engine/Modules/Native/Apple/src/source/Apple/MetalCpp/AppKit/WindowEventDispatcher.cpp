//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: WindowDelegate.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Apple/MetalCpp/AppKit/WindowEventDispatcher.hpp"

namespace CE::Native {

void WindowEventDispatcher::DispatchWindowDidResize(NS::Notification* notification) {
	nsWindowGeometryEvents.didResizeDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidMove(NS::Notification* notification) {
	nsWindowGeometryEvents.didMoveDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidBecomeKey(NS::Notification* notification) {
	nsWindowFocusEvents.didBecomeKeyDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidResignKey(NS::Notification* notification) {
	nsWindowFocusEvents.didResignKeyDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidMiniaturize(NS::Notification* notification) {
	nsWindowStateEvents.didMiniaturizeDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidDeminiaturize(NS::Notification* notification) {
	nsWindowStateEvents.didDeminiaturizeDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidEnterFullScreen(NS::Notification* notification) {
	nsWindowStateEvents.didEnterFullScreenDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowDidExitFullScreen(NS::Notification* notification) {
	nsWindowStateEvents.didExitFullScreenDispatcher.Dispatch(notification);
}

void WindowEventDispatcher::DispatchWindowWillClose(NS::Notification* notification) {
	nsWindowLifecycleEvents.willCloseDispatcher.Dispatch(notification);
}

}
