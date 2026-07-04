//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: ViewEventDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-04
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_VIEWEVENTDISPATCHER_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_VIEWEVENTDISPATCHER_HPP

#include <AppKit/AppKit.hpp>

#include "Utility/Callback/Dispatcher.hpp"

using EventDispatcher = CE::Utility::Dispatcher<const NS::Event*>;
using VoidDispatcher = CE::Utility::Dispatcher<>;

namespace NS {

class ViewEventDispatcher: public I_ViewEventDispatcher {
public:

	ViewEventDispatcher() = default;

	~ViewEventDispatcher() override = default;

public:
	void DispatchMouseDown(Event* event) override;
	void DispatchMouseUp(Event* event) override;
	void DispatchMouseDragged(Event* event) override;
	void DispatchRightMouseDown(Event* event) override;
	void DispatchRightMouseUp(Event* event) override;
	void DispatchRightMouseDragged(Event* event) override;
	void DispatchOtherMouseDown(Event* event) override;
	void DispatchOtherMouseUp(Event* event) override;
	void DispatchOtherMouseDragged(Event* event) override;
	void DispatchMouseMoved(Event* event) override;
	void DispatchMouseEntered(Event* event) override;
	void DispatchMouseExited(Event* event) override;
	void DispatchKeyDown(Event* event) override;
	void DispatchKeyUp(Event* event) override;
	void DispatchFlagsChanged(Event* event) override;
	void DispatchScrollWheel(Event* event) override;
	void DispatchViewDidMoveToWindow() override;
	void DispatchViewDidMoveToSuperview() override;
	void DispatchViewDidLayout() override;
	void DispatchViewDidEndLiveResize() override;

public:
	EventDispatcher mouseDownDispatcher;
	EventDispatcher mouseUpDispatcher;
	EventDispatcher mouseDraggedDispatcher;
	EventDispatcher rightMouseDownDispatcher;
	EventDispatcher rightMouseUpDispatcher;
	EventDispatcher rightMouseDraggedDispatcher;
	EventDispatcher otherMouseDownDispatcher;
	EventDispatcher otherMouseUpDispatcher;
	EventDispatcher otherMouseDraggedDispatcher;
	EventDispatcher mouseMovedDispatcher;
	EventDispatcher mouseEnteredDispatcher;
	EventDispatcher mouseExitedDispatcher;
	EventDispatcher keyDownDispatcher;
	EventDispatcher keyUpDispatcher;
	EventDispatcher flagsChangedDispatcher;
	EventDispatcher scrollWheelDispatcher;

	VoidDispatcher viewDidMoveToWindowDispatcher;
	VoidDispatcher viewDidMoveToSuperviewDispatcher;
	VoidDispatcher viewDidLayoutDispatcher;
	VoidDispatcher viewDidEndLiveResizeDispatcher;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_VIEWEVENTDISPATCHER_HPP
