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

#include "MetalKitPrivate.hpp"
#include "MTKView.hpp"

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

namespace MTK {

/**
 * @class RenderView
 * @brief A view for rendering Metal content in a macOS application
 * @details This class represents a view that can be used for rendering Metal content in a macOS application. It provides methods for managing the view's properties, handling events, and accessing the current render pass descriptor for rendering operations. Clients can use this class to create and configure a view that is suitable for Metal rendering, as well as to handle user interactions and other events related to the view.
 */
class RenderView: public NS::Referencing<RenderView> {
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
	[[nodiscard]] RenderView* init(const NS::Coder* coder);

public:
	[[nodiscard]] CGPoint convertPointFromView(const CGPoint& point);

	[[nodiscard]] CGPoint convertPointFromView(const CGPoint& point, const RenderView* view);

public:
	void releaseDrawables();

	void draw();

public:
	void setAutoresizeDrawable(bool autoresizeDrawable);

	void setClearColor(const MTL::ClearColor& clearColor);

	void setClearDepth(double clearDepth);

	void setClearStencil(uint32_t clearStencil);

	void setColorPixelFormat(MTL::PixelFormat colorPixelFormat);

	void setColorSpace(CGColorSpaceRef colorSpace);

	void SetDelegate(const ViewDelegate* delegate);

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

	void setSampleCount(NS::UInteger sampleCount);

	void setPaused(bool paused);

	void setPreferredFramesPerSecond(NS::Integer preferredFramesPerSecond);

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

	[[nodiscard]] ViewDelegate* delegate() const;

	[[nodiscard]] MTL::Device* device() const;

	[[nodiscard]] CGSize drawableSize() const;

	[[nodiscard]] bool enableSetNeedsDisplay() const;

	[[nodiscard]] bool framebufferOnly() const;

	[[nodiscard]] CGRect frame() const;

	[[nodiscard]] bool isPaused() const;

	[[nodiscard]] MTL::TextureUsage multisampleColorAttachmentTextureUsage() const;

	[[nodiscard]] MTL::Texture* multisampleColorTexture() const;

	[[nodiscard]] NS::UInteger sampleCount() const;

	[[nodiscard]] MTL::Device* preferredDevice() const;

	[[nodiscard]] CGSize preferredDrawableSize() const;

	[[nodiscard]] NS::Integer preferredFramesPerSecond() const;

	[[nodiscard]] bool presentsWithTransaction() const;
};

_NS_INLINE RenderView* RenderView::alloc() {
	return NS::Object::alloc<RenderView>(_MTK_PRIVATE_CLS(RenderView));
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE RenderView* RenderView::init(const CGRect& frame, const MTL::Device* device) {
	return sendMessage<RenderView*>(this, _MTK_PRIVATE_SEL(initWithFrame_device_), frame, device);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE RenderView* RenderView::init(const NS::Coder* coder) {
	return sendMessage<RenderView*>(this, _MTK_PRIVATE_SEL(initWithCoder_), coder);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE CGPoint RenderView::convertPointFromView(const CGPoint& point) {
	return sendMessage<CGPoint>(this, _MTK_PRIVATE_SEL(convertPoint_fromView_), point, this);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE CGPoint RenderView::convertPointFromView(const CGPoint& point, const RenderView* view) {
	return sendMessage<CGPoint>(this, _MTK_PRIVATE_SEL(convertPoint_fromView_), point, view);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::releaseDrawables() {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(releaseDrawables));
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::draw() {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(draw));
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setAutoresizeDrawable(const bool autoresizeDrawable) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setAutoresizeDrawable_), autoresizeDrawable);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setClearColor(const MTL::ClearColor& clearColor) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setClearColor_), clearColor);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setClearDepth(const double clearDepth) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setClearDepth_), clearDepth);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setClearStencil(const uint32_t clearStencil) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setClearStencil_), clearStencil);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setColorPixelFormat(const MTL::PixelFormat colorPixelFormat) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setColorPixelFormat_), colorPixelFormat);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setColorSpace(CGColorSpaceRef colorSpace) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setColorspace_), colorSpace);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::SetDelegate(const ViewDelegate* delegate) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setDelegate_), delegate);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setDepthStencilAttachmentTextureUsage(const MTL::TextureUsage textureUsage) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setDepthStencilAttachmentTextureUsage_), textureUsage);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setDepthStencilPixelFormat(const MTL::PixelFormat colorPixelFormat) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setDepthStencilPixelFormat_), colorPixelFormat);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::SetDevice(const MTL::Device* device) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setDevice_), device);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setDrawableSize(const CGSize drawableSize) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setDrawableSize_), drawableSize);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setEnableSetNeedsDisplay(const bool enableSetNeedsDisplay) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setEnableSetNeedsDisplay_), enableSetNeedsDisplay);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::SetFramebufferOnly(const bool framebufferOnly) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setFramebufferOnly_), framebufferOnly);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::SetEventHandler(I_ViewEventHandler* handler) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setEventHandler_), handler);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setLayer(CA::MetalLayer* layer) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setLayer_), layer);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setMultisampleColorAttachmentTextureUsage(const MTL::TextureUsage textureUsage) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setMultisampleColorAttachmentTextureUsage_), textureUsage);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setOpaque(const bool opaque) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setOpaque_), opaque);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setSampleCount(const NS::UInteger sampleCount) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setSampleCount_), sampleCount);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setPaused(const bool paused) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setPaused_), paused);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setPreferredFramesPerSecond(const NS::Integer preferredFramesPerSecond) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setPreferredFramesPerSecond_), preferredFramesPerSecond);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setPresentsWithTransaction(const bool presentsWithTransaction) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setPresentsWithTransaction_), presentsWithTransaction);
}

