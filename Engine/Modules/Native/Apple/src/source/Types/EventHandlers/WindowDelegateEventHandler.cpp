//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: WindowDelegateEventHandler.cpp
// Created by: Catalin Chirosca
// Updated: 2026-05-21
// Updated by: Catalin Chirosca
// Updated: 2026-05-21
//

#include "Types/EventHandlers/WindowDelegateEventHandler.hpp"

namespace CE::Apple::Types {

void WindowDelegateEventHandler::OnWindowWillClose(NativeNotificationCallback callback) {
	_windowWillCloseCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowWillEnterFullScreen(NativeNotificationCallback callback) {
	_windowWillEnterFullScreenCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowDidEnterFullScreen(NativeNotificationCallback callback) {
	_windowDidEnterFullScreenCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowWillExitFullScreen(NativeNotificationCallback callback) {
	_windowWillExitFullScreenCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowDidExitFullScreen(NativeNotificationCallback callback) {
	_windowDidExitFullScreenCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowDidResize(NativeNotificationCallback callback) {
	_windowDidResizeCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowWillMiniaturize(NativeNotificationCallback callback) {
	_windowWillMiniaturizeCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowDidMiniaturize(NativeNotificationCallback callback) {
	_windowDidMiniaturizeCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowDidDeminiaturize(NativeNotificationCallback callback) {
	_windowDidDeminiaturizeCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowDidMove(NativeNotificationCallback callback) {
	_windowDidMoveCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowDidBecomeKey(NativeNotificationCallback callback) {
	_windowDidBecomeKeyCallback = std::move(callback);
}

void WindowDelegateEventHandler::OnWindowDidResignKey(NativeNotificationCallback callback) {
	_windowDidResignKeyCallback = std::move(callback);
}

void WindowDelegateEventHandler::DispatchWindowWillClose(NS::Notification* event) {
	if (_windowWillCloseCallback)
		_windowWillCloseCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowWillEnterFullScreen(NS::Notification* event) {
	if (_windowWillEnterFullScreenCallback)
		_windowWillEnterFullScreenCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowDidEnterFullScreen(NS::Notification* event) {
	if (_windowDidEnterFullScreenCallback)
		_windowDidEnterFullScreenCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowWillExitFullScreen(NS::Notification* event) {
	if (_windowWillExitFullScreenCallback)
		_windowWillExitFullScreenCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowDidExitFullScreen(NS::Notification* event) {
	if (_windowDidExitFullScreenCallback)
		_windowDidExitFullScreenCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowDidResize(NS::Notification* event) {
	if (_windowDidResizeCallback)
		_windowDidResizeCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowWillMiniaturize(NS::Notification* event) {
	if (_windowWillMiniaturizeCallback)
		_windowWillMiniaturizeCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowDidMiniaturize(NS::Notification* event) {
	if (_windowDidMiniaturizeCallback)
		_windowDidMiniaturizeCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowDidDeminiaturize(NS::Notification* event) {
	if (_windowDidDeminiaturizeCallback)
		_windowDidDeminiaturizeCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowDidMove(NS::Notification* event) {
	if (_windowDidMoveCallback)
		_windowDidMoveCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowDidBecomeKey(NS::Notification* event) {
	if (_windowDidBecomeKeyCallback)
		_windowDidBecomeKeyCallback(event);
}

void WindowDelegateEventHandler::DispatchWindowDidResignKey(NS::Notification* event) {
	if (_windowDidResignKeyCallback)
		_windowDidResignKeyCallback(event);
}

}
