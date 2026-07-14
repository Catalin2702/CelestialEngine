//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/AppKit
// File: NsWindowEventDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Apple/MetalCpp/AppKit/NsWindowEventDispatcher.hpp"

namespace CE::Native {

void NsWindowEventDispatcher::DispatchWindowDidResize(NS::Notification* notification) {
	nsWindowGeometryEvents.didResizeDispatcher.Dispatch(notification);
}

void NsWindowEventDispatcher::DispatchWindowDidMove(NS::Notification* notification) {
	nsWindowGeometryEvents.didMoveDispatcher.Dispatch(notification);
}

void NsWindowEventDispatcher::DispatchWindowDidBecomeKey(NS::Notification* notification) {
	nsWindowFocusEvents.didBecomeKeyDispatcher.Dispatch(notification);
}

void NsWindowEventDispatcher::DispatchWindowDidResignKey(NS::Notification* notification) {
	nsWindowFocusEvents.didResignKeyDispatcher.Dispatch(notification);
}

void NsWindowEventDispatcher::DispatchWindowDidMiniaturize(NS::Notification* notification) {
	nsWindowStateEvents.didMiniaturizeDispatcher.Dispatch(notification);
}

void NsWindowEventDispatcher::DispatchWindowDidDeminiaturize(NS::Notification* notification) {
	nsWindowStateEvents.didDeminiaturizeDispatcher.Dispatch(notification);
}

void NsWindowEventDispatcher::DispatchWindowDidEnterFullScreen(NS::Notification* notification) {
	nsWindowStateEvents.didEnterFullScreenDispatcher.Dispatch(notification);
}

void NsWindowEventDispatcher::DispatchWindowDidExitFullScreen(NS::Notification* notification) {
	nsWindowStateEvents.didExitFullScreenDispatcher.Dispatch(notification);
}

void NsWindowEventDispatcher::DispatchWindowWillClose(NS::Notification* notification) {
	nsWindowLifecycleEvents.willCloseDispatcher.Dispatch(notification);
}

}
