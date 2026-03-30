//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/View
// File: RenderView.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-30
//

#pragma once

#ifndef CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_HPP
#define CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_HPP

// ReSharper disable once CppUnusedIncludeDirective
#include <CoreGraphics/CGGeometry.h>
#include <Foundation/NSObject.hpp>
#include "AppKit/AppKitPrivate.hpp"

#include "AppKit/View/RenderViewCallback.h"


namespace CA {
class MetalLayer;
}

namespace NS {

class RenderView: public Referencing<RenderView> {
public:
	static RenderView* alloc();
	[[nodiscard]] RenderView* init(const CGRect& frame) const;

public:
	void setCallbacks(const RenderViewCallbacks& callbacks, void* userData) const;
	void setLayer(CA::MetalLayer* layer) const;
	void setOpaque(bool opaque) const;
	void setWantsLayer(bool wantsLayer) const;

public:
	[[nodiscard]] CGRect frame() const;
	[[nodiscard]] void* getUserData() const;
	[[nodiscard]] void* layer() const;
};

_NS_INLINE RenderView* RenderView::alloc() {
	return sendMessage<RenderView*>(objc_getClass("RenderView"), _APPKIT_PRIVATE_SEL(alloc));
}

_NS_INLINE RenderView* RenderView::init(const CGRect& frame) const {
	return sendMessage<RenderView*>(this, _APPKIT_PRIVATE_SEL(initWithFrame_), frame);
}

_NS_INLINE void RenderView::setCallbacks(const RenderViewCallbacks& callbacks, void* userData) const {
	return sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setCallbacks_userData_), callbacks, userData);
}

_NS_INLINE void RenderView::setLayer(CA::MetalLayer* layer) const {
	return sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setLayer_), layer);
}

_NS_INLINE void RenderView::setOpaque(const bool opaque) const {
	return sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setOpaque_), opaque);
}

_NS_INLINE void RenderView::setWantsLayer(const bool wantsLayer) const {
	return sendMessage<void>(this, _APPKIT_PRIVATE_SEL(setWantsLayer_), wantsLayer);
}

_NS_INLINE CGRect RenderView::frame() const {
	return sendMessage<CGRect>(this, _APPKIT_PRIVATE_SEL(frame));
}

_NS_INLINE void* RenderView::getUserData() const {
	return sendMessage<void*>(this, _APPKIT_PRIVATE_SEL(getUserData_));
}

_NS_INLINE void* RenderView::layer() const {
	return sendMessage<void*>(this, _APPKIT_PRIVATE_SEL(layer));
}

}

#endif //CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_HPP
