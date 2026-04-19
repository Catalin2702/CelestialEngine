//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/View
// File: RenderView.mm
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#include "AppKit/View/RenderView.h"

#import <QuartzCore/QuartzCore.h>

@interface RenderView()
{
	NSTrackingArea* _trackingArea;
}
@end

@implementation RenderView

- (void)dealloc {
	if (_trackingArea) {
		[self removeTrackingArea:_trackingArea];
		[_trackingArea release];
		_trackingArea = nil;
	}

	_callbacks = {};
	_userData = nullptr;

	[super dealloc];
}

- (void)setCallbacks:(RenderViewCallbacks)callbacks userData:(void*)userData {
	_callbacks = callbacks;
	_userData = userData;
}

- (void*)getUserData {
	return _userData;
}

- (BOOL)acceptsFirstResponder {
	return YES;
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

- (void)mouseDown:(NSEvent*)event {
	if (self.callbacks.MouseButtonPressedEventCallback) {
		self.callbacks.MouseButtonPressedEventCallback(self.userData, [event buttonNumber]);
	}
}

- (void)mouseUp:(NSEvent*)event {
	if (self.callbacks.MouseButtonReleasedEventCallback) {
		self.callbacks.MouseButtonReleasedEventCallback(self.userData, [event buttonNumber]);
	}
}

- (void)mouseDragged:(NSEvent*)event {
	if (self.callbacks.MouseDraggedEventCallback) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		self.callbacks.MouseDraggedEventCallback(self.userData, [event buttonNumber], static_cast<float>(point.x), static_cast<float>(point.y));
	}
}

- (void)rightMouseDown:(NSEvent*)event {
	if (self.callbacks.MouseButtonPressedEventCallback) {
		self.callbacks.MouseButtonPressedEventCallback(self.userData, [event buttonNumber]);
	}
}

- (void)rightMouseUp:(NSEvent*)event {
	if (self.callbacks.MouseButtonReleasedEventCallback) {
		self.callbacks.MouseButtonReleasedEventCallback(self.userData, [event buttonNumber]);
	}
}

- (void)rightMouseDragged:(NSEvent*)event {
	if (self.callbacks.MouseDraggedEventCallback) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		self.callbacks.MouseDraggedEventCallback(self.userData, [event buttonNumber], static_cast<float>(point.x), static_cast<float>(point.y));
	}
}

- (void)otherMouseDown:(NSEvent*)event {
	if (self.callbacks.MouseButtonPressedEventCallback) {
		self.callbacks.MouseButtonPressedEventCallback(self.userData, [event buttonNumber]);
	}
}

- (void)otherMouseUp:(NSEvent*)event {
	if (self.callbacks.MouseButtonReleasedEventCallback) {
		self.callbacks.MouseButtonReleasedEventCallback(self.userData, [event buttonNumber]);
	}
}

- (void)otherMouseDragged:(NSEvent*)event {
	if (self.callbacks.MouseDraggedEventCallback) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		self.callbacks.MouseDraggedEventCallback(self.userData, [event buttonNumber], static_cast<float>(point.x), static_cast<float>(point.y));
	}
}

- (void)mouseMoved:(NSEvent*)event {
	if (self.callbacks.MouseMovedEventCallback) {
		NSPoint point = [self convertPoint:[event locationInWindow] fromView:nil];
		self.callbacks.MouseMovedEventCallback(self.userData, static_cast<float>(point.x), static_cast<float>(point.y));
	}
}

- (void)scrollWheel:(NSEvent*)event {
	if (self.callbacks.MouseScrolledEventCallback) {
		self.callbacks.MouseScrolledEventCallback(self.userData, static_cast<float>([event scrollingDeltaX]), static_cast<float>([event scrollingDeltaY]));
	}
}

- (void)keyDown:(NSEvent*)event {
	if (self.callbacks.KeyPressedEventCallback) {
		self.callbacks.KeyPressedEventCallback(self.userData, [event keyCode], 0);
	}
	if (const auto* chars = [event characters]) {
		if (const char* utf8Chars = [chars UTF8String]; utf8Chars && utf8Chars[0] != '\0') {
			if (self.callbacks.KeyTypedEventCallback) {
				self.callbacks.KeyTypedEventCallback(self.userData, utf8Chars[0]);
			}
		}
	}
}

- (void)keyUp:(NSEvent*)event {
	if (self.callbacks.KeyReleasedEventCallback) {
		self.callbacks.KeyReleasedEventCallback(self.userData, [event keyCode]);
	}
}

@end
