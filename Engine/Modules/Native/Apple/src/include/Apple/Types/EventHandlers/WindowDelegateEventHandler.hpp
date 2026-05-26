//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: WindowDelegateEventHandler.hpp
// Created by: Catalin Chirosca
// Updated: 2026-05-21
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_WINDOWDELEGATEEVENTHANDLER_HPP
#define CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_WINDOWDELEGATEEVENTHANDLER_HPP

#include "Apple/Types/Types.hpp"
#include "Apple/Types/EventHandlers/I_WindowDelegateEventHandler.hpp"

namespace CE::Apple::Types {

class WindowDelegateEventHandler final: public I_WindowDelegateEventHandler {
public:
	void OnWindowWillClose(NativeNotificationCallback callback) override;
	void OnWindowWillEnterFullScreen(NativeNotificationCallback callback) override;
	void OnWindowDidEnterFullScreen(NativeNotificationCallback callback) override;
	void OnWindowWillExitFullScreen(NativeNotificationCallback callback) override;
	void OnWindowDidExitFullScreen(NativeNotificationCallback callback) override;
	void OnWindowDidResize(NativeNotificationCallback callback) override;
	void OnWindowWillMiniaturize(NativeNotificationCallback callback) override;
	void OnWindowDidMiniaturize(NativeNotificationCallback callback) override;
	void OnWindowDidDeminiaturize(NativeNotificationCallback callback) override;
	void OnWindowDidMove(NativeNotificationCallback callback) override;
	void OnWindowDidBecomeKey(NativeNotificationCallback callback) override;
	void OnWindowDidResignKey(NativeNotificationCallback callback) override;

public:
	void DispatchWindowWillClose(NS::Notification* event) override;
	void DispatchWindowWillEnterFullScreen(NS::Notification* event) override;
	void DispatchWindowDidEnterFullScreen(NS::Notification* event) override;
	void DispatchWindowWillExitFullScreen(NS::Notification* event) override;
	void DispatchWindowDidExitFullScreen(NS::Notification* event) override;
	void DispatchWindowDidResize(NS::Notification* event) override;
	void DispatchWindowWillMiniaturize(NS::Notification* event) override;
	void DispatchWindowDidMiniaturize(NS::Notification* event) override;
	void DispatchWindowDidDeminiaturize(NS::Notification* event) override;
	void DispatchWindowDidMove(NS::Notification* event) override;
	void DispatchWindowDidBecomeKey(NS::Notification* event) override;
	void DispatchWindowDidResignKey(NS::Notification* event) override;

private:
	NativeNotificationCallback _windowWillCloseCallback;
	NativeNotificationCallback _windowWillEnterFullScreenCallback;
	NativeNotificationCallback _windowDidEnterFullScreenCallback;
	NativeNotificationCallback _windowWillExitFullScreenCallback;
	NativeNotificationCallback _windowDidExitFullScreenCallback;
	NativeNotificationCallback _windowDidResizeCallback;
	NativeNotificationCallback _windowWillMiniaturizeCallback;
	NativeNotificationCallback _windowDidMiniaturizeCallback;
	NativeNotificationCallback _windowDidDeminiaturizeCallback;
	NativeNotificationCallback _windowDidMoveCallback;
	NativeNotificationCallback _windowDidBecomeKeyCallback;
	NativeNotificationCallback _windowDidResignKeyCallback;
};

}

#endif //CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_WINDOWDELEGATEEVENTHANDLER_HPP
