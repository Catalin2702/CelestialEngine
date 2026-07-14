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

#include "Define/DynamicLinker.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

#include <AppKit/AppKit.hpp>

namespace CE::Native {

class CE_API NsWindowEventDispatcher: public NS::I_WindowEventDispatcher {
public:
	struct CE_API NsWindowGeometryEvents {
		UnicastDispatcher<const NS::Notification*> didResizeDispatcher;
		UnicastDispatcher<const NS::Notification*> didMoveDispatcher;
	};

	struct CE_API NsWindowFocusEvents {
		UnicastDispatcher<const NS::Notification*> didBecomeKeyDispatcher;
		UnicastDispatcher<const NS::Notification*> didResignKeyDispatcher;
	};

	struct CE_API NsWindowStateEvents {
		UnicastDispatcher<const NS::Notification*> didMiniaturizeDispatcher;
		UnicastDispatcher<const NS::Notification*> didDeminiaturizeDispatcher;
		UnicastDispatcher<const NS::Notification*> didEnterFullScreenDispatcher;
		UnicastDispatcher<const NS::Notification*> didExitFullScreenDispatcher;
	};

	struct CE_API NsWindowLifecycleEvents {
		UnicastDispatcher<const NS::Notification*> willCloseDispatcher;
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
