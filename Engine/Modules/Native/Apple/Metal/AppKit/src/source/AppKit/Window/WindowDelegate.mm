//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/Window
// File: WindowDelegate.mm
// Created by: Catalin Chirosca
// Created: 2026-03-22
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
//

#include "AppKit/Window/WindowDelegate.h"

@implementation WindowDelegate

- (void *)getUserData {
	return self.userData;
}

- (void)setCallbacks:(WindowDelegateCallbacks)callbacks userData:(void *)userData {
	self.callbacks = callbacks;
	self.userData = userData;
}

- (void)windowWillClose:(NSNotification *)notification {
	if (not self.callbacks.WindowWillCloseEventCallback)
		return;

	self.callbacks.WindowWillCloseEventCallback(self.userData);
}

- (void)windowWillEnterFullScreen:(NSNotification *)notification {
	if (not self.callbacks.WindowWillEnterFullScreenEventCallback)
		return;

	self.callbacks.WindowWillEnterFullScreenEventCallback(self.userData);
}

- (void)windowDidEnterFullScreen:(NSNotification *)notification {
	if (not self.callbacks.WindowDidEnterFullScreenEventCallback)
		return;

	NSWindow* window = [notification object];
	NSRect frame = [window frame];

	self.callbacks.WindowDidEnterFullScreenEventCallback(
			self.userData,
			static_cast<unsigned int>(frame.size.width),
			static_cast<unsigned int>(frame.size.height)
	);
}

- (void)windowWillExitFullScreen:(NSNotification *)notification {
	if (not self.callbacks.WindowWillExitFullScreenEventCallback)
		return;

	self.callbacks.WindowWillExitFullScreenEventCallback(self.userData);
}

- (void)windowDidExitFullScreen:(NSNotification *)notification {
	if (not self.callbacks.WindowDidExitFullScreenEventCallback)
		return;

	NSWindow* window = [notification object];
	NSRect frame = [window frame];

	self.callbacks.WindowDidExitFullScreenEventCallback(
			self.userData,
			static_cast<unsigned int>(frame.size.width),
			static_cast<unsigned int>(frame.size.height)
	);
}

- (void)windowDidResize:(NSNotification *)notification {
	if (not self.callbacks.WindowDidResizeEventCallback)
		return;

	NSWindow* window = [notification object];
	NSRect frame = [window frame];

	self.callbacks.WindowDidResizeEventCallback(
			self.userData,
			static_cast<unsigned int>(frame.size.width),
			static_cast<unsigned int>(frame.size.height)
	);
}

- (void)windowWillMiniaturize:(NSNotification *)notification {
	if (not self.callbacks.WindowWillMiniaturizeEventCallback)
		return;

	self.callbacks.WindowWillMiniaturizeEventCallback(self.userData);
}

- (void)windowDidMiniaturize:(NSNotification *)notification {
	if (not self.callbacks.WindowDidMiniaturizeEventCallback)
		return;

	self.callbacks.WindowDidMiniaturizeEventCallback(self.userData);
}

- (void)windowDidDeminiaturize:(NSNotification *)notification {
	if (not self.callbacks.WindowDidDeminiaturizeEventCallback)
		return;

	NSWindow* window = [notification object];
	NSRect frame = [window frame];

	self.callbacks.WindowDidDeminiaturizeEventCallback(
		self.userData,
		static_cast<unsigned int>(frame.size.width),
		static_cast<unsigned int>(frame.size.height)
	);
}

- (void)windowDidMove:(NSNotification *)notification {
	if (not self.callbacks.WindowDidMoveEventCallback)
		return;

	NSWindow* window = [notification object];
	NSRect frame = [window frame];

	self.callbacks.WindowDidMoveEventCallback(
			self.userData,
			static_cast<unsigned int>(frame.origin.x),
			static_cast<unsigned int>(frame.origin.y)
	);
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
	if (not self.callbacks.WindowDidBecomeKeyEventCallback)
		return;

	self.callbacks.WindowDidBecomeKeyEventCallback(self.userData);
}

- (void)windowDidResignKey:(NSNotification *)notification {
	if (not self.callbacks.WindowDidResignKeyEventCallback)
		return;

	self.callbacks.WindowDidResignKeyEventCallback(self.userData);
}

@end
