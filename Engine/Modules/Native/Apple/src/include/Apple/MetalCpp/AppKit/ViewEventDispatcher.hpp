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

#include "Utility/Callback/EventDispatcher.hpp"

namespace NS {

class ViewEventDispatcher: public I_ViewEventDispatcher {
	using EventListener = std::function<void(Event*)>;
	using VoidListener = std::function<void()>;

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
	CE::Utility::EventDispatcher<const Event*> mouseDownDelegate;
	CE::Utility::EventDispatcher<const Event*> mouseUpDelegate;
	CE::Utility::EventDispatcher<const Event*> mouseDraggedDelegate;
	CE::Utility::EventDispatcher<const Event*> rightMouseDownDelegate;
	CE::Utility::EventDispatcher<const Event*> rightMouseUpDelegate;
	CE::Utility::EventDispatcher<const Event*> rightMouseDraggedDelegate;
	CE::Utility::EventDispatcher<const Event*> otherMouseDownDelegate;
	CE::Utility::EventDispatcher<const Event*> otherMouseUpDelegate;
	CE::Utility::EventDispatcher<const Event*> otherMouseDraggedDelegate;
	CE::Utility::EventDispatcher<const Event*> mouseMovedDelegate;
	CE::Utility::EventDispatcher<const Event*> mouseEnteredDelegate;
	CE::Utility::EventDispatcher<const Event*> mouseExitedDelegate;
	CE::Utility::EventDispatcher<const Event*> keyDownDelegate;
	CE::Utility::EventDispatcher<const Event*> keyUpDelegate;
	CE::Utility::EventDispatcher<const Event*> flagsChangedDelegate;
	CE::Utility::EventDispatcher<const Event*> scrollWheelDelegate;

	CE::Utility::EventDispatcher<> viewDidMoveToWindowDelegate;
	CE::Utility::EventDispatcher<> viewDidMoveToSuperviewDelegate;
	CE::Utility::EventDispatcher<> viewDidLayoutDelegate;
	CE::Utility::EventDispatcher<> viewDidEndLiveResizeDelegate;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_VIEWEVENTDISPATCHER_HPP
