//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/AppKit
// File: NsWindowEventDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-22
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_NSWINDOWEVENTDISPATCHER_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_NSWINDOWEVENTDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

#include <AppKit/AppKit.hpp>


namespace CE::Native {

/**
 * @class NsWindowEventDispatcher
 * @brief Forwards the native NSWindow notifications to per-event unicast dispatchers
 * @details Installed as the window's delegate: geometry (resize/move), focus (become/resign key), state
 *			(miniaturize/fullscreen) and lifecycle (will close) notifications are re-emitted on the matching
 *			UnicastDispatcher, which the application binds to the event hub's Receive* methods.
 */
class CE_APPLE_API NsWindowEventDispatcher: public NS::I_WindowEventDispatcher {
public:
	struct CE_APPLE_API NsWindowGeometryEvents {
		UnicastDispatcher<const NS::Notification*> didResizeDispatcher;
		UnicastDispatcher<const NS::Notification*> didMoveDispatcher;
	};

	struct CE_APPLE_API NsWindowFocusEvents {
		UnicastDispatcher<const NS::Notification*> didBecomeKeyDispatcher;
		UnicastDispatcher<const NS::Notification*> didResignKeyDispatcher;
	};

	struct CE_APPLE_API NsWindowStateEvents {
		UnicastDispatcher<const NS::Notification*> didMiniaturizeDispatcher;
		UnicastDispatcher<const NS::Notification*> didDeminiaturizeDispatcher;
		UnicastDispatcher<const NS::Notification*> didEnterFullScreenDispatcher;
		UnicastDispatcher<const NS::Notification*> didExitFullScreenDispatcher;
	};

	struct CE_APPLE_API NsWindowLifecycleEvents {
		UnicastDispatcher<const NS::Notification*> willCloseDispatcher;
	};
public:
	NsWindowEventDispatcher() = default;

	~NsWindowEventDispatcher() override = default;

public:
	/**
	 * @brief Re-emits the native window did resize callback on its unicast dispatcher
	 */
	void DispatchWindowDidResize(NS::Notification* notification) override;
	/**
	 * @brief Re-emits the native window did move callback on its unicast dispatcher
	 */
	void DispatchWindowDidMove(NS::Notification* notification) override;
	/**
	 * @brief Re-emits the native window did become key callback on its unicast dispatcher
	 */
	void DispatchWindowDidBecomeKey(NS::Notification* notification) override;
	/**
	 * @brief Re-emits the native window did resign key callback on its unicast dispatcher
	 */
	void DispatchWindowDidResignKey(NS::Notification* notification) override;
	/**
	 * @brief Re-emits the native window did miniaturize callback on its unicast dispatcher
	 */
	void DispatchWindowDidMiniaturize(NS::Notification* notification) override;
	/**
	 * @brief Re-emits the native window did deminiaturize callback on its unicast dispatcher
	 */
	void DispatchWindowDidDeminiaturize(NS::Notification* notification) override;
	/**
	 * @brief Re-emits the native window did enter full screen callback on its unicast dispatcher
	 */
	void DispatchWindowDidEnterFullScreen(NS::Notification* notification) override;
	/**
	 * @brief Re-emits the native window did exit full screen callback on its unicast dispatcher
	 */
	void DispatchWindowDidExitFullScreen(NS::Notification* notification) override;
	/**
	 * @brief Re-emits the native window will close callback on its unicast dispatcher
	 */
	void DispatchWindowWillClose(NS::Notification* notification) override;

public:
	NsWindowGeometryEvents nsWindowGeometryEvents;
	NsWindowFocusEvents nsWindowFocusEvents;
	NsWindowStateEvents nsWindowStateEvents;
	NsWindowLifecycleEvents nsWindowLifecycleEvents;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_NSWINDOWEVENTDISPATCHER_HPP
