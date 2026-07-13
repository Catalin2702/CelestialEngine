//
// Module: CelestialEngine/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: ViewEventDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_VIEWEVENTDISPATCHER_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_VIEWEVENTDISPATCHER_HPP

#include <AppKit/AppKit.hpp>

#include "Utility/Delegate/Dispatcher.hpp"

using NSEventDelegate = EventDelegate<const NS::Event*>;
using NSEventMulticastDispatcher = CE::Utility::MulticastDispatcher<const NS::Event*>;

namespace CE::Native {

class ViewEventDispatcher: public NS::I_ViewEventDispatcher {
public:
	struct MouseEvents {
		NSEventMulticastDispatcher mouseDownMulticastDispatcher;
		NSEventMulticastDispatcher mouseUpMulticastDispatcher;
		NSEventMulticastDispatcher mouseDraggedMulticastDispatcher;
		NSEventMulticastDispatcher rightMouseDownMulticastDispatcher;
		NSEventMulticastDispatcher rightMouseUpMulticastDispatcher;
		NSEventMulticastDispatcher rightMouseDraggedMulticastDispatcher;
		NSEventMulticastDispatcher otherMouseDownMulticastDispatcher;
		NSEventMulticastDispatcher otherMouseUpMulticastDispatcher;
		NSEventMulticastDispatcher otherMouseDraggedMulticastDispatcher;
		NSEventMulticastDispatcher mouseMovedMulticastDispatcher;
		NSEventMulticastDispatcher mouseEnteredMulticastDispatcher;
		NSEventMulticastDispatcher mouseExitedMulticastDispatcher;
		NSEventMulticastDispatcher scrollWheelMulticastDispatcher;
	};

	struct KeyboardEvents {
		NSEventMulticastDispatcher keyDownMulticastDispatcher;
		NSEventMulticastDispatcher keyUpMulticastDispatcher;
		NSEventMulticastDispatcher flagsChangedMulticastDispatcher;
	};

	struct ViewStateEvents {
		VoidMulticastEventDispatcher viewDidMoveToWindowMulticastDispatcher;
		VoidMulticastEventDispatcher viewDidMoveToSuperviewMulticastDispatcher;
		VoidMulticastEventDispatcher viewDidLayoutMulticastDispatcher;
		VoidMulticastEventDispatcher viewDidEndLiveResizeMulticastDispatcher;
	};

public:
	ViewEventDispatcher() = default;

	~ViewEventDispatcher() override = default;

public:
	void DispatchMouseDown(NS::Event* event) override;
	void DispatchMouseUp(NS::Event* event) override;
	void DispatchMouseDragged(NS::Event* event) override;
	void DispatchRightMouseDown(NS::Event* event) override;
	void DispatchRightMouseUp(NS::Event* event) override;
	void DispatchRightMouseDragged(NS::Event* event) override;
	void DispatchOtherMouseDown(NS::Event* event) override;
	void DispatchOtherMouseUp(NS::Event* event) override;
	void DispatchOtherMouseDragged(NS::Event* event) override;
	void DispatchMouseMoved(NS::Event* event) override;
	void DispatchMouseEntered(NS::Event* event) override;
	void DispatchMouseExited(NS::Event* event) override;
	void DispatchScrollWheel(NS::Event* event) override;

	void DispatchKeyDown(NS::Event* event) override;
	void DispatchKeyUp(NS::Event* event) override;
	void DispatchFlagsChanged(NS::Event* event) override;

	void DispatchViewDidMoveToWindow() override;
	void DispatchViewDidMoveToSuperview() override;
	void DispatchViewDidLayout() override;
	void DispatchViewDidEndLiveResize() override;

public:
	MouseEvents mouseEvents;
	KeyboardEvents keyboardEvents;
	ViewStateEvents stateEvents;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_VIEWEVENTDISPATCHER_HPP
