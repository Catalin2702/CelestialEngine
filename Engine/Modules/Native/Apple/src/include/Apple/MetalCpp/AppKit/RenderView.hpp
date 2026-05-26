//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: RenderView.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-05-26
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_HPP
#define CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_HPP

#include "Apple/MetalCpp/AppKit/AppKitPrivate.hpp"
#include "Apple/MetalCpp/Foundation/NSObject.hpp"
#include "Apple/Types/EventHandlers/I_ViewEventHandler.hpp"

#include <CoreGraphics/CGColorSpace.h>


namespace CA {
class MetalDrawable;
}
namespace MTL {
class Device;
class RenderPassDescriptor;
}

namespace NS {

/**
 * @class RenderView
 * @brief A view for rendering Metal content in a macOS application
 * @details This class represents a view that can be used for rendering Metal content in a macOS application. It provides methods for managing the view's properties, handling events, and accessing the current render pass descriptor for rendering operations. Clients can use this class to create and configure a view that is suitable for Metal rendering, as well as to handle user interactions and other events related to the view.
 */
class RenderView: public Referencing<RenderView> {
public:
	/**
	 * @brief Allocates a new instance of RenderView
	 * @return RenderView* Pointer to the newly allocated RenderView instance
	 * @details The alloc method is a static factory method that allocates a new instance of RenderView. Clients should call this method to create a new view before initializing it with init() or initWithCoder().
	 */
	static RenderView* alloc();

	/**
	 * @brief Initializes the RenderView instance
	 * @param frame The frame rectangle for the view, measured in points. The origin of the frame is relative to the superview's coordinate system.
	 * @param device A pointer to the MTLDevice that will be used for rendering. This device will be associated with the RenderView and used for rendering operations.
	 * @return RenderView* Pointer to the initialized RenderView instance
	 * @details The init method initializes the RenderView instance with a specified frame and Metal device.
	 *			Clients should call this method after allocating a new instance with alloc() to properly initialize the view with the desired frame and rendering device.
	 *			The provided frame will determine the size and position of the view, while the Metal device will be used for rendering operations.
	 *			Clients should ensure that the Metal device is properly initialized and compatible with the RenderView for successful rendering.
	 */
	[[nodiscard]] RenderView* init(const CGRect& frame, const MTL::Device* device);

	/**
	 * @brief Initializes the RenderView instance with a coder
	 * @param coder A pointer to the Coder object used for initialization
	 * @return RenderView* Pointer to the initialized RenderView instance
	 * @details The initWithCoder method initializes the RenderView instance using a Coder object. This is typically used when the view is being loaded from a storyboard or nib file. Clients should call this method after allocating a new instance with alloc() if they are initializing the view from a coder.
	 */
	[[nodiscard]] RenderView* init(const Coder* coder);

public:
	[[nodiscard]] CGPoint convertPointFromView(const CGPoint& point);
	[[nodiscard]] CGPoint convertPointFromView(const CGPoint& point, const RenderView* view);

	void releaseDrawables();

	void draw();

public:
	void setAutoresizeDrawable(bool autoresizeDrawable);

	void setClearColor(const MTL::ClearColor& clearColor);

	void setClearDepth(double clearDepth);

	void setClearStencil(uint32_t clearStencil);

	void setColorPixelFormat(MTL::PixelFormat colorPixelFormat);

	void setColorSpace(CGColorSpaceRef colorSpace);

	void SetDelegate(const void* delegate);

	void setDepthStencilAttachmentTextureUsage(MTL::TextureUsage textureUsage);

	void setDepthStencilPixelFormat(MTL::PixelFormat colorPixelFormat);

	void SetDevice(const MTL::Device* device);

	void setDrawableSize(CGSize drawableSize);

	void setEnableSetNeedsDisplay(bool enableSetNeedsDisplay);

	void SetFramebufferOnly(bool framebufferOnly);

	void SetEventHandler(I_ViewEventHandler* handler);

	void setLayer(CA::MetalLayer* layer);

