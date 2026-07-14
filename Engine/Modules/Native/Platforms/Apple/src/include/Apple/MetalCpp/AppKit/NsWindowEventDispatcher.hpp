//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/AppKit
// File: NsWindowEventDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-22
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_NSWINDOWEVENTDISPATCHER_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_NSWINDOWEVENTDISPATCHER_HPP

#include <AppKit/AppKit.hpp>

#include "Utility/Delegate/Dispatcher.hpp"

namespace CE::Native {

class NsWindowEventDispatcher: public NS::I_WindowEventDispatcher {
public:
	struct NsWindowGeometryEvents {
		UnicastEventDispatcher<const NS::Notification*> didResizeDispatcher;
		UnicastEventDispatcher<const NS::Notification*> didMoveDispatcher;
	};

	struct NsWindowFocusEvents {
		UnicastEventDispatcher<const NS::Notification*> didBecomeKeyDispatcher;
		UnicastEventDispatcher<const NS::Notification*> didResignKeyDispatcher;
	};

	struct NsWindowStateEvents {
		UnicastEventDispatcher<const NS::Notification*> didMiniaturizeDispatcher;
		UnicastEventDispatcher<const NS::Notification*> didDeminiaturizeDispatcher;
		UnicastEventDispatcher<const NS::Notification*> didEnterFullScreenDispatcher;
		UnicastEventDispatcher<const NS::Notification*> didExitFullScreenDispatcher;
	};

	struct NsWindowLifecycleEvents {
		UnicastEventDispatcher<const NS::Notification*> willCloseDispatcher;
	};
public:
	NsWindowEventDispatcher() = default;

	~NsWindowEventDispatcher() override = default;

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
	NsWindowGeometryEvents nsWindowGeometryEvents;
	NsWindowFocusEvents nsWindowFocusEvents;
	NsWindowStateEvents nsWindowStateEvents;
	NsWindowLifecycleEvents nsWindowLifecycleEvents;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_NSWINDOWEVENTDISPATCHER_HPP
