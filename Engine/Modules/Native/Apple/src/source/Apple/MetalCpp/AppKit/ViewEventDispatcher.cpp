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
	mouseDownDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseUp(Event* event) {
	mouseUpDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseDragged(Event* event) {
	mouseDraggedDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseDown(Event* event) {
	rightMouseDownDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseUp(Event* event) {
	rightMouseUpDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseDragged(Event* event) {
	rightMouseDraggedDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseDown(Event* event) {
	otherMouseDownDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseUp(Event* event) {
	otherMouseUpDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseDragged(Event* event) {
	otherMouseDraggedDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseMoved(Event* event) {
	mouseMovedDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseEntered(Event* event) {
	mouseEnteredDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseExited(Event* event) {
	mouseExitedDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchKeyDown(Event* event) {
	keyDownDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchKeyUp(Event* event) {
	keyUpDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchFlagsChanged(Event* event) {
	flagsChangedDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchScrollWheel(Event* event) {
	scrollWheelDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchViewDidMoveToWindow() {
	viewDidMoveToWindowDispatcher.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidMoveToSuperview() {
	viewDidMoveToSuperviewDispatcher.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidLayout() {
	viewDidLayoutDispatcher.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidEndLiveResize() {
	viewDidEndLiveResizeDispatcher.Dispatch();
}

}
