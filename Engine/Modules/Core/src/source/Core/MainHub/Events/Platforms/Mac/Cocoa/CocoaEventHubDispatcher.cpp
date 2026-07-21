//
// Module: CelestialEngine/Engine/Modules/Core/MainHub/Events
// File: CocoaEventHubDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#include "Core/MainHub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp"

#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <AppKit/AppKit.hpp>

namespace CE::Core {

// Cocoa reports the cursor in the window's coordinate system, whose origin is the BOTTOM-left with Y growing upward.
// Convert it into the view's space, then flip Y so the engine (and ImGui) receive TOP-left origin coordinates with Y
// growing downward. Without this flip the vertical axis is inverted (moving the mouse down moves the cursor up).
static std::pair<float, float> MouseLocationTopLeft(const MetalContext* context, const CocoaWindow* window, const NS::Event* event) {
	if (not (context and context->GetView() and window))
		return {0.0f, 0.0f};

	const auto view = context->GetView();

	const auto [x, y] = view->convertPointFromView(event->locationInWindow(), nullptr);
	const auto [frameWidth, frameHeight] = window->GetFrameSize();

	return {static_cast<float>(x), frameHeight - static_cast<float>(y)};
}

void CocoaEventHubDispatcher::SetSources(MetalContext* context, CocoaWindow* window) {
	_context = context;
	_window = window;
}

void CocoaEventHubDispatcher::DispatchAppTickEvent(Events::AppTickEvent& appTickEvent) {
	cocoaApplicationEventHub.onTickMulticastDispatcher.Dispatch(appTickEvent);
}

void CocoaEventHubDispatcher::DispatchAppUpdateEvent(Events::AppUpdateEvent& appUpdateEvent) {
	cocoaApplicationEventHub.onUpdateMulticastDispatcher.Dispatch(appUpdateEvent);
}

void CocoaEventHubDispatcher::DispatchAppRenderEvent(Events::AppRenderEvent& appRenderEvent) {
	cocoaApplicationEventHub.onRenderMulticastDispatcher.Dispatch(appRenderEvent);
}

void CocoaEventHubDispatcher::DispatchAppErrorEvent(Events::ErrorEvent& appErrorEvent) {
	cocoaApplicationEventHub.onErrorMulticastDispatcher.Dispatch(appErrorEvent);
}

void CocoaEventHubDispatcher::DispatchKeyPressedEvent(Events::KeyPressedEvent& keyPressedEvent) {
	cocoaKeyboardEventHub.onPressedMulticastDispatcher.Dispatch(keyPressedEvent);
}

void CocoaEventHubDispatcher::DispatchKeyReleasedEvent(Events::KeyReleasedEvent& keyReleasedEvent) {
	cocoaKeyboardEventHub.onReleasedMulticastDispatcher.Dispatch(keyReleasedEvent);
}

void CocoaEventHubDispatcher::DispatchKeyTypedEvent(Events::KeyTypedEvent& keyTypedEvent) {
	cocoaKeyboardEventHub.onTypedMulticastDispatcher.Dispatch(keyTypedEvent);
}

void CocoaEventHubDispatcher::DispatchMouseMovedEvent(Events::MouseMovedEvent& mouseMovedEvent) {
	cocoaMouseEventHub.onMovedMulticastDispatcher.Dispatch(mouseMovedEvent);
}

void CocoaEventHubDispatcher::DispatchMouseButtonPressedEvent(Events::MouseButtonPressedEvent& mouseButtonPressedEvent) {
	cocoaMouseEventHub.onButtonPressedMulticastDispatcher.Dispatch(mouseButtonPressedEvent);
}

void CocoaEventHubDispatcher::DispatchMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& mouseButtonReleasedEvent) {
	cocoaMouseEventHub.onButtonReleasedMulticastDispatcher.Dispatch(mouseButtonReleasedEvent);
}

void CocoaEventHubDispatcher::DispatchMouseDraggedEvent(Events::MouseDraggedEvent& mouseDraggedEvent) {
	cocoaMouseEventHub.onDraggedMulticastDispatcher.Dispatch(mouseDraggedEvent);
}

void CocoaEventHubDispatcher::DispatchMouseWheelScrolledEvent(Events::MouseWheelScrolledEvent& mouseWheelScrolledEvent) {
	cocoaMouseEventHub.onWheelScrolledMulticastDispatcher.Dispatch(mouseWheelScrolledEvent);
}

void CocoaEventHubDispatcher::DispatchRenderContextChangeVSyncEvent(Events::VSyncChangeEvent& VSyncChangeEvent) {
	metalRenderContextEventHub.onChangeVSyncDispatcher.Dispatch(VSyncChangeEvent);
}

void CocoaEventHubDispatcher::DispatchRenderContextResizeViewEvent(Events::ViewResizeEvent& viewResizeEvent) {
	metalRenderContextEventHub.onResizeViewDispatcher.Dispatch(viewResizeEvent);
}

void CocoaEventHubDispatcher::DispatchWindowCloseEvent(Events::WindowCloseEvent& windowCloseEvent) {
	cocoaWindowEventHub.onCloseMulticastDispatcher.Dispatch(windowCloseEvent);
}

void CocoaEventHubDispatcher::DispatchWindowErrorEvent(Events::ErrorEvent& errorEvent) {
	cocoaWindowEventHub.onErrorMulticastDispatcher.Dispatch(errorEvent);
}

