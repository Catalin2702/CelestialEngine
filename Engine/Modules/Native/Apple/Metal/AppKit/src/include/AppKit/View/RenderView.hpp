//
// Module: CelestialEngine/Engine/Modules/Native/Apple/Metal/AppKit/View
// File: RenderView.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
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

class RenderView: public  NS::Referencing<RenderView> {
public:
	static RenderView* alloc();
	[[nodiscard]] RenderView* init(const CGRect& frame) const;
	void setLayer(CA::MetalLayer* layer) const;
	void setOpaque(bool opaque) const;
	void setWantsLayer(bool wantsLayer) const;
	void setCallbacks(const RenderViewCallbacks& callbacks, void* userData) const;
	[[nodiscard]] void* getUserData() const;
	[[nodiscard]] CGRect frame() const;
	[[nodiscard]] void* layer() const;
};

_NS_INLINE NS::RenderView* NS::RenderView::alloc() {
	return Object::sendMessage< RenderView* >( objc_getClass("RenderView"), _APPKIT_PRIVATE_SEL( alloc ) );
}

_NS_INLINE NS::RenderView* NS::RenderView::init(const CGRect& frame) const {
	return Object::sendMessage< RenderView* >( this, _APPKIT_PRIVATE_SEL( initWithFrame_ ), frame );
}

_NS_INLINE void NS::RenderView::setLayer(CA::MetalLayer* layer) const {
	return Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setLayer_ ), layer );
}

_NS_INLINE void NS::RenderView::setOpaque(const bool opaque) const {
	return Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setOpaque_ ), opaque );
}

_NS_INLINE void NS::RenderView::setWantsLayer(const bool wantsLayer) const {
	return Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setWantsLayer_ ), wantsLayer );
}

_NS_INLINE void NS::RenderView::setCallbacks(const RenderViewCallbacks& callbacks, void* userData) const {
	return Object::sendMessage< void >( this, _APPKIT_PRIVATE_SEL( setCallbacks_userData_ ), callbacks, userData );
}

_NS_INLINE void* NS::RenderView::getUserData() const {
	return Object::sendMessage< void* >( this, _APPKIT_PRIVATE_SEL( getUserData_ ) );
}

_NS_INLINE CGRect NS::RenderView::frame() const {
	return Object::sendMessage< CGRect >( this, _APPKIT_PRIVATE_SEL( frame ) );
}

_NS_INLINE void* NS::RenderView::layer() const {
	return Object::sendMessage< void* >( this, _APPKIT_PRIVATE_SEL( layer ) );
}

}

#endif //CE_NATIVE_APPLE_METAL_APPKIT_VIEW_RENDERVIEW_HPP
