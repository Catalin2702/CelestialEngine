//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: ViewEventDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#include "Apple/MetalCpp/AppKit/ViewEventDispatcher.hpp"

namespace NS {

void ViewEventDispatcher::DispatchMouseDown(Event* event) {
	mouseDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseUp(Event* event) {
	mouseUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseDragged(Event* event) {
	mouseDraggedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseDown(Event* event) {
	rightMouseDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseUp(Event* event) {
	rightMouseUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseDragged(Event* event) {
	rightMouseDraggedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseDown(Event* event) {
	otherMouseDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseUp(Event* event) {
	otherMouseUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseDragged(Event* event) {
	otherMouseDraggedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseMoved(Event* event) {
	mouseMovedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseEntered(Event* event) {
	mouseEnteredMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseExited(Event* event) {
	mouseExitedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchKeyDown(Event* event) {
	keyDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchKeyUp(Event* event) {
	keyUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchFlagsChanged(Event* event) {
	flagsChangedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchScrollWheel(Event* event) {
	scrollWheelMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchViewDidMoveToWindow() {
	viewDidMoveToWindowMulticastDispatcher.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidMoveToSuperview() {
	viewDidMoveToSuperviewMulticastDispatcher.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidLayout() {
	viewDidLayoutMulticastDispatcher.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidEndLiveResize() {
	viewDidEndLiveResizeMulticastDispatcher.Dispatch();
}

}
