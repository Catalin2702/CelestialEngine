//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/View
// File: CocoaNSView.mm
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#include "Render/View/Platforms/Mac/AppKit/CocoaNSView.h"
#include "Render/View/Platforms/Mac/Bridge/ViewBridge.h"


@interface CocoaNSView ()
{
	CocoaViewImpl* _impl;
}
@end

@implementation CocoaNSView

- (void)setImpl:(void*)impl {
	_impl = (CocoaViewImpl*)impl;
}

- (BOOL)acceptsFirstResponder {
	return YES;
}

- (void)mouseDown:(NSEvent*)event {
	if (_impl)
		HandleMouseDown(_impl, event);
}

- (void)mouseUp:(NSEvent*)event {
	if (_impl)
		HandleMouseUp(_impl, event);
}

- (void)mouseDragged:(NSEvent*)event {
	if (_impl)
		HandleMouseDragged(_impl, event);
}

- (void)rightMouseDown:(NSEvent*)event {
	if (_impl)
		HandleMouseDown(_impl, event);
}

- (void)rightMouseUp:(NSEvent*)event {
	if (_impl)
		HandleMouseUp(_impl, event);
}

- (void)rightMouseDragged:(NSEvent*)event {
	if (_impl)
		HandleMouseDragged(_impl, event);
}

- (void)otherMouseDown:(NSEvent*)event {
	if (_impl)
		HandleMouseDown(_impl, event);
}

- (void)otherMouseUp:(NSEvent*)event {
	if (_impl)
		HandleMouseUp(_impl, event);
}

- (void)otherMouseDragged:(NSEvent*)event {
	if (_impl)
		HandleMouseDragged(_impl, event);
}

- (void)mouseMoved:(NSEvent*)event {
	if (_impl)
		HandleMouseMoved(_impl, event);
}

- (void)scrollWheel:(NSEvent*)event {
	if (_impl)
		HandleWheelScrolled(_impl, event);
}

- (void)keyDown:(NSEvent*)event {
	if (_impl)
		HandleKeyDown(_impl, event);
}

- (void)keyUp:(NSEvent*)event {
	if (_impl)
		HandleKeyUp(_impl, event);
}


@end
