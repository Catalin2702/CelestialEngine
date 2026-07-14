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
	mouseEvents.mouseDownDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchMouseUp(NS::Event* event) {
	mouseEvents.mouseUpDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchMouseDragged(NS::Event* event) {
	mouseEvents.mouseDraggedDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchRightMouseDown(NS::Event* event) {
	mouseEvents.rightMouseDownDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchRightMouseUp(NS::Event* event) {
	mouseEvents.rightMouseUpDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchRightMouseDragged(NS::Event* event) {
	mouseEvents.rightMouseDraggedDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchOtherMouseDown(NS::Event* event) {
	mouseEvents.otherMouseDownDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchOtherMouseUp(NS::Event* event) {
	mouseEvents.otherMouseUpDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchOtherMouseDragged(NS::Event* event) {
	mouseEvents.otherMouseDraggedDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchMouseMoved(NS::Event* event) {
	mouseEvents.mouseMovedDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchMouseEntered(NS::Event* event) {
	mouseEvents.mouseEnteredDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchMouseExited(NS::Event* event) {
	mouseEvents.mouseExitedDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchScrollWheel(NS::Event* event) {
	mouseEvents.scrollWheelDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchKeyDown(NS::Event* event) {
	keyboardEvents.keyDownDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchKeyUp(NS::Event* event) {
	keyboardEvents.keyUpDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchFlagsChanged(NS::Event* event) {
	keyboardEvents.flagsChangedDispatcher.Dispatch(event);
}

void NsViewEventDispatcher::DispatchViewDidMoveToWindow() {
	stateEvents.viewDidMoveToWindowDispatcher.Dispatch();
}

void NsViewEventDispatcher::DispatchViewDidMoveToSuperview() {
	stateEvents.viewDidMoveToSuperviewDispatcher.Dispatch();
}

void NsViewEventDispatcher::DispatchViewDidLayout() {
	stateEvents.viewDidLayoutDispatcher.Dispatch();
}

void NsViewEventDispatcher::DispatchViewDidEndLiveResize() {
	stateEvents.viewDidEndLiveResizeDispatcher.Dispatch();
}

}
