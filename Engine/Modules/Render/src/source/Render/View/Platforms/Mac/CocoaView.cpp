//
// Module: CelestialEngine/Engine/Modules/Render/View/Platforms/Mac
// File: CocoaView.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#include "Render/View/Platforms/Mac/CocoaView.hpp"

#include <AppKit/AppKit.hpp>

namespace CE::Render::View {
CocoaView::CocoaView(NS::Window* window) {
	assert(window && "CocoaView::CocoaView: NS::Window pointer cannot be null");
	// ReSharper disable All
	window->setContentView(_view.get());
	// ReSharper restore All
}

CocoaView::~CocoaView() {
	_view.reset();
}

void CocoaView::SetCocoaCallbacks(const CocoaCallbacks& callbacks) {
}

void CocoaView::SetKeyPressedEventCallback(const KeyPressedEventCallbackFn& callback) {
}

void CocoaView::SetKeyReleasedEventCallback(const KeyReleasedEventCallbackFn& callback) {
}

void CocoaView::SetKeyTypedEventCallback(const KeyTypedEventCallbackFn& callback) {
}

void CocoaView::SetMouseMovedEventCallback(const MouseMovedEventCallbackFn& callback) {
}

void CocoaView::SetMouseScrolledEventCallback(const MouseScrolledEventCallbackFn& callback) {
}

void CocoaView::
SetMouseButtonPressedEventCallback(const MouseButtonPressedEventCallbackFn& callback) {
}

void CocoaView::SetMouseButtonReleasedEventCallback(
	const MouseButtonReleasedEventCallbackFn& callback) {
}

void CocoaView::SetMouseDraggedEventCallback(const MouseDraggedEventCallbackFn& callback) {
}

}
