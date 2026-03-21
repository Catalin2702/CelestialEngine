//
// Module: CelestialEngine/Engine/Modules/Render/View/Platforms/Mac/Bridge
// File: ViewBridge.mm
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#include "Render/View/Platforms/Mac/Bridge/ViewBridge.h"
#include "Render/View/Platforms/Mac/CocoaView.hpp"

#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"
#include "Types/KeyCode/KeyboardKeyCode.hpp"
#include "Types/KeyCode/MouseButtonCode.hpp"

#import <AppKit/AppKit.h>


struct CocoaViewImpl {
	NSView* view;
	void* owner;
	CE::Render::View::CocoaCallbacks* callbacks;
};

static NSPoint GetMousePos(NSView* view, NSEvent* event) {
	return [view convertPoint:[event locationInWindow] fromView:nil];
}

void HandleMouseDown(CocoaViewImpl* impl, NSEvent* event) {
	if (!impl->callbacks) return;

	const auto button = CE::KeyCode::MouseButtonKeyCodeFromCocoa([event buttonNumber]);

	CE::Events::MouseButtonPressedEvent _event{button};

	impl->callbacks->MouseButtonPressedEventCallback(_event);
}

void HandleMouseUp(CocoaViewImpl* impl, NSEvent* event) {
	if (!impl->callbacks) return;

	const auto button = CE::KeyCode::MouseButtonKeyCodeFromCocoa([event buttonNumber]);

	CE::Events::MouseButtonReleasedEvent _event{button};

	impl->callbacks->MouseButtonReleasedEventCallback(_event);
}

void HandleMouseMoved(CocoaViewImpl* impl, NSEvent* event) {
	if (!impl->callbacks) return;

	const auto point = GetMousePos(impl->view, event);

	CE::Events::MouseMovedEvent _event{static_cast<float>(point.x), static_cast<float>(point.y)};

	impl->callbacks->MouseMovedEventCallback(_event);
}

void HandleMouseDragged(CocoaViewImpl* impl, NSEvent* event) {
	if (!impl->callbacks) return;

	const auto point = GetMousePos(impl->view, event);
	const auto button = CE::KeyCode::MouseButtonKeyCodeFromCocoa([event buttonNumber]);

	CE::Events::MouseDraggedEvent _event{button, static_cast<float>(point.x), static_cast<float>(point.y)};

	impl->callbacks->MouseDraggedEventCallback(_event);
}

void HandleWheelScrolled(CocoaViewImpl* impl, NSEvent* event) {
	if (!impl->callbacks) return;

	CE::Events::MouseScrolledEvent _event{static_cast<float>([event scrollingDeltaX]), static_cast<float>([event scrollingDeltaY])};

	impl->callbacks->MouseScrolledEventCallback(_event);
}

void HandleKeyDown(CocoaViewImpl* impl, NSEvent* event) {
	if (!impl->callbacks) return;

	const auto keycode = CE::KeyCode::KeyboardKeyCodeFromCocoa([event keyCode]);

	CE::Events::KeyPressedEvent _event{keycode, 0};

	impl->callbacks->KeyPressedEventCallback(_event);
}

void HandleKeyUp(CocoaViewImpl* impl, NSEvent* event) {
	if (!impl->callbacks) return;
	if (!impl->callbacks) return;
	
	const auto keycode = CE::KeyCode::KeyboardKeyCodeFromCocoa([event keyCode]);

	CE::Events::KeyReleasedEvent _event{keycode};

	impl->callbacks->KeyReleasedEventCallback(_event);
}