	void setMultisampleColorAttachmentTextureUsage(MTL::TextureUsage textureUsage);

	void setOpaque(bool opaque);

	void setSampleCount(UInteger sampleCount);

	void setPaused(bool paused);

	void setPreferredFramesPerSecond(Integer preferredFramesPerSecond);

	void setPresentsWithTransaction(bool presentsWithTransaction);

	void setWantsLayer(bool wantsLayer);

public:
	[[nodiscard]] bool autoresizeDrawable() const;

	[[nodiscard]] MTL::ClearColor clearColor() const;

	[[nodiscard]] double clearDepth() const;

	[[nodiscard]] uint32_t clearStencil() const;

	[[nodiscard]] CGColorSpaceRef colorSpace() const;

	[[nodiscard]] MTL::PixelFormat colorPixelFormat() const;

	[[nodiscard]] CA::MetalDrawable* currentDrawable() const;

	[[nodiscard]] MTL::RenderPassDescriptor* currentRenderPassDescriptor() const;

	[[nodiscard]] MTL::TextureUsage depthStencilAttachmentTextureUsage() const;

	[[nodiscard]] MTL::PixelFormat depthStencilPixelFormat() const;

	[[nodiscard]] MTL::Texture* depthStencilTexture() const;

	[[nodiscard]] void* delegate() const;

	[[nodiscard]] MTL::Device* device() const;

	[[nodiscard]] CGSize drawableSize() const;

	[[nodiscard]] bool enableSetNeedsDisplay() const;

	[[nodiscard]] bool framebufferOnly() const;

	[[nodiscard]] CGRect frame() const;

	[[nodiscard]] bool isPaused() const;

	[[nodiscard]] void* layer() const;

	[[nodiscard]] MTL::TextureUsage multisampleColorAttachmentTextureUsage() const;

	[[nodiscard]] MTL::Texture* multisampleColorTexture() const;

	[[nodiscard]] UInteger sampleCount() const;

	[[nodiscard]] MTL::Device* preferredDevice() const;

	[[nodiscard]] CGSize preferredDrawableSize() const;

	[[nodiscard]] Integer preferredFramesPerSecond() const;

	[[nodiscard]] bool presentsWithTransaction() const;
};

_NS_INLINE RenderView* RenderView::alloc() {
	return sendMessage<RenderView*>(_APPKIT_PRIVATE_CLS(RenderView), _APPKIT_PRIVATE_SEL(alloc));
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE RenderView* RenderView::init(const CGRect& frame, const MTL::Device* device) {
	return sendMessage<RenderView*>(this, _APPKIT_PRIVATE_SEL(initWithFrame_), frame, device);
}

_NS_INLINE MTL::RenderPassDescriptor* RenderView::currentRenderPassDescriptor() const {
	return sendMessage<MTL::RenderPassDescriptor*>(this, _APPKIT_PRIVATE_SEL(currentRenderPassDescriptor));
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE CGPoint RenderView::convertPointFromView(const CGPoint& point) {
	return sendMessage<CGPoint>(this, _APPKIT_PRIVATE_SEL(convertPoint_fromView_), point, this);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE CGPoint RenderView::convertPointFromView(const CGPoint& point, const RenderView* view) {
	return sendMessage<CGPoint>(this, _APPKIT_PRIVATE_SEL(convertPoint_fromView_), point, view);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::SetEventHandler(I_ViewEventHandler* handler) {
	return sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setEventHandler_), handler);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setLayer(CA::MetalLayer* layer) {
	return sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setLayer_), layer);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setOpaque(const bool opaque) {
	return sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setOpaque_), opaque);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setWantsLayer(const bool wantsLayer) {
	return sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setWantsLayer_), wantsLayer);
}

_NS_INLINE CGRect RenderView::frame() const {
	return sendMessage<CGRect>(this, _APPKIT_PRIVATE_SEL(frame));
}

_NS_INLINE void* RenderView::layer() const {
	return sendMessage<void*>(this, _APPKIT_PRIVATE_SEL(layer));
}

}

#endif //CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_HPP
