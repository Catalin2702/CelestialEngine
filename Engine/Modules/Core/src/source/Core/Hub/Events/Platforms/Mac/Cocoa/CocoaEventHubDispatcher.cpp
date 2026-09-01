//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: CocoaEventHubDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp"

#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <AppKit/AppKit.hpp>


namespace CE::Core {

// Cocoa reports the cursor in the window's coordinate system, whose origin is the BOTTOM-left with Y growing upward.
// Convert it into the view's space, then flip Y so the engine (and ImGui) receive TOP-left origin coordinates with Y
// growing downward. Without this flip the vertical axis is inverted (moving the mouse down moves the cursor up).
static std::pair<f32, f32> MouseLocationTopLeft(const MetalContext* context, const CocoaWindow* window, const NS::Event* event) {
	if (not (context and context->GetView() and window)) [[unlikely]]
		return {0.0f, 0.0f};

	const auto view = context->GetView();

	const auto [x, y] = view->convertPointFromView(event->locationInWindow(), nullptr);
	const auto [frameWidth, frameHeight] = window->GetFrameSize();

	return {static_cast<f32>(x), frameHeight - static_cast<f32>(y)};
}

void CocoaEventHubDispatcher::SetSources(MetalContext* context, CocoaWindow* window) {
	_context = context;
	_window = window;
}

void CocoaEventHubDispatcher::DispatchAppTickEvent(Events::AppTickEvent& appTickEvent) {
	applicationEventHub.onTickMulticastDispatcher.Dispatch(appTickEvent);
}

void CocoaEventHubDispatcher::DispatchAppUpdateEvent(Events::AppUpdateEvent& appUpdateEvent) {
	applicationEventHub.onUpdateMulticastDispatcher.Dispatch(appUpdateEvent);
}

void CocoaEventHubDispatcher::DispatchAppRenderEvent(Events::AppRenderEvent& appRenderEvent) {
	applicationEventHub.onRenderMulticastDispatcher.Dispatch(appRenderEvent);
}

void CocoaEventHubDispatcher::DispatchAppErrorEvent(Events::ErrorEvent& appErrorEvent) {
	applicationEventHub.onErrorMulticastDispatcher.Dispatch(appErrorEvent);
}

void CocoaEventHubDispatcher::DispatchKeyPressedEvent(Events::KeyPressedEvent& keyPressedEvent) {
	keyboardEventHub.onPressedMulticastDispatcher.Dispatch(keyPressedEvent);
}

void CocoaEventHubDispatcher::DispatchKeyReleasedEvent(Events::KeyReleasedEvent& keyReleasedEvent) {
	keyboardEventHub.onReleasedMulticastDispatcher.Dispatch(keyReleasedEvent);
}

void CocoaEventHubDispatcher::DispatchKeyTypedEvent(Events::KeyTypedEvent& keyTypedEvent) {
	keyboardEventHub.onTypedMulticastDispatcher.Dispatch(keyTypedEvent);
}

void CocoaEventHubDispatcher::DispatchMouseMovedEvent(Events::MouseMovedEvent& mouseMovedEvent) {
	mouseEventHub.onMovedMulticastDispatcher.Dispatch(mouseMovedEvent);
}

void CocoaEventHubDispatcher::DispatchMouseButtonPressedEvent(Events::MouseButtonPressedEvent& mouseButtonPressedEvent) {
	mouseEventHub.onButtonPressedMulticastDispatcher.Dispatch(mouseButtonPressedEvent);
}

void CocoaEventHubDispatcher::DispatchMouseButtonReleasedEvent(Events::MouseButtonReleasedEvent& mouseButtonReleasedEvent) {
	mouseEventHub.onButtonReleasedMulticastDispatcher.Dispatch(mouseButtonReleasedEvent);
}

void CocoaEventHubDispatcher::DispatchMouseDraggedEvent(Events::MouseDraggedEvent& mouseDraggedEvent) {
	mouseEventHub.onDraggedMulticastDispatcher.Dispatch(mouseDraggedEvent);
}

void CocoaEventHubDispatcher::DispatchMouseWheelScrolledEvent(Events::MouseWheelScrolledEvent& mouseWheelScrolledEvent) {
	mouseEventHub.onWheelScrolledMulticastDispatcher.Dispatch(mouseWheelScrolledEvent);
}

void CocoaEventHubDispatcher::DispatchRenderContextChangeVSyncEvent(Events::VSyncEvent& VSyncChangeEvent) {
	renderContextEventHub.onChangeVSyncDispatcher.Dispatch(VSyncChangeEvent);
}

void CocoaEventHubDispatcher::DispatchRenderContextResizeViewEvent(Events::ViewResizeEvent& viewResizeEvent) {
	renderContextEventHub.onResizeViewDispatcher.Dispatch(viewResizeEvent);
}

void CocoaEventHubDispatcher::DispatchWindowCloseEvent(Events::WindowCloseEvent& windowCloseEvent) {
	windowEventHub.onCloseMulticastDispatcher.Dispatch(windowCloseEvent);
}

void CocoaEventHubDispatcher::DispatchWindowErrorEvent(Events::ErrorEvent& errorEvent) {
	windowEventHub.onErrorMulticastDispatcher.Dispatch(errorEvent);
}

void CocoaEventHubDispatcher::DispatchWindowResizeEvent(Events::WindowResizeEvent& windowResizeEvent) {
	windowEventHub.onResizeMulticastDispatcher.Dispatch(windowResizeEvent);
}

void CocoaEventHubDispatcher::DispatchWindowFocusEvent(Events::WindowFocusEvent& windowFocusEvent) {
	windowEventHub.onFocusMulticastDispatcher.Dispatch(windowFocusEvent);
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
	if (not event) [[unlikely]]
		return;

	Events::KeyPressedEvent keyPressedEvent{Types::KeyboardKeyCodeFromCocoa(event->keyCode()), 0};
	DispatchKeyPressedEvent(keyPressedEvent);

	if (const auto* characters = event->characters(); characters and characters->length() > 0) [[likely]] {
		const unsigned int codepoint = characters->character(0);
		Events::KeyTypedEvent keyTypedEvent{codepoint};
		DispatchKeyTypedEvent(keyTypedEvent);
	}
}

void CocoaEventHubDispatcher::ReceiveKeyUpEvent(const NS::Event* event) {
	if (not event) [[unlikely]]
		return;

	Events::KeyReleasedEvent keyReleasedEvent{Types::KeyboardKeyCodeFromCocoa(event->keyCode())};
	DispatchKeyReleasedEvent(keyReleasedEvent);
}

// AppKit does not send keyDown/keyUp for modifier keys: they arrive as a single flagsChanged event. The event's keyCode
// identifies which modifier changed, and the device-dependent modifier bits (IOKit NX_DEVICE*KEYMASK values, set by AppKit
// in modifierFlags alongside the device-independent masks) tell whether that specific left/right key is now down — the
// device-independent masks cannot, since e.g. releasing LeftShift while RightShift is held keeps the generic Shift bit set.
static NS::UInteger DeviceMaskForModifierKey(const Types::KeyboardKeyCode keyCode) {
	switch (keyCode) {
		case Types::KeyboardKeyCode::LeftControl: return 0x00000001;	// NX_DEVICELCTLKEYMASK
		case Types::KeyboardKeyCode::LeftShift: return 0x00000002;		// NX_DEVICELSHIFTKEYMASK
		case Types::KeyboardKeyCode::RightShift: return 0x00000004;		// NX_DEVICERSHIFTKEYMASK
		case Types::KeyboardKeyCode::LeftSuper: return 0x00000008;		// NX_DEVICELCMDKEYMASK
		case Types::KeyboardKeyCode::RightSuper: return 0x00000010;		// NX_DEVICERCMDKEYMASK
		case Types::KeyboardKeyCode::LeftAlt: return 0x00000020;		// NX_DEVICELALTKEYMASK
		case Types::KeyboardKeyCode::RightAlt: return 0x00000040;		// NX_DEVICERALTKEYMASK
		case Types::KeyboardKeyCode::RightControl: return 0x00002000;	// NX_DEVICERCTLKEYMASK
		default: return 0;
	}
}

void CocoaEventHubDispatcher::ReceiveFlagsChangedEvent(const NS::Event* event) {
	if (not event) [[unlikely]]
		return;

	const auto keyCode = Types::KeyboardKeyCodeFromCocoa(event->keyCode());
	const auto deviceMask = DeviceMaskForModifierKey(keyCode);
	if (deviceMask == 0) [[unlikely]]
		return; // Not a modifier we track (e.g. CapsLock, Fn)

	if (event->modifierFlags() & deviceMask) {
		Events::KeyPressedEvent keyPressedEvent{keyCode, 0};
		DispatchKeyPressedEvent(keyPressedEvent);
	}
	else {
		Events::KeyReleasedEvent keyReleasedEvent{keyCode};
		DispatchKeyReleasedEvent(keyReleasedEvent);
	}
}

void CocoaEventHubDispatcher::ReceiveMouseButtonDownEvent(const NS::Event* event) {
	if (not event) [[unlikely]]
		return;

	Events::MouseButtonPressedEvent mouseButtonPressedEvent{Types::MouseButtonKeyCodeFromCocoa(event->buttonNumber())};
	DispatchMouseButtonPressedEvent(mouseButtonPressedEvent);
}

void CocoaEventHubDispatcher::ReceiveMouseButtonUpEvent(const NS::Event* event) {
	if (not event) [[unlikely]]
		return;

	Events::MouseButtonReleasedEvent mouseButtonReleasedEvent{Types::MouseButtonKeyCodeFromCocoa(event->buttonNumber())};
	DispatchMouseButtonReleasedEvent(mouseButtonReleasedEvent);
}

void CocoaEventHubDispatcher::ReceiveMouseDraggedEvent(const NS::Event* event) {
	if (not event) [[unlikely]]
		return;

	const auto [x, y] = MouseLocationTopLeft(_context, _window, event);
	Events::MouseDraggedEvent mouseDraggedEvent{Types::MouseButtonKeyCodeFromCocoa(event->buttonNumber()), x, y};
	DispatchMouseDraggedEvent(mouseDraggedEvent);
}

void CocoaEventHubDispatcher::ReceiveMouseMovedEvent(const NS::Event* event) {
	if (not event) [[unlikely]]
		return;

	const auto [x, y] = MouseLocationTopLeft(_context, _window, event);
	Events::MouseMovedEvent mouseMovedEvent{x, y};
	DispatchMouseMovedEvent(mouseMovedEvent);
}

void CocoaEventHubDispatcher::ReceiveScrollWheelEvent(const NS::Event* event) {
	if (not event) [[unlikely]]
		return;

	Events::MouseWheelScrolledEvent mouseWheelScrolledEvent{
		static_cast<f32>(event->scrollingDeltaX()),
		static_cast<f32>(event->scrollingDeltaY())
	};
	DispatchMouseWheelScrolledEvent(mouseWheelScrolledEvent);
}

void CocoaEventHubDispatcher::ReceiveContextChangeVSyncEvent(const bool state) {
	Events::VSyncEvent VSyncChangeEvent{state};
	DispatchRenderContextChangeVSyncEvent(VSyncChangeEvent);
}

void CocoaEventHubDispatcher::ReceiveContextResizeViewEvent(const f64 width, const f64 height) {
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

void CocoaEventHubDispatcher::ReceiveWindowDidBecomeKeyEvent(const NS::Notification*) {
	Events::WindowFocusEvent windowFocusEvent{true};
	DispatchWindowFocusEvent(windowFocusEvent);
}

void CocoaEventHubDispatcher::ReceiveWindowDidResignKeyEvent(const NS::Notification*) {
	Events::WindowFocusEvent windowFocusEvent{false};
	DispatchWindowFocusEvent(windowFocusEvent);
}

}
