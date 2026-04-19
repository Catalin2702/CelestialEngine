//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/CoreAnimation/DisplayLink
// File: DisplayLink.mm
// Created by: Catalin Chirosca
// Created: 2026-04-19
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#include "CoreAnimation/DisplayLink/DisplayLink.h"

#import <AppKit/AppKit.h>
#import <QuartzCore/CADisplayLink.h>

@implementation DisplayLink {
	CADisplayLink* _displayLink;
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
	_callback = nullptr;
	_userData = nullptr;
	if (_displayLink) {
		[_displayLink invalidate];
	}
}

- (void)_tick:(CADisplayLink*)sender {
	(void)sender;
	if (!_callback || !_userData)
	    return;

	DisplayLinkCallbackFn cb = _callback;
	void* ud = _userData;
	cb(ud);
}

- (void)setCallback:(DisplayLinkCallbackFn)callback userData:(void*)userData {
	_callback = callback;
	_userData = userData;
}

@end
