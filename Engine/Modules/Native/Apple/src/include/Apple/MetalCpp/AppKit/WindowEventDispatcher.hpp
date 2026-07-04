//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: WindowDelegate.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-22
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_WINDOWEVENTDISPATCHER_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_WINDOWEVENTDISPATCHER_HPP

#include <AppKit/AppKit.hpp>

#include "Utility/Callback/Dispatcher.hpp"

using NotificationDispatcher = CE::Utility::Dispatcher<const NS::Notification*>;

namespace NS {

class WindowEventDispatcher: public I_WindowEventDispatcher {
public:
	WindowEventDispatcher() = default;

	~WindowEventDispatcher() override = default;

public:
	void DispatchWindowDidResize(Notification* notification) override;
	void DispatchWindowDidMove(Notification* notification) override;
	void DispatchWindowDidBecomeKey(Notification* notification) override;
	void DispatchWindowDidResignKey(Notification* notification) override;
	void DispatchWindowDidMiniaturize(Notification* notification) override;
	void DispatchWindowDidDeminiaturize(Notification* notification) override;
	void DispatchWindowDidEnterFullScreen(Notification* notification) override;
	void DispatchWindowDidExitFullScreen(Notification* notification) override;
	void DispatchWindowWillClose(Notification* notification) override;

public:
	NotificationDispatcher windowDidResizeDispatcher;
	NotificationDispatcher windowDidMoveDispatcher;
	NotificationDispatcher windowDidBecomeKeyDispatcher;
	NotificationDispatcher windowDidResignKeyDispatcher;
	NotificationDispatcher windowDidMiniaturizeDispatcher;
	NotificationDispatcher windowDidDeminiaturizeDispatcher;
	NotificationDispatcher windowDidEnterFullScreenDispatcher;
	NotificationDispatcher windowDidExitFullScreenDispatcher;
	NotificationDispatcher windowWillCloseDispatcher;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_WINDOWEVENTDISPATCHER_HPP
