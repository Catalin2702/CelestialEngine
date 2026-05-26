//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: WindowDelegateCallback.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-20
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_I_WINDOWDELEGATEEVENTHANDLER_HPP
#define CE_NATIVE_APPLE_TYPES_EVENTCALLBACKS_I_WINDOWDELEGATEEVENTHANDLER_HPP

#include "Apple/Types/Types.hpp"

namespace NS {
class Notification;
}

/**
 * @brief Interface for window delegate event handlers.
 */
class I_WindowDelegateEventHandler {
public:
	/**
	 * @brief Virtual destructor for proper cleanup in derived classes.
	 */
	virtual ~I_WindowDelegateEventHandler() = default;

public:
	/**
	 * @brief Registers a callback for the window will close event.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowWillClose(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers callbacks for window full screen events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowWillEnterFullScreen(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers a callback for the window did enter full screen event.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowDidEnterFullScreen(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers a callback for the window will exit full screen event.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowWillExitFullScreen(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers a callback for the window did exit full screen event.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowDidExitFullScreen(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers a callback for the window did resize event.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowDidResize(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers callbacks for window miniaturize events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowWillMiniaturize(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers a callback for the window did miniaturize event.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowDidMiniaturize(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers a callback for the window did deminiaturize event.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowDidDeminiaturize(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers a callback for the window did move event.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowDidMove(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers callbacks for window key events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowDidBecomeKey(NativeNotificationCallback callback) = 0;

	/**
	 * @brief Registers a callback for the window did resign key event.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnWindowDidResignKey(NativeNotificationCallback callback) = 0;

public:
	/**
	 * @brief Dispatches the window will close event to the registered callback.
	 * @param notification The notification data associated with the window will close event.
	 */
	virtual void DispatchWindowWillClose(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window will enter full screen event to the registered callback.
	 * @param notification The notification data associated with the window will enter full screen event.
	 */
	virtual void DispatchWindowWillEnterFullScreen(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window did enter full screen event to the registered callback.
	 * @param notification The notification data associated with the window did enter full screen event.
	 */
	virtual void DispatchWindowDidEnterFullScreen(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window will exit full screen event to the registered callback.
	 * @param notification The notification data associated with the window will exit full screen event.
	 */
	virtual void DispatchWindowWillExitFullScreen(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window did exit full screen event to the registered callback.
	 * @param notification The notification data associated with the window did exit full screen event.
	 */
	virtual void DispatchWindowDidExitFullScreen(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window did resize event to the registered callback.
	 * @param notification The notification data associated with the window did resize event.
	 */
	virtual void DispatchWindowDidResize(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window will miniaturize event to the registered callback.
	 * @param notification The notification data associated with the window will miniaturize event.
	 */
	virtual void DispatchWindowWillMiniaturize(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window did miniaturize event to the registered callback.
	 * @param notification The notification data associated with the window did miniaturize event.
	 */
	virtual void DispatchWindowDidMiniaturize(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window did deminiaturize event to the registered callback.
	 * @param notification The notification data associated with the window did deminiaturize event.
	 */
	virtual void DispatchWindowDidDeminiaturize(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window did move event to the registered callback.
	 * @param notification The notification data associated with the window did move event.
	 */
	virtual void DispatchWindowDidMove(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window did become key event to the registered callback.
	 * @param notification The notification data associated with the window did become key event.
	 */
	virtual void DispatchWindowDidBecomeKey(NS::Notification* notification) = 0;

	/**
	 * @brief Dispatches the window did resign key event to the registered callback.
	 * @param notification The notification data associated with the window did resign key event.
	 */
	virtual void DispatchWindowDidResignKey(NS::Notification* notification) = 0;
};


#endif //CE_NATIVE_APPLE_TYPES_EVENTCALLBACKS_I_WINDOWDELEGATEEVENTHANDLER_HPP
