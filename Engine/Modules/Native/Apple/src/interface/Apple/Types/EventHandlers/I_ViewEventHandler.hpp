//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: I_ViewCallbackHandler.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-20
// Updated by: Catalin Chirosca
// Updated: 2026-05-28
//

#pragma once

#ifndef CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_I_VIEWEVENTHANDLER_HPP
#define CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_I_VIEWEVENTHANDLER_HPP

#include <functional>

namespace NS {
	class Event;
}

using NativeEventCallback = std::function<void(NS::Event*)>;

namespace NS {
class Event;
}

/**
 * @brief Interface for view event handlers.
 */
class I_ViewEventHandler {
public:
	/**
	 * @brief Virtual destructor for proper cleanup in derived classes.
	 */
	virtual ~I_ViewEventHandler() = default;

public:
	/**
	 * @brief Registers a callback for key pressed events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnKeyPressed(NativeEventCallback callback) = 0;

	/**
	 * @brief Registers a callback for key released events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnKeyReleased(NativeEventCallback callback) = 0;

	/**
	 * @brief Registers a callback for key typed events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnKeyTyped(NativeEventCallback callback) = 0;

	/**
	 * @brief Registers a callback for mouse moved events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnMouseMoved(NativeEventCallback callback) = 0;

	/**
	 * @brief Registers a callback for mouse scrolled events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnMouseScrolled(NativeEventCallback callback) = 0;

	/**
	 * @brief Registers a callback for mouse button pressed events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnMouseButtonPressed(NativeEventCallback callback) = 0;

	/**
	 * @brief Registers a callback for mouse button released events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnMouseButtonReleased(NativeEventCallback callback) = 0;

	/**
	 * @brief Registers a callback for mouse dragged events.
	 * @param callback The callback function to be called when the event occurs.
	 */
	virtual void OnMouseDragged(NativeEventCallback callback) = 0;

public:
	/**
	 * @brief Dispatches the key pressed event to the registered callback.
	 * @param event The event data associated with the key pressed event.
	 */
	virtual void DispatchKeyPressed(NS::Event* event) = 0;

	/**
	 * @brief Dispatches the key released event to the registered callback.
	 * @param event The event data associated with the key released event.
	 */
	virtual void DispatchKeyReleased(NS::Event* event) = 0;

	/**
	 * @brief Dispatches the key typed event to the registered callback.
	 * @param event The event data associated with the key typed event.
	 */
	virtual void DispatchKeyTyped(NS::Event* event) = 0;

	/**
	 * @brief Dispatches the mouse moved event to the registered callback.
	 * @param event The event data associated with the mouse moved event.
	 */
	virtual void DispatchMouseMoved(NS::Event* event) = 0;

	/**
	 * @brief Dispatches the mouse scrolled event to the registered callback.
	 * @param event The event data associated with the mouse scrolled event.
	 */
	virtual void DispatchMouseScrolled(NS::Event* event) = 0;

	/**
	 * @brief Dispatches the mouse button pressed event to the registered callback.
	 * @param event The event data associated with the mouse button pressed event.
	 */
	virtual void DispatchMouseButtonPressed(NS::Event* event) = 0;

	/**
	 * @brief Dispatches the mouse button released event to the registered callback.
	 * @param event The event data associated with the mouse button released event.
	 */
	virtual void DispatchMouseButtonReleased(NS::Event* event) = 0;

	/**
	 * @brief Dispatches the mouse dragged event to the registered callback.
	 * @param event The event data associated with the mouse dragged event.
	 */
	virtual void DispatchMouseDragged(NS::Event* event) = 0;
};

#endif //CE_NATIVE_APPLE_TYPES_EVENTHANDLERS_I_VIEWEVENTHANDLER_HPP
