//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/CoreAnimation/DisplayLink
// File: DisplayLink.mm
// Created by: Catalin Chirosca
// Created: 2026-04-19
// Updated by: Catalin Chirosca
// Updated: 2026-04-19
//

#include "CoreAnimation/DisplayLink/DisplayLink.h"

#import <AppKit/AppKit.h>
#import <QuartzCore/CADisplayLink.h>

@implementation DisplayLink {
	CADisplayLink* _displayLink;
}

- (instancetype)init {
	if (self = [super init]) {
		_displayLink = [[NSScreen mainScreen] displayLinkWithTarget:self selector:@selector(_tick:)];
		if (!_displayLink) {
			NSLog(@"[DisplayLink] Failed to create CADisplayLink from NSScreen");
			return nil;
		}
		NSLog(@"[DisplayLink] Successfully created CADisplayLink");
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
	NSLog(@"[DisplayLink] start called");
	if (_displayLink) {
		[_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];
		NSLog(@"[DisplayLink] Added to run loop");
	}
}

- (void)stop {
	NSLog(@"[DisplayLink] stop called");
	if (_displayLink) {
		[_displayLink invalidate];
	}
}

- (void)_tick:(CADisplayLink*)sender {
	(void)sender; // Suppress unused parameter warning
	if (_callback && _userData) {
		_callback(_userData);
	}
}

- (void)setCallback:(DisplayLinkCallbackFn)callback userData:(void*)userData {
	NSLog(@"[DisplayLink] setCallback called: callback=%p, userData=%p", callback, userData);
	_callback = callback;
	_userData = userData;
}

@end

// C wrapper implementations
extern "C" {

void* CE_DisplayLink_Create(void) {
	DisplayLink* link = [[DisplayLink alloc] init];
	return (void*)link;
}

void CE_DisplayLink_Destroy(void* displayLink) {
	if (displayLink) {
		DisplayLink* link = (DisplayLink*)displayLink;
		[link stop];
		[link release];
	}
}

void CE_DisplayLink_SetCallback(void* displayLink, DisplayLinkCallbackFn callback, void* userData) {
	if (displayLink) {
		DisplayLink* link = (DisplayLink*)displayLink;
		[link setCallback:callback userData:userData];
	}
}

void CE_DisplayLink_Start(void* displayLink) {
	if (displayLink) {
		DisplayLink* link = (DisplayLink*)displayLink;
		[link start];
	}
}

void CE_DisplayLink_Stop(void* displayLink) {
	if (displayLink) {
		DisplayLink* link = (DisplayLink*)displayLink;
		[link stop];
	}
}

}
