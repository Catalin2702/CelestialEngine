//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/AppKit
// File: NsViewEventDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Apple/MetalCpp/AppKit/NsViewEventDispatcher.hpp"

namespace CE::Native {

void NsViewEventDispatcher::DispatchMouseDown(NS::Event* event) {
	mouseEvents.mouseDownMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchMouseUp(NS::Event* event) {
	mouseEvents.mouseUpMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchMouseDragged(NS::Event* event) {
	mouseEvents.mouseDraggedMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchRightMouseDown(NS::Event* event) {
	mouseEvents.rightMouseDownMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchRightMouseUp(NS::Event* event) {
	mouseEvents.rightMouseUpMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchRightMouseDragged(NS::Event* event) {
	mouseEvents.rightMouseDraggedMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchOtherMouseDown(NS::Event* event) {
	mouseEvents.otherMouseDownMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchOtherMouseUp(NS::Event* event) {
	mouseEvents.otherMouseUpMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchOtherMouseDragged(NS::Event* event) {
	mouseEvents.otherMouseDraggedMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchMouseMoved(NS::Event* event) {
	mouseEvents.mouseMovedMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchMouseEntered(NS::Event* event) {
	mouseEvents.mouseEnteredMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchMouseExited(NS::Event* event) {
	mouseEvents.mouseExitedMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchScrollWheel(NS::Event* event) {
	mouseEvents.scrollWheelMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchKeyDown(NS::Event* event) {
	keyboardEvents.keyDownMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchKeyUp(NS::Event* event) {
	keyboardEvents.keyUpMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchFlagsChanged(NS::Event* event) {
	keyboardEvents.flagsChangedMulticastDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchViewDidMoveToWindow() {
	stateEvents.viewDidMoveToWindowMulticastDispatcher.Dispatch();
}

void NsViewEventDispatcher::DispatchViewDidMoveToSuperview() {
	stateEvents.viewDidMoveToSuperviewMulticastDispatcher.Dispatch();
}

void NsViewEventDispatcher::DispatchViewDidLayout() {
	stateEvents.viewDidLayoutMulticastDispatcher.Dispatch();
}

void NsViewEventDispatcher::DispatchViewDidEndLiveResize() {
	stateEvents.viewDidEndLiveResizeMulticastDispatcher.Dispatch();
}

}
