//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: I_EventHubDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/Hub/Events/I_EventHubDispatcher.hpp"


namespace CE::Core {

void I_EventHubDispatcher::DispatchAppErrorEvent(Events::ErrorEvent& appErrorEvent) {
	GetApplicationEventHub().onErrorMulticastDispatcher.Dispatch(appErrorEvent);
}

void I_EventHubDispatcher::DispatchAppRenderEvent(Events::AppRenderEvent& appRenderEvent) {
	GetApplicationEventHub().onRenderMulticastDispatcher.Dispatch(appRenderEvent);
}

void I_EventHubDispatcher::DispatchAppTickEvent(Events::AppTickEvent& appTickEvent) {
	GetApplicationEventHub().onTickMulticastDispatcher.Dispatch(appTickEvent);
}

void I_EventHubDispatcher::DispatchAppUpdateEvent(Events::AppUpdateEvent& appUpdateEvent) {
	GetApplicationEventHub().onUpdateMulticastDispatcher.Dispatch(appUpdateEvent);
}

void I_EventHubDispatcher::DispatchKeyPressedEvent(Events::KeyPressedEvent& keyPressedEvent) {
	GetKeyboardEventHub().onPressedMulticastDispatcher.Dispatch(keyPressedEvent);
}

void I_EventHubDispatcher::DispatchKeyReleasedEvent(Events::KeyReleasedEvent& keyReleasedEvent) {
	GetKeyboardEventHub().onReleasedMulticastDispatcher.Dispatch(keyReleasedEvent);
}

void I_EventHubDispatcher::DispatchKeyTypedEvent(Events::KeyTypedEvent& keyTypedEvent) {
	GetKeyboardEventHub().onTypedMulticastDispatcher.Dispatch(keyTypedEvent);
}

void I_EventHubDispatcher::DispatchMouseMovedEvent(Events::MouseMovedEvent& mouseMovedEvent) {
	GetMouseEventHub().onMovedMulticastDispatcher.Dispatch(mouseMovedEvent);
}

void I_EventHubDispatcher::DispatchMouseButtonPressedEvent(Events::MouseButtonPressedEvent& mouseButtonPressedEvent) {
	GetMouseEventHub().onButtonPressedMulticastDispatcher.Dispatch(mouseButtonPressedEvent);
}

void I_EventHubDispatcher::DispatchMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& mouseButtonReleasedEvent) {
	GetMouseEventHub().onButtonReleasedMulticastDispatcher.Dispatch(mouseButtonReleasedEvent);
}

void I_EventHubDispatcher::DispatchMouseDraggedEvent(Events::MouseDraggedEvent& mouseDraggedEvent) {
	GetMouseEventHub().onDraggedMulticastDispatcher.Dispatch(mouseDraggedEvent);
}

void I_EventHubDispatcher::DispatchMouseWheelScrolledEvent(Events::MouseWheelScrolledEvent& mouseWheelScrolledEvent) {
	GetMouseEventHub().onWheelScrolledMulticastDispatcher.Dispatch(mouseWheelScrolledEvent);
}

void I_EventHubDispatcher::DispatchWindowResizeEvent(Events::WindowResizeEvent& windowResizeEvent) {
	GetWindowEventHub().onResizeMulticastDispatcher.Dispatch(windowResizeEvent);
}

void I_EventHubDispatcher::DispatchWindowCloseEvent(Events::WindowCloseEvent& windowCloseEvent) {
	GetWindowEventHub().onCloseMulticastDispatcher.Dispatch(windowCloseEvent);
}

void I_EventHubDispatcher::DispatchWindowErrorEvent(Events::ErrorEvent& windowErrorEvent) {
	GetWindowEventHub().onErrorMulticastDispatcher.Dispatch(windowErrorEvent);
}

void I_EventHubDispatcher::DispatchWindowFocusEvent(Events::WindowFocusEvent& windowFocusEvent) {
	GetWindowEventHub().onFocusMulticastDispatcher.Dispatch(windowFocusEvent);
}

void I_EventHubDispatcher::DispatchRenderContextChangeVSyncEvent(Events::VSyncEvent& vSyncChangeEvent) {
	GetRenderContextEventHub().onChangeVSyncDispatcher.Dispatch(vSyncChangeEvent);
}

}
