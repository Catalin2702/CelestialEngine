//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: RenderViewController.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-25
// Updated by: Catalin Chirosca
// Updated: 2026-05-25
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCONTROLLER_HPP
#define CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCONTROLLER_HPP

#include "MetalCpp/Foundation/Foundation.hpp"

#include "MetalCpp/AppKit/AppKitPrivate.hpp"

namespace MTK {
	class View;
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

	[[nodiscard]] RenderViewController* init(const CGRect& frame, const MTL::Device* device) const;

public:
	[[nodiscard]] MTK::View* view() const;
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

_NS_INLINE MTK::View* RenderViewController::view() const {
	return sendMessage<MTK::View*>(this, _APPKIT_PRIVATE_SEL(view));
}

}

#endif //CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEWCONTROLLER_HPP
