//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: WindowDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-22
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_WINDOWEVENTDISPATCHER_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_WINDOWEVENTDISPATCHER_HPP

#include <AppKit/AppKit.hpp>

#include "Utility/Delegate/Dispatcher.hpp"

using NSNotificationDelegate = EventDelegate<const NS::Notification*>;
using NSNotificationMulticastDispatcher = CE::Utility::MulticastDispatcher<const NS::Notification*>;

namespace CE::Native {

class WindowEventDispatcher: public NS::I_WindowEventDispatcher {
public:
	struct NSWindowGeometryEvents {
		UnicastEventDispatcher<const NS::Notification*> didResizeDispatcher;
		UnicastEventDispatcher<const NS::Notification*> didMoveDispatcher;
	};

	struct NSWindowFocusEvents {
		UnicastEventDispatcher<const NS::Notification*> didBecomeKeyDispatcher;
		UnicastEventDispatcher<const NS::Notification*> didResignKeyDispatcher;
	};

	struct NSWindowStateEvents {
		UnicastEventDispatcher<const NS::Notification*> didMiniaturizeDispatcher;
		UnicastEventDispatcher<const NS::Notification*> didDeminiaturizeDispatcher;
		UnicastEventDispatcher<const NS::Notification*> didEnterFullScreenDispatcher;
		UnicastEventDispatcher<const NS::Notification*> didExitFullScreenDispatcher;
	};

	struct NSWindowLifecycleEvents {
		UnicastEventDispatcher<const NS::Notification*> willCloseDispatcher;
	};
public:
	WindowEventDispatcher() = default;

	~WindowEventDispatcher() override = default;

public:
	void DispatchWindowDidResize(NS::Notification* notification) override;
	void DispatchWindowDidMove(NS::Notification* notification) override;
	void DispatchWindowDidBecomeKey(NS::Notification* notification) override;
	void DispatchWindowDidResignKey(NS::Notification* notification) override;
	void DispatchWindowDidMiniaturize(NS::Notification* notification) override;
	void DispatchWindowDidDeminiaturize(NS::Notification* notification) override;
	void DispatchWindowDidEnterFullScreen(NS::Notification* notification) override;
	void DispatchWindowDidExitFullScreen(NS::Notification* notification) override;
	void DispatchWindowWillClose(NS::Notification* notification) override;

public:
	NSWindowGeometryEvents nsWindowGeometryEvents;
	NSWindowFocusEvents nsWindowFocusEvents;
	NSWindowStateEvents nsWindowStateEvents;
	NSWindowLifecycleEvents nsWindowLifecycleEvents;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_WINDOWEVENTDISPATCHER_HPP
