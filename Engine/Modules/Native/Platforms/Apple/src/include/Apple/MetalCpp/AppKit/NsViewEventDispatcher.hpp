//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/AppKit
// File: NsViewEventDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_NSVIEWEVENTDISPATCHER_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_NSVIEWEVENTDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

#include <AppKit/AppKit.hpp>

namespace CE::Native {

class CE_API NsViewEventDispatcher: public NS::I_ViewEventDispatcher {
public:
	struct CE_API MouseEvents {
		UnicastDispatcher<const NS::Event*> mouseDownDispatcher;
		UnicastDispatcher<const NS::Event*> mouseUpDispatcher;
		UnicastDispatcher<const NS::Event*> mouseDraggedDispatcher;
		UnicastDispatcher<const NS::Event*> rightMouseDownDispatcher;
		UnicastDispatcher<const NS::Event*> rightMouseUpDispatcher;
		UnicastDispatcher<const NS::Event*> rightMouseDraggedDispatcher;
		UnicastDispatcher<const NS::Event*> otherMouseDownDispatcher;
		UnicastDispatcher<const NS::Event*> otherMouseUpDispatcher;
		UnicastDispatcher<const NS::Event*> otherMouseDraggedDispatcher;
		UnicastDispatcher<const NS::Event*> mouseMovedDispatcher;
		UnicastDispatcher<const NS::Event*> mouseEnteredDispatcher;
		UnicastDispatcher<const NS::Event*> mouseExitedDispatcher;
		UnicastDispatcher<const NS::Event*> scrollWheelDispatcher;
	};

	struct CE_API KeyboardEvents {
		UnicastDispatcher<const NS::Event*> keyDownDispatcher;
		UnicastDispatcher<const NS::Event*> keyUpDispatcher;
		UnicastDispatcher<const NS::Event*> flagsChangedDispatcher;
	};

	struct CE_API ViewStateEvents {
		UnicastDispatcher<> viewDidMoveToWindowDispatcher;
		UnicastDispatcher<> viewDidMoveToSuperviewDispatcher;
		UnicastDispatcher<> viewDidLayoutDispatcher;
		UnicastDispatcher<> viewDidEndLiveResizeDispatcher;
	};

public:
	NsViewEventDispatcher() = default;

	~NsViewEventDispatcher() override = default;

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

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_NSVIEWEVENTDISPATCHER_HPP