// ReSharper disable once CppMemberFunctionMayBeConst
_NS_INLINE void RenderView::setWantsLayer(const bool wantsLayer) {
	sendMessage<void>(this, _MTK_PRIVATE_SEL(setWantsLayer_), wantsLayer);
}

_NS_INLINE bool RenderView::autoresizeDrawable() const {
	return sendMessage<bool>(this, _MTK_PRIVATE_SEL(autoresizeDrawable));
}

_NS_INLINE MTL::ClearColor RenderView::clearColor() const {
	return sendMessage<MTL::ClearColor>(this, _MTK_PRIVATE_SEL(clearColor));
}

_NS_INLINE double RenderView::clearDepth() const {
	return sendMessage<double>(this, _MTK_PRIVATE_SEL(clearDepth));
}

_NS_INLINE uint32_t RenderView::clearStencil() const {
	return sendMessage<uint32_t>(this, _MTK_PRIVATE_SEL(clearStencil));
}

_NS_INLINE CGColorSpaceRef RenderView::colorSpace() const {
	return sendMessage<CGColorSpaceRef>(this, _MTK_PRIVATE_SEL(colorspace));
}

_NS_INLINE MTL::PixelFormat RenderView::colorPixelFormat() const {
	return sendMessage<MTL::PixelFormat>(this, _MTK_PRIVATE_SEL(colorPixelFormat));
}

_NS_INLINE CA::MetalDrawable* RenderView::currentDrawable() const {
	return sendMessage<CA::MetalDrawable*>(this, _MTK_PRIVATE_SEL(currentDrawable));
}

_NS_INLINE MTL::RenderPassDescriptor* RenderView::currentRenderPassDescriptor() const {
	return sendMessage<MTL::RenderPassDescriptor*>(this, _MTK_PRIVATE_SEL(currentRenderPassDescriptor));
}

_NS_INLINE MTL::TextureUsage RenderView::depthStencilAttachmentTextureUsage() const {
	return sendMessage<MTL::TextureUsage>(this, _MTK_PRIVATE_SEL(depthStencilAttachmentTextureUsage));
}

_NS_INLINE MTL::PixelFormat RenderView::depthStencilPixelFormat() const {
	return sendMessage<MTL::PixelFormat>(this, _MTK_PRIVATE_SEL(depthStencilPixelFormat));
}

_NS_INLINE MTL::Texture* RenderView::depthStencilTexture() const {
	return sendMessage<MTL::Texture*>(this, _MTK_PRIVATE_SEL(depthStencilTexture));
}

_NS_INLINE ViewDelegate* RenderView::delegate() const {
	return sendMessage<ViewDelegate*>(this, _MTK_PRIVATE_SEL(delegate));
}

_NS_INLINE MTL::Device* RenderView::device() const {
	return sendMessage<MTL::Device*>(this, _MTK_PRIVATE_SEL(device));
}

_NS_INLINE CGSize RenderView::drawableSize() const {
	return sendMessage<CGSize>(this, _MTK_PRIVATE_SEL(drawableSize));
}

_NS_INLINE bool RenderView::enableSetNeedsDisplay() const {
	return sendMessage<bool>(this, _MTK_PRIVATE_SEL(enableSetNeedsDisplay));
}

_NS_INLINE bool RenderView::framebufferOnly() const {
	return sendMessage<bool>(this, _MTK_PRIVATE_SEL(framebufferOnly));
}

_NS_INLINE CGRect RenderView::frame() const {
	return sendMessage<CGRect>(this, _MTK_PRIVATE_SEL(frame));
}

_NS_INLINE bool RenderView::isPaused() const {
	return sendMessage<bool>(this, _MTK_PRIVATE_SEL(isPaused));
}

_NS_INLINE MTL::TextureUsage RenderView::multisampleColorAttachmentTextureUsage() const {
	return sendMessage<MTL::TextureUsage>(this, _MTK_PRIVATE_SEL(multisampleColorAttachmentTextureUsage));
}

_NS_INLINE MTL::Texture* RenderView::multisampleColorTexture() const {
	return sendMessage<MTL::Texture*>(this, _MTK_PRIVATE_SEL(multisampleColorTexture));
}

_NS_INLINE NS::UInteger RenderView::sampleCount() const {
	return sendMessage<NS::UInteger>(this, _MTK_PRIVATE_SEL(sampleCount));
}

_NS_INLINE MTL::Device* RenderView::preferredDevice() const {
	return sendMessage<MTL::Device*>(this, _MTK_PRIVATE_SEL(preferredDevice));
}

_NS_INLINE CGSize RenderView::preferredDrawableSize() const {
	return sendMessage<CGSize>(this, _MTK_PRIVATE_SEL(preferredDrawableSize));
}

_NS_INLINE NS::Integer RenderView::preferredFramesPerSecond() const {
	return sendMessage<NS::Integer>(this, _MTK_PRIVATE_SEL(preferredFramesPerSecond));
}

_NS_INLINE bool RenderView::presentsWithTransaction() const {
	return sendMessage<bool>(this, _MTK_PRIVATE_SEL(presentsWithTransaction));
}

}

#endif //CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_HPP
