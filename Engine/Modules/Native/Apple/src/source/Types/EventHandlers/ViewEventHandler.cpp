//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Types/EventHandlers
// File: ViewEventHandler.cpp
// Created by: Catalin Chirosca
// Updated: 2026-05-21
// Updated by: Catalin Chirosca
// Updated: 2026-05-21
//

#include "Types/EventHandlers/ViewEventHandler.hpp"

namespace CE::Apple::Types {

void ViewEventHandler::OnKeyPressed(NativeEventCallback callback) {
	_keyPressedCallback = std::move(callback);
}

void ViewEventHandler::OnKeyReleased(NativeEventCallback callback) {
	_keyReleasedCallback = std::move(callback);
}

void ViewEventHandler::OnKeyTyped(NativeEventCallback callback) {
	_keyTypedCallback = std::move(callback);
}

void ViewEventHandler::OnMouseMoved(NativeEventCallback callback) {
	_mouseMovedCallback = std::move(callback);
}

void ViewEventHandler::OnMouseScrolled(NativeEventCallback callback) {
	_mouseScrolledCallback = std::move(callback);
}

void ViewEventHandler::OnMouseButtonPressed(NativeEventCallback callback) {
	_mouseButtonPressedCallback = std::move(callback);
}

void ViewEventHandler::OnMouseButtonReleased(NativeEventCallback callback) {
	_mouseButtonReleasedCallback = std::move(callback);
}

void ViewEventHandler::OnMouseDragged(NativeEventCallback callback) {
	_mouseDraggedCallback = std::move(callback);
}

void ViewEventHandler::DispatchKeyPressed(NS::Event* event) {
	if (_keyPressedCallback)
		_keyPressedCallback(event);
}

void ViewEventHandler::DispatchKeyReleased(NS::Event* event) {
	if (_keyReleasedCallback)
		_keyReleasedCallback(event);
}

void ViewEventHandler::DispatchKeyTyped(NS::Event* event) {
	if (_keyTypedCallback)
		_keyTypedCallback(event);
}

void ViewEventHandler::DispatchMouseMoved(NS::Event* event) {
	if (_mouseMovedCallback)
		_mouseMovedCallback(event);
}

void ViewEventHandler::DispatchMouseScrolled(NS::Event* event) {
	if (_mouseScrolledCallback)
		_mouseScrolledCallback(event);
}

void ViewEventHandler::DispatchMouseButtonPressed(NS::Event* event) {
	if (_mouseButtonPressedCallback)
		_mouseButtonPressedCallback(event);
}

void ViewEventHandler::DispatchMouseButtonReleased(NS::Event* event) {
	if (_mouseButtonReleasedCallback)
		_mouseButtonReleasedCallback(event);
}

void ViewEventHandler::DispatchMouseDragged(NS::Event* event) {
	if (_mouseDraggedCallback)
		_mouseDraggedCallback(event);
}

}
