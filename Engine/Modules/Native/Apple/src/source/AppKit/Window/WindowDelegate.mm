//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/Window
// File: WindowDelegate.mm
// Created by: Catalin Chirosca
// Created: 2026-03-22
// Updated by: Catalin Chirosca
// Updated: 2026-05-21
//

#include "AppKit/Window/WindowDelegate.h"
#include "MetalCpp/Foundation/NSNotification.hpp"

@interface WindowDelegate()
{
	I_WindowDelegateEventHandler* _eventHandler;
}
@end

@implementation WindowDelegate

- (void)setEventHandler:(I_WindowDelegateEventHandler*)eventHandler {
	_eventHandler = eventHandler;
}

- (void)windowWillClose:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowWillClose((__bridge NS::Notification*)notification);
}

- (void)windowWillEnterFullScreen:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowWillEnterFullScreen((__bridge NS::Notification*)notification);
}

- (void)windowDidEnterFullScreen:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowDidEnterFullScreen((__bridge NS::Notification*)notification);
}

- (void)windowWillExitFullScreen:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowWillExitFullScreen((__bridge NS::Notification*)notification);
}

- (void)windowDidExitFullScreen:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowDidExitFullScreen((__bridge NS::Notification*)notification);
}

- (void)windowDidResize:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowDidResize((__bridge NS::Notification*)notification);
}

- (void)windowWillMiniaturize:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowWillMiniaturize((__bridge NS::Notification*)notification);
}

- (void)windowDidMiniaturize:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowDidMiniaturize((__bridge NS::Notification*)notification);
}

- (void)windowDidDeminiaturize:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowDidDeminiaturize((__bridge NS::Notification*)notification);
}

- (void)windowDidMove:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowDidMove((__bridge NS::Notification*)notification);
}

- (void)windowDidBecomeKey:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowDidBecomeKey((__bridge NS::Notification*)notification);
}

- (void)windowDidResignKey:(NSNotification *)notification {
	if (not (_eventHandler and notification))
		return;

	_eventHandler->DispatchWindowDidResignKey((__bridge NS::Notification*)notification);
}

@end
