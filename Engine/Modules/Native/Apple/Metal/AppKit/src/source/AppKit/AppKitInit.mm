//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit
// File: AppKitInit.mm
// Created by: Catalin Chirosca
// Created: 2026-03-29
// Updated by: Catalin Chirosca
// Updated: 2026-03-29
//

#include "AppKit/AppKitInit.hpp"

#import <Foundation/Foundation.h>
#import <objc/runtime.h>

// Forward declare the classes to force the linker to include them
@class RenderView;
@class WindowDelegate;

// These symbols force the linker to include the Objective-C class implementations
static void* _ce_force_link_renderview = (void*)objc_getClass("RenderView");
static void* _ce_force_link_windowdelegate = (void*)objc_getClass("WindowDelegate");

bool CE_AppKit_Init(void) {
	// Force the Objective-C runtime to initialize the classes
	// by attempting to look them up
	Class renderViewClass = objc_getClass("RenderView");
	Class windowDelegateClass = objc_getClass("WindowDelegate");

	// Both classes should be available if the library is properly linked
	return (renderViewClass != nil) && (windowDelegateClass != nil);
}

bool CE_AppKit_IsRenderViewAvailable(void) {
	return objc_getClass("RenderView") != nil;
}

bool CE_AppKit_IsWindowDelegateAvailable(void) {
	return objc_getClass("WindowDelegate") != nil;
}

// Constructor attribute ensures this function is called when the library is loaded
__attribute__((constructor))
static void CE_AppKit_AutoInit(void) {
	// This ensures the classes are registered when the library loads
	(void)objc_getClass("RenderView");
	(void)objc_getClass("WindowDelegate");
}

