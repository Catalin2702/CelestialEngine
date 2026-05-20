//
// Module: CelestialEngine/Engine/Modules/Native/Apple/CoreAnimation/DisplayLink
// File: DisplayLink.mm
// Created by: Catalin Chirosca
// Created: 2026-04-19
// Updated by: Catalin Chirosca
// Updated: 2026-05-21
//

#include "CoreAnimation/DisplayLink/DisplayLink.h"

#import <AppKit/AppKit.h>
#import <QuartzCore/CADisplayLink.h>

@implementation DisplayLink {
	CADisplayLink* _displayLink;
	I_DisplayLinkEventHandler* _eventHandler;
}

- (instancetype)init {
	if (self = [super init]) {
		_displayLink = [[[NSScreen mainScreen] displayLinkWithTarget:self selector:@selector(_tick:)] retain];
		if (!_displayLink) {
			NSLog(@"[DisplayLink] ::init - Failed to create CADisplayLink from NSScreen");
			return nil;
		}
	}
	return self;
}

- (void)dealloc {
	[self stop];
	[_displayLink release];
	_displayLink = nil;
	[super dealloc];
}

- (void)start {
	if (_displayLink) {
		[_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
	}
}

- (void)stop {
	if (_displayLink) {
		[_displayLink invalidate];
	}
}

- (void)_tick:(CADisplayLink*)sender {
	(void)sender;

	if (not _eventHandler)
		return;

    _eventHandler->DispatchTick();
}

- (void)setEventHandler:(I_DisplayLinkEventHandler*)handler {
	_eventHandler = handler;
}

@end
