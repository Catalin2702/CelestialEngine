//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: ViewEventDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#include "Apple/MetalCpp/AppKit/ViewEventDispatcher.hpp"

namespace CE::Native {

void ViewEventDispatcher::DispatchMouseDown(NS::Event* event) {
	mouseEvents.mouseDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseUp(NS::Event* event) {
	mouseEvents.mouseUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseDragged(NS::Event* event) {
	mouseEvents.mouseDraggedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseDown(NS::Event* event) {
	mouseEvents.rightMouseDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseUp(NS::Event* event) {
	mouseEvents.rightMouseUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchRightMouseDragged(NS::Event* event) {
	mouseEvents.rightMouseDraggedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseDown(NS::Event* event) {
	mouseEvents.otherMouseDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseUp(NS::Event* event) {
	mouseEvents.otherMouseUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchOtherMouseDragged(NS::Event* event) {
	mouseEvents.otherMouseDraggedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseMoved(NS::Event* event) {
	mouseEvents.mouseMovedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseEntered(NS::Event* event) {
	mouseEvents.mouseEnteredMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchMouseExited(NS::Event* event) {
	mouseEvents.mouseExitedMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchScrollWheel(NS::Event* event) {
	mouseEvents.scrollWheelMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchKeyDown(NS::Event* event) {
	keyboardEvents.keyDownMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchKeyUp(NS::Event* event) {
	keyboardEvents.keyUpMulticastDispatcher.Dispatch(event);
}

void ViewEventDispatcher::DispatchFlagsChanged(NS::Event* event) {
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
