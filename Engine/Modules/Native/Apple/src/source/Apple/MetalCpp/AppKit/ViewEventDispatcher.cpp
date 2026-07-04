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
	mouseDownDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseUp(Event* event) {
	mouseUpDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseDragged(Event* event) {
	mouseDraggedDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseDown(Event* event) {
	rightMouseDownDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseUp(Event* event) {
	rightMouseUpDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseDragged(Event* event) {
	rightMouseDraggedDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseDown(Event* event) {
	otherMouseDownDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseUp(Event* event) {
	otherMouseUpDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseDragged(Event* event) {
	otherMouseDraggedDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseMoved(Event* event) {
	mouseMovedDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseEntered(Event* event) {
	mouseEnteredDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseExited(Event* event) {
	mouseExitedDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchKeyDown(Event* event) {
	keyDownDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchKeyUp(Event* event) {
	keyUpDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchFlagsChanged(Event* event) {
	flagsChangedDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchScrollWheel(Event* event) {
	scrollWheelDelegate.Dispatch(event);
}

void ViewEventDispatcher::DispatchViewDidMoveToWindow() {
	viewDidMoveToWindowDelegate.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidMoveToSuperview() {
	viewDidMoveToSuperviewDelegate.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidLayout() {
	viewDidLayoutDelegate.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidEndLiveResize() {
	viewDidEndLiveResizeDelegate.Dispatch();
}

}
