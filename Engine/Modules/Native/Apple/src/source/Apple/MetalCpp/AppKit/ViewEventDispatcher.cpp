//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: ViewEventDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Apple/MetalCpp/AppKit/ViewEventDispatcher.hpp"

namespace NS {

void ViewEventDispatcher::DispatchMouseDown(Event* event) {
	mouseEvents.mouseDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseUp(Event* event) {
	mouseEvents.mouseUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseDragged(Event* event) {
	mouseEvents.mouseDraggedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseDown(Event* event) {
	mouseEvents.rightMouseDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseUp(Event* event) {
	mouseEvents.rightMouseUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseDragged(Event* event) {
	mouseEvents.rightMouseDraggedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseDown(Event* event) {
	mouseEvents.otherMouseDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseUp(Event* event) {
	mouseEvents.otherMouseUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseDragged(Event* event) {
	mouseEvents.otherMouseDraggedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseMoved(Event* event) {
	mouseEvents.mouseMovedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseEntered(Event* event) {
	mouseEvents.mouseEnteredMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseExited(Event* event) {
	mouseEvents.mouseExitedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchScrollWheel(Event* event) {
	mouseEvents.scrollWheelMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchKeyDown(Event* event) {
	keyboardEvents.keyDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchKeyUp(Event* event) {
	keyboardEvents.keyUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchFlagsChanged(Event* event) {
	keyboardEvents.flagsChangedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchViewDidMoveToWindow() {
	stateEvents.viewDidMoveToWindowMulticastDispatcher.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidMoveToSuperview() {
	stateEvents.viewDidMoveToSuperviewMulticastDispatcher.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidLayout() {
	stateEvents.viewDidLayoutMulticastDispatcher.Dispatch();
}

void ViewEventDispatcher::DispatchViewDidEndLiveResize() {
	stateEvents.viewDidEndLiveResizeMulticastDispatcher.Dispatch();
}

}