void CocoaEventHubDispatcher::DispatchWindowResizeEvent(Events::WindowResizeEvent& windowResizeEvent) {
	cocoaWindowEventHub.onResizeMulticastDispatcher.Dispatch(windowResizeEvent);
}

void CocoaEventHubDispatcher::ReceiveAppErrorEvent(const int errorCode, const char* description) {
	Events::ErrorEvent appErrorEvent{errorCode, description};
	DispatchAppErrorEvent(appErrorEvent);
}

void CocoaEventHubDispatcher::ReceiveAppRenderEvent() {
	Events::AppRenderEvent appRenderEvent;
	DispatchAppRenderEvent(appRenderEvent);
}

void CocoaEventHubDispatcher::ReceiveAppTickEvent() {
	Events::AppTickEvent appTickEvent;
	DispatchAppTickEvent(appTickEvent);
}

void CocoaEventHubDispatcher::ReceiveAppUpdateEvent() {
	Events::AppUpdateEvent appUpdateEvent;
	DispatchAppUpdateEvent(appUpdateEvent);
}

void CocoaEventHubDispatcher::ReceiveKeyDownEvent(const NS::Event* event) {
	if (not event)
		return;

	Events::KeyPressedEvent keyPressedEvent{Types::KeyboardKeyCodeFromCocoa(event->keyCode()), 0};
	DispatchKeyPressedEvent(keyPressedEvent);

	if (const auto* characters = event->characters(); characters and characters->length() > 0) {
		const unsigned int codepoint = characters->character(0);
		Events::KeyTypedEvent keyTypedEvent{codepoint};
		DispatchKeyTypedEvent(keyTypedEvent);
	}
}

void CocoaEventHubDispatcher::ReceiveKeyUpEvent(const NS::Event* event) {
	if (not event)
		return;

	Events::KeyReleasedEvent keyReleasedEvent{Types::KeyboardKeyCodeFromCocoa(event->keyCode())};
	DispatchKeyReleasedEvent(keyReleasedEvent);
}

void CocoaEventHubDispatcher::ReceiveMouseButtonDownEvent(const NS::Event* event) {
	if (not event)
		return;

	Events::MouseButtonPressedEvent mouseButtonPressedEvent{Types::MouseButtonKeyCodeFromCocoa(event->buttonNumber())};
	DispatchMouseButtonPressedEvent(mouseButtonPressedEvent);
}

void CocoaEventHubDispatcher::ReceiveMouseButtonUpEvent(const NS::Event* event) {
	if (not event)
		return;

	Events::MouseButtonReleasedEvent mouseButtonReleasedEvent{Types::MouseButtonKeyCodeFromCocoa(event->buttonNumber())};
	DispatchMouseButtonReleasedEvent(mouseButtonReleasedEvent);
}

void CocoaEventHubDispatcher::ReceiveMouseDraggedEvent(const NS::Event* event) {
	if (not event)
		return;

	const auto [x, y] = MouseLocationTopLeft(_context, _window, event);
	Events::MouseDraggedEvent mouseDraggedEvent{Types::MouseButtonKeyCodeFromCocoa(event->buttonNumber()), x, y};
	DispatchMouseDraggedEvent(mouseDraggedEvent);
}

void CocoaEventHubDispatcher::ReceiveMouseMovedEvent(const NS::Event* event) {
	if (not event)
		return;

	const auto [x, y] = MouseLocationTopLeft(_context, _window, event);
	Events::MouseMovedEvent mouseMovedEvent{x, y};
	DispatchMouseMovedEvent(mouseMovedEvent);
}

void CocoaEventHubDispatcher::ReceiveScrollWheelEvent(const NS::Event* event) {
	if (not event)
		return;

	Events::MouseWheelScrolledEvent mouseWheelScrolledEvent{
		static_cast<float>(event->scrollingDeltaX()),
		static_cast<float>(event->scrollingDeltaY())
	};
	DispatchMouseWheelScrolledEvent(mouseWheelScrolledEvent);
}

void CocoaEventHubDispatcher::ReceiveContextChangeVSyncEvent(const bool state) {
	Events::VSyncChangeEvent VSyncChangeEvent{state};
	DispatchRenderContextChangeVSyncEvent(VSyncChangeEvent);
}

void CocoaEventHubDispatcher::ReceiveContextResizeViewEvent(const double width, const double height) {
	Events::ViewResizeEvent viewResizeEvent{static_cast<unsigned int>(width), static_cast<unsigned int>(height)};
	DispatchRenderContextResizeViewEvent(viewResizeEvent);
}
void CocoaEventHubDispatcher::ReceiveWindowWillCloseEvent(const NS::Notification*) {
	Events::WindowCloseEvent windowCloseEvent{false};
	DispatchWindowCloseEvent(windowCloseEvent);
}

void CocoaEventHubDispatcher::ReceiveWindowErrorEvent(const int errorCode, const char* description) {
	Events::ErrorEvent errorEvent{errorCode, description};
	DispatchWindowErrorEvent(errorEvent);
}

void CocoaEventHubDispatcher::ReceiveWindowResizeEvent(const unsigned int width, const unsigned int height) {
	Events::WindowResizeEvent windowResizeEvent{width, height};
	DispatchWindowResizeEvent(windowResizeEvent);
}

}
