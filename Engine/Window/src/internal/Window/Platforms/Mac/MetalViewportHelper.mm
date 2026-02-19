//
// Created by Catalin Chirosca on 2026-02-19.
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CAMetalLayer.h>

extern "C" {

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

    // Usa maximumFramesPerSecond disponibile da macOS 10.15+
    if (@available(macOS 10.15, *)) {
        NSInteger maxFPS = [screen maximumFramesPerSecond];
        if (maxFPS > 0) {
            return (int)maxFPS;
        }
    }

    // Fallback per versioni più vecchie o se maximumFramesPerSecond fallisce
    // Prova a usare il mode del display
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
