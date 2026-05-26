//
// Module: CelestialEngine/Engine/Modules/Native/Apple/AppKit/View
// File: RenderViewController.h
// Created by: Catalin Chirosca
// Created: 2026-05-25
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCONTROLLER_H
#define CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCONTROLLER_H

#include "Apple/Types/EventHandlers/I_ViewControllerEventHandler.hpp"

#import <AppKit/AppKit.h>
#include <MetalKit/MetalKit.h>

#include <functional>

namespace NS {
class RenderViewController;
}

class MTLDevice;

/**
 * @class RenderViewController
 * @brief A view controller for managing a Metal rendering view in a macOS application
 * @details This class is responsible for managing the lifecycle of a Metal rendering view (MTKView) and providing hooks for customizing the view loading and setup process.
 *			It allows clients to set custom callbacks for the view loading and setup stages, enabling flexible integration with the rendering system.
 *			The RenderViewController can be used to encapsulate the logic for initializing and configuring the Metal view, as well as handling any necessary setup before rendering begins.
 */
@interface RenderViewController: NSViewController
/**
 * @brief Initializes the RenderViewController instance with a frame and Metal device
 * @param frame The frame rectangle for the view controller's view, measured in points. The origin of the frame is relative to the superview's coordinate system.
 * @param device A pointer to the MTLDevice that will be used for rendering. This device will be associated with the MTKView managed by this view controller.
 * @return An initialized instance of RenderViewController configured with the specified frame and Metal device.
 * @details This initializer allows clients to create a RenderViewController instance with a specific frame and Metal device. The provided frame will determine the size and position of the view controller's view, while the Metal device will be used for rendering operations. Clients should use this initializer when they need to set up a RenderViewController with specific rendering configurations from the outset.
 */
- (instancetype) initWithFrame:(NSRect)frame device:(id<MTLDevice>)device;

- (void)setEventHandler:(I_ViewControllerEventHandler*)handler;
@end

#endif //CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCONTROLLER_H
