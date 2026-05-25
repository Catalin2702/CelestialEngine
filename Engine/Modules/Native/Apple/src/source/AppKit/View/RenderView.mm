//
// Module: CelestialEngine/Engine/Modules/Native/Apple/AppKit/View
// File: RenderView.mm
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-05-25
//

#include "AppKit/View/RenderView.h"
#include "MetalCpp/AppKit/NSEvent.hpp"

#import <QuartzCore/QuartzCore.h>

@interface RenderView()
{
	NSTrackingArea* _trackingArea;
	I_ViewEventHandler* _eventHandler;
}
@end

@implementation RenderView

- (void)dealloc {
	if (_trackingArea) {
		[self removeTrackingArea:_trackingArea];
		[_trackingArea release];
		_trackingArea = nil;
	}

	[super dealloc];
}

- (BOOL)acceptsFirstResponder {
	return YES;
}

- (BOOL)becomeFirstResponder {
	return YES;
}

- (BOOL)canBecomeKeyView {
	return YES;
}

- (void)viewDidMoveToWindow {
	[super viewDidMoveToWindow];
	if (self.window) {
		[self.window makeFirstResponder:self];
	}
}

- (BOOL)isFlipped {
	return YES;
}

- (BOOL)wantsLayer {
	return YES;
}

- (BOOL)isOpaque {
	return YES;
}

- (BOOL)wantsUpdateLayer {
	return YES;
}

- (CALayer*)makeBackingLayer {
	CAMetalLayer* layer = [CAMetalLayer layer];
	layer.opaque = YES;
	return layer;
}

- (void)updateTrackingAreas {
	[super updateTrackingAreas];

	if (_trackingArea) {
		[self removeTrackingArea:_trackingArea];
	}

	_trackingArea = [[NSTrackingArea alloc]
		initWithRect:self.bounds
			 options:NSTrackingMouseMoved |
					 NSTrackingActiveInKeyWindow |
					 NSTrackingInVisibleRect
			   owner:self
			userInfo:nil];

	[self addTrackingArea:_trackingArea];
}

- (void)setEventHandler:(I_ViewEventHandler*)handler {
	_eventHandler = handler;
}

- (void)mouseDown:(NSEvent*)event {
	// Ensure this view is the first responder to receive all input events
	if (self.window.firstResponder != self) {
		[self.window makeFirstResponder:self];
	}

	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseButtonPressed((__bridge NS::Event*)event);
}

- (void)mouseUp:(NSEvent*)event {
	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseButtonReleased((__bridge NS::Event*)event);
}

- (void)mouseDragged:(NSEvent*)event {
	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseDragged((__bridge NS::Event*)event);
}

- (void)rightMouseDown:(NSEvent*)event {
	// Ensure this view is the first responder to receive all input events
	if (self.window.firstResponder != self) {
		[self.window makeFirstResponder:self];
	}

	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseButtonPressed((__bridge NS::Event*)event);
}

- (void)rightMouseUp:(NSEvent*)event {
	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseButtonReleased((__bridge NS::Event*)event);
}

- (void)rightMouseDragged:(NSEvent*)event {
	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseDragged((__bridge NS::Event*)event);
}

- (void)otherMouseDown:(NSEvent*)event {
	// Ensure this view is the first responder to receive all input events
	if (self.window.firstResponder != self) {
		[self.window makeFirstResponder:self];
	}

	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseButtonPressed((__bridge NS::Event*)event);
}

- (void)otherMouseUp:(NSEvent*)event {
	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseButtonReleased((__bridge NS::Event*)event);
}

- (void)otherMouseDragged:(NSEvent*)event {
	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseDragged((__bridge NS::Event*)event);
}

- (void)mouseMoved:(NSEvent*)event {
	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseMoved((__bridge NS::Event*)event);
}

- (void)scrollWheel:(NSEvent*)event {
	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchMouseScrolled((__bridge NS::Event*)event);
}

- (void)keyDown:(NSEvent*)event {
	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchKeyPressed((__bridge NS::Event*)event);
	if (const auto* chars = [event characters]) {
		if (const char* utf8Chars = [chars UTF8String]; utf8Chars && utf8Chars[0] != '\0') {
			_eventHandler->DispatchKeyTyped((__bridge NS::Event*)event);
		}
	}
}

- (void)keyUp:(NSEvent*)event {
	if (not (_eventHandler and event)) {
		return;
	}

	_eventHandler->DispatchKeyReleased((__bridge NS::Event*)event);
}

@end
