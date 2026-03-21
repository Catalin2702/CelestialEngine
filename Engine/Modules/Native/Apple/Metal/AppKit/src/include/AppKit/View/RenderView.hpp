//
// Module: CelestialEngine/Engine/Modules/Render/View/Platforms/Mac
// File: RenderView.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#pragma once

#ifndef CE_RENDER_VIEW_MAC_COCOAVIEW_HPP
#define CE_RENDER_VIEW_MAC_COCOAVIEW_HPP

// ReSharper disable once CppUnusedIncludeDirective
#include <CoreGraphics/CGGeometry.h>
#include <Foundation/NSObject.hpp>
#include "AppKit/AppKitPrivate.hpp"

#include "AppKit/View/RenderViewCallback.h"

#include "Utility/Callback/Callback.hpp"

// namespace CE::Events {
// class I_Event;
//
// class KeyPressedEvent;
// class KeyReleasedEvent;
// class KeyTypedEvent;
//
// class MouseMovedEvent;
// class MouseScrolledEvent;
// class MouseButtonPressedEvent;
// class MouseButtonReleasedEvent;
// class MouseDraggedEvent;
// }
//
// using KeyPressedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::KeyPressedEvent>;
// using KeyReleasedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::KeyReleasedEvent>;
// using KeyTypedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::KeyTypedEvent>;
//
// using MouseMovedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::MouseMovedEvent>;
// using MouseScrolledEventCallbackFn = CE::Utility::CallbackFn<CE::Events::MouseScrolledEvent>;
// using MouseButtonPressedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::MouseButtonPressedEvent>;
// using MouseButtonReleasedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::MouseButtonReleasedEvent>;
// using MouseDraggedEventCallbackFn = CE::Utility::CallbackFn<CE::Events::MouseDraggedEvent>;
//
// namespace CE::Render::View {
//
// struct CocoaCallbacks {
// #pragma region KeyCallbacks
// 	KeyPressedEventCallbackFn KeyPressedEventCallback;						///< Callback for key pressed events
// 	KeyReleasedEventCallbackFn KeyReleasedEventCallback;					///< Callback for key released events
// 	KeyTypedEventCallbackFn KeyTypedEventCallback;							///< Callback for key typed events
// #pragma endregion
//
// #pragma region MouseCallbacks
// 	MouseMovedEventCallbackFn MouseMovedEventCallback;						///< Callback for mouse moved events
// 	MouseScrolledEventCallbackFn MouseScrolledEventCallback;				///< Callback for mouse scrolled events
// 	MouseButtonPressedEventCallbackFn MouseButtonPressedEventCallback;		///< Callback for mouse button pressed events
// 	MouseButtonReleasedEventCallbackFn MouseButtonReleasedEventCallback;	///< Callback for mouse button released events
// 	MouseDraggedEventCallbackFn MouseDraggedEventCallback;					///< Callback for mouse dragged events
// #pragma endregion
// };



// }

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
	void* getUserData() const;
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

}

#endif //CE_RENDER_VIEW_MAC_COCOAVIEW_HPP
