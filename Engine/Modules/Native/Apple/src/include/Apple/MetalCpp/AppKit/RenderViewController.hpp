//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: RenderViewController.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-25
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCONTROLLER_HPP
#define CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCONTROLLER_HPP

#include "Apple/MetalCpp/Foundation/Foundation.hpp"

#include "Apple/MetalCpp/AppKit/AppKitPrivate.hpp"
#include "Apple/Types/EventHandlers/ViewControllerEventHandler.hpp"

namespace MTK {
class RenderView;
}

namespace NS {

/**
 * @class RenderViewController
 * @brief A view controller for managing a Metal rendering view in a macOS application
 * @details This class is responsible for managing the lifecycle of a Metal rendering view (MTKView) and providing hooks for customizing the view loading and setup process.
 *			It allows clients to set custom callbacks for the view loading and setup stages, enabling flexible integration with the rendering system.
 *			The RenderViewController can be used to encapsulate the logic for initializing and configuring the Metal view, as well as handling any necessary setup before rendering begins.
 */
class RenderViewController: public Referencing<RenderViewController> {
public:
	/**
	 * @brief Allocates a new instance of RenderViewController
	 * @return RenderViewController* Pointer to the newly allocated RenderViewController instance
	 * @details The alloc method is a static factory method that allocates a new instance of RenderViewController. Clients should call this method to create a new view controller before initializing it with init() or initWithCoder().
	 */
	static RenderViewController* alloc();

	/**
	 * @brief Initializes the RenderViewController instance
	 * @return RenderViewController* Pointer to the initialized RenderViewController instance
	 * @details The init method initializes the RenderViewController instance. Clients should call this method after allocating a new instance with alloc() to properly initialize the view controller before use.
	 */
	[[nodiscard]] RenderViewController* init() const;

	/**
	 * @brief Initializes the RenderViewController instance with a coder
	 * @param coder A pointer to the Coder object used for initialization
	 * @return RenderViewController* Pointer to the initialized RenderViewController instance
	 * @details The initWithCoder method initializes the RenderViewController instance using a Coder object. This is typically used when the view controller is being loaded from a storyboard or nib file. Clients should call this method after allocating a new instance with alloc() if they are initializing the view controller from a coder.
	 */
	[[nodiscard]] RenderViewController* init(const Coder* coder) const;

	/**
	 * @brief Initializes the RenderViewController instance with a frame and Metal device
	 * @param frame The frame rectangle for the view controller's view, measured in points. The origin of the frame is relative to the superview's coordinate system.
	 * @param device A pointer to the MTLDevice that will be used for rendering. This device will be associated with the MTKView managed by this view controller.
	 * @return RenderViewController* Pointer to the initialized RenderViewController instance
	 * @details This initializer allows clients to create a RenderViewController instance with a specific frame and Metal device. The provided frame will determine the size and position of the view controller's view, while the Metal device will be used for rendering operations. Clients should use this initializer when they need to set up a RenderViewController with specific rendering configurations from the outset.
	 */
	[[nodiscard]] RenderViewController* init(const CGRect& frame, const MTL::Device* device) const;

public:
	/**
	 * @brief Sets the event handler for the RenderViewController
	 * @param handler A pointer to an object that implements the I_ViewControllerEventHandler interface
	 * @details This method allows clients to set a custom event handler for the RenderViewController. The event handler will receive callbacks for various view-related events, such as view loading and setup. Clients should implement the I_ViewControllerEventHandler interface and pass an instance of their event handler to this method to receive event notifications from the view controller.
	 */
	void SetEventHandler(I_ViewControllerEventHandler* handler) const;

public:
	/**
	 * @brief Retrieves the Metal rendering view (MTKView) managed by this RenderViewController
	 * @return MTK::View* Pointer to the MTKView instance managed by this view controller
	 * @details This method allows clients to access the MTKView instance that is managed by this RenderViewController. The MTKView is the primary view used for rendering Metal content, and clients may need to access it directly for certain configurations or operations. Clients should call this method to retrieve the MTKView instance after the view controller has been initialized and set up.
	 */
	[[nodiscard]] MTK::RenderView* view() const;
};

_NS_INLINE RenderViewController* RenderViewController::alloc() {
	return sendMessage<RenderViewController*>(_APPKIT_PRIVATE_CLS(RenderViewController), _APPKIT_PRIVATE_SEL(alloc));
}

_NS_INLINE RenderViewController* RenderViewController::init() const {
	return sendMessage<RenderViewController*>(this, _APPKIT_PRIVATE_SEL(init));
}

_NS_INLINE RenderViewController* RenderViewController::init(const Coder* coder) const {
	return sendMessage<RenderViewController*>(this, _APPKIT_PRIVATE_SEL(initWithCoder_), coder);
}

_NS_INLINE RenderViewController* RenderViewController::init(const CGRect& frame, const MTL::Device* device) const {
	return sendMessage<RenderViewController*>(this, _APPKIT_PRIVATE_SEL(initWithFrame_device_), frame, device);
}

_NS_INLINE void RenderViewController::SetEventHandler(I_ViewControllerEventHandler* handler) const {
	sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setEventHandler_), handler);
}

_NS_INLINE MTK::RenderView* RenderViewController::view() const {
	return sendMessage<MTK::RenderView*>(this, _APPKIT_PRIVATE_SEL(view));
}

}

#endif //CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCONTROLLER_HPP
