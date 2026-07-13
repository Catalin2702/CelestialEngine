//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Bridge/AppKit/Window
// File: WindowBridge.mm
// Created by: Catalin Chirosca
// Created: 2026-02-19
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <math.h>

namespace CE::Native {

bool IsKeyPressed(unsigned short keyCode) {
	// Use CGEventSourceKeyState to check if a key is currently pressed
	// CGEventSourceGetTypeID() returns the event source for the current user
	return CGEventSourceKeyState(kCGEventSourceStateHIDSystemState, keyCode);
}

bool IsMouseButtonPressed(int buttonNumber) {
	// Get the currently pressed mouse buttons as a bitmask
	NSUInteger pressedButtons = [NSEvent pressedMouseButtons];

	// Check if the specific button is pressed (bit is set)
	return (pressedButtons & (1 << buttonNumber)) != 0;
}

void GetMousePosition(void* cocoaWindow, float* outX, float* outY) {
	if (!cocoaWindow || !outX || !outY) {
		if (outX) *outX = 0.0f;
		if (outY) *outY = 0.0f;
		return;
	}

	@autoreleasepool {
		NSWindow* window = (__bridge NSWindow*)cocoaWindow;

		// Get mouse location in screen coordinates
		NSPoint mouseLocationScreen = [NSEvent mouseLocation];

		// Convert to window coordinates
		NSPoint mouseLocationWindow = [window convertPointFromScreen:mouseLocationScreen];

		// Get content view to flip Y coordinate
		NSView* contentView = [window contentView];
		CGFloat viewHeight = [contentView frame].size.height;

		// Flip Y coordinate (Cocoa uses bottom-left origin, we want top-left)
		*outX = (float)mouseLocationWindow.x;
		*outY = (float)(viewHeight - mouseLocationWindow.y);
	}
}

}
