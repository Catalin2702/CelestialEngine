//
// Module: CelestialEngine/Engine/Modules/MetalBridge/Cocoa
// File: CocoaBridge.mm
// Created by: Catalin Chirosca
// Created: 2026-02-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-04
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>
#import <math.h>


namespace CE::Bridge {

/**
 * @brief Gets the content view of a Cocoa window
 * @param cocoaWindow Void pointer to NSWindow
 * @return void* Pointer to the window's content NSView, or nullptr if window is invalid
 * @details Bridges C++ to Objective-C by casting the void pointer to NSWindow and
 *			retrieving its contentView. The returned pointer is bridged back to void*
 *			for C++ compatibility. Returns nullptr if the input window is null.
 */
void* GetCocoaContentView(void* cocoaWindow) {
	if (!cocoaWindow)
		return nullptr;

	NSWindow* window = (__bridge NSWindow*)cocoaWindow;
	return (__bridge void*)[window contentView];
}

/**
 * @brief Sets a Metal layer as the backing layer for a Cocoa view
 * @param cocoaView Void pointer to NSView
 * @param metalLayer Void pointer to CAMetalLayer
 * @details Configures the NSView to use a CAMetalLayer as its backing layer,
 *			enabling Metal rendering in the view. The view is configured with
 *			setWantsLayer:YES to enable layer-backed rendering. Does nothing
 *			if either parameter is null.
 */
void SetCocoaViewLayer(void* cocoaView, void* metalLayer) {
	if (!cocoaView || !metalLayer)
		return;

	NSView* view = (__bridge NSView*)cocoaView;
	CAMetalLayer* layer = (__bridge CAMetalLayer*)metalLayer;

	[view setLayer:layer];
	[view setWantsLayer:YES];
}

/**
 * @brief Sets the content view of a Cocoa window
 * @param cocoaWindow Void pointer to NSWindow
 * @param cocoaView Void pointer to NSView to set as the new content view
 * @details Replaces the window's current content view with the specified view.
 *			The new view becomes the primary view in the window's view hierarchy.
 *			Does nothing if either parameter is null.
 */
void SetCocoaWindowContentView(void* cocoaWindow, void* cocoaView) {
	if (!cocoaWindow || !cocoaView)
		return;

	NSWindow* window = (__bridge NSWindow*)cocoaWindow;
	NSView* view = (__bridge NSView*)cocoaView;

	[window setContentView:view];
}

/**
 * @brief Adds a subview to the window's content view
 * @param cocoaWindow Void pointer to NSWindow
 * @param subview Void pointer to NSView to add as a subview
 * @details Adds the specified NSView as a subview to the window's content view.
 *			The subview is configured to automatically resize with the content view
 *			(NSViewWidthSizable | NSViewHeightSizable) and its frame is set to
 *			match the content view's bounds. Does nothing if either parameter is null
 *			or if the window has no content view.
 */
void AddSubviewToContentView(void* cocoaWindow, void* subview) {
	if (!cocoaWindow || !subview)
		return;

	NSWindow* window = (__bridge NSWindow*)cocoaWindow;
	NSView* contentView = [window contentView];
	NSView* newSubview = (__bridge NSView*)subview;

	if (!contentView)
		return;

	[newSubview setAutoresizingMask:NSViewWidthSizable | NSViewHeightSizable];

	[newSubview setFrame:[contentView bounds]];

	[contentView addSubview:newSubview];
}

/**
 * @brief Gets the refresh rate of the display containing the window
 * @param cocoaWindow Void pointer to NSWindow
 * @return int Display refresh rate in Hz (e.g., 60, 120, 144)
 * @details Queries the display's refresh rate using multiple fallback methods:
 *			1. On macOS 10.15+, uses maximumFramesPerSecond from NSScreen
 *			2. Falls back to CGDisplayModeGetRefreshRate for older systems
 *			3. Returns 60 Hz as final fallback if all methods fail
 *			The refresh rate is useful for VSync configuration and smooth rendering.
 *			If the window is null or has no associated screen, defaults to 60 Hz.
 */
int GetDisplayRefreshRate(void* cocoaWindow) {
	if (!cocoaWindow)
		return 60; // Default fallback

	NSWindow* window = (__bridge NSWindow*)cocoaWindow;
	NSScreen* screen = [window screen];

	if (!screen) {
		screen = [NSScreen mainScreen];
	}

	if (!screen)
		return 60; // Default fallback

	if (@available(macOS 10.15, *)) {
		NSInteger maxFPS = [screen maximumFramesPerSecond];
		if (maxFPS > 0) {
			  return (int)maxFPS;
		}
	}

	NSDictionary* description = [screen deviceDescription];
	CGDirectDisplayID displayID = [[description objectForKey:@"NSScreenNumber"] unsignedIntValue];

	CGDisplayModeRef mode = CGDisplayCopyDisplayMode(displayID);
	if (mode) {
		double refreshRate = CGDisplayModeGetRefreshRate(mode);
		CGDisplayModeRelease(mode);

		if (refreshRate > 0) {
			  return (int)round(refreshRate);
		}
	}

	return 60; // Default fallback
}

}

