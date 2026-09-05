//
// Module: CelestialEngine/Engine/Modules/Core/Hub/Events
// File: CocoaEventHubDispatcher.cpp
// Created by: Catalin Chirosca
// Created: 2026-07-14
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp"

#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#include <AppKit/AppKit.hpp>

namespace CE::Core {

// Cocoa reports the cursor in the window's coordinate system, whose origin is the BOTTOM-left with Y growing upward.
// Convert it into the view's space, then flip Y so the engine (and ImGui) receive TOP-left origin coordinates with Y
// growing downward. Without this flip the vertical axis is inverted (moving the mouse down moves the cursor up).
static std::pair<f32, f32> MouseLocationTopLeft(const CocoaWindow* window, const NS::Event* event) {
	if (not (window and window->GetWindow())) [[unlikely]]
		return {0.0f, 0.0f};

	// The content view, not the window's own MTK::View member: they are the same object on this path, but the legacy
	// CocoaApplication installs the render context's view instead, and what input is delivered to is whatever is
	// actually installed.
	const auto view = window->GetWindow()->contentView();
	if (not view) [[unlikely]]
		return {0.0f, 0.0f};

	const auto [x, y] = view->convertPointFromView(event->locationInWindow(), nullptr);

	// Points, not pixels, and that is the whole subtlety here. convertPointFromView answers in the view's own
	// coordinates, which are points; GetFrameSize is the drawable, which is pixels. Flipping one against the other
	// put every cursor position half a window below where it belonged on a Retina display - inside the window by the
	// engine's numbers, outside it by ImGui's, which is why events arrived and nothing ever reacted to them.
	//
	// Points is also what the rest of the engine means by a mouse position: GLFW reports screen coordinates, and
	// ImGui's DisplaySize is in the same space with DisplayFramebufferScale applied on top.
	const auto [frameWidth, frameHeight] = window->GetFrameSize();
	const auto scale = window->GetContentScale();
	const auto heightInPoints = scale > 0.0f ? static_cast<f32>(frameHeight) / scale : static_cast<f32>(frameHeight);

	return {static_cast<f32>(x), heightInPoints - static_cast<f32>(y)};
}

void CocoaEventHubDispatcher::SetSources(CocoaWindow* window) {
	_window = window;
}

void CocoaEventHubDispatcher::DispatchRenderContextResizeViewEvent(Events::ViewResizeEvent& viewResizeEvent) {
	renderContextEventHub.onResizeViewDispatcher.Dispatch(viewResizeEvent);
}

void CocoaEventHubDispatcher::ReceiveAppErrorEvent(const int errorCode, const char* description) {
	Events::ErrorEvent appErrorEvent{errorCode, description};
	DispatchAppErrorEvent(appErrorEvent);
}

void CocoaEventHubDispatcher::ReceiveAppRenderEvent() {
	Events::AppRenderEvent appRenderEvent;
	DispatchAppRenderEvent(appRenderEvent);
}

void CocoaEventHubDispatcher::ReceiveAppTickEvent(const f32 deltaTime) {
	Events::AppTickEvent appTickEvent{deltaTime};
	DispatchAppTickEvent(appTickEvent);
}

void CocoaEventHubDispatcher::ReceiveAppUpdateEvent() {
	Events::AppUpdateEvent appUpdateEvent;
	DispatchAppUpdateEvent(appUpdateEvent);
}

void CocoaEventHubDispatcher::ReceiveKeyDownEvent(const NS::Event* event) {
	if (not event) [[unlikely]]
		return;

	// AppKit marks an auto-repeated keyDown instead of sending a distinct event, which is how GLFW's GLFW_REPEAT
	// maps onto it: without this a held key repeats on the OpenGL backend and does not on the Metal one.
	Events::KeyPressedEvent keyPressedEvent{Types::KeyboardKeyCodeFromCocoa(event->keyCode()), event->isARepeat() ? 1 : 0};
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

	const auto [x, y] = MouseLocationTopLeft(_window, event);
	Events::MouseDraggedEvent mouseDraggedEvent{Types::MouseButtonKeyCodeFromCocoa(event->buttonNumber()), x, y};
	DispatchMouseDraggedEvent(mouseDraggedEvent);
}

void CocoaEventHubDispatcher::ReceiveMouseMovedEvent(const NS::Event* event) {
	if (not event) [[unlikely]]
		return;

	const auto [x, y] = MouseLocationTopLeft(_window, event);
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
