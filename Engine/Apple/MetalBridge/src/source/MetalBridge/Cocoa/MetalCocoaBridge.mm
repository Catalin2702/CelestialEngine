//
// Created by Catalin Chirosca on 2026-02-19.
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>


namespace CE::Bridge {

void* GetCocoaContentView(void* cocoaWindow) {
    if (!cocoaWindow)
        return nullptr;

    NSWindow* window = (__bridge NSWindow*)cocoaWindow;
    return (__bridge void*)[window contentView];
}

void SetCocoaViewLayer(void* cocoaView, void* metalLayer) {
    if (!cocoaView || !metalLayer)
        return;

    NSView* view = (__bridge NSView*)cocoaView;
    CAMetalLayer* layer = (__bridge CAMetalLayer*)metalLayer;

    [view setLayer:layer];
    [view setWantsLayer:YES];
}

void SetCocoaWindowContentView(void* cocoaWindow, void* cocoaView) {
    if (!cocoaWindow || !cocoaView)
        return;

    NSWindow* window = (__bridge NSWindow*)cocoaWindow;
    NSView* view = (__bridge NSView*)cocoaView;

    [window setContentView:view];
}

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

