//
// Module: CelestialEngine/Engine/Modules/Native/Platforms/Apple/MetalCpp/AppKit
// File: NsViewEventDispatcher.hpp
// Created by: Catalin Chirosca
// Created: 2026-07-03
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#pragma once

#ifndef CE_NATIVE_APPLE_METALCPP_APPKIT_NSVIEWEVENTDISPATCHER_HPP
#define CE_NATIVE_APPLE_METALCPP_APPKIT_NSVIEWEVENTDISPATCHER_HPP

#include "Define/DynamicLinker.hpp"

#include "Utility/Delegate/Dispatcher.hpp"

#include <AppKit/AppKit.hpp>


namespace CE::Native {

/**
 * @class NsViewEventDispatcher
 * @brief Forwards the native NSView input callbacks to per-event unicast dispatchers
 * @details Installed on the render view: every mouse/keyboard/view-state callback the view receives is re-emitted on
 *			the matching UnicastDispatcher, which the application binds to the event hub's Receive* methods. This is
 *			the single source of input events for the Metal/Cocoa backend.
 */
class CE_APPLE_API NsViewEventDispatcher: public NS::I_ViewEventDispatcher {
public:
	struct CE_APPLE_API MouseEvents {
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

	struct CE_APPLE_API KeyboardEvents {
		UnicastDispatcher<const NS::Event*> keyDownDispatcher;
		UnicastDispatcher<const NS::Event*> keyUpDispatcher;
		UnicastDispatcher<const NS::Event*> flagsChangedDispatcher;
	};

	struct CE_APPLE_API ViewStateEvents {
		UnicastDispatcher<> viewDidMoveToWindowDispatcher;
		UnicastDispatcher<> viewDidMoveToSuperviewDispatcher;
		UnicastDispatcher<> viewDidLayoutDispatcher;
		UnicastDispatcher<> viewDidEndLiveResizeDispatcher;
	};

public:
	// The user-declared destructor suppresses the implicit move constructor and move assignment, so a move of an owner
	// holding this dispatcher by value would silently fall back to a copy. They are re-declared here (memberwise: every
	// member is a nothrow-movable dispatcher). Copies stay deleted: this is a polymorphic type, and duplicating a hub
	// would deliver every event twice.
	NsViewEventDispatcher() = default;
	NsViewEventDispatcher(const NsViewEventDispatcher&) = delete;
	NsViewEventDispatcher(NsViewEventDispatcher&&) noexcept = default;
	~NsViewEventDispatcher() override = default;

	NsViewEventDispatcher& operator=(const NsViewEventDispatcher&) = delete;
	NsViewEventDispatcher& operator=(NsViewEventDispatcher&&) noexcept = default;

public:
	/**
	 * @brief Re-emits the native mouse down callback on its unicast dispatcher
	 */
	void DispatchMouseDown(NS::Event* event) override;
	/**
	 * @brief Re-emits the native mouse up callback on its unicast dispatcher
	 */
	void DispatchMouseUp(NS::Event* event) override;
	/**
	 * @brief Re-emits the native mouse dragged callback on its unicast dispatcher
	 */
	void DispatchMouseDragged(NS::Event* event) override;
	/**
	 * @brief Re-emits the native right mouse down callback on its unicast dispatcher
	 */
	void DispatchRightMouseDown(NS::Event* event) override;
	/**
	 * @brief Re-emits the native right mouse up callback on its unicast dispatcher
	 */
	void DispatchRightMouseUp(NS::Event* event) override;
	/**
	 * @brief Re-emits the native right mouse dragged callback on its unicast dispatcher
	 */
	void DispatchRightMouseDragged(NS::Event* event) override;
	/**
	 * @brief Re-emits the native other mouse down callback on its unicast dispatcher
	 */
	void DispatchOtherMouseDown(NS::Event* event) override;
	/**
	 * @brief Re-emits the native other mouse up callback on its unicast dispatcher
	 */
	void DispatchOtherMouseUp(NS::Event* event) override;
	/**
	 * @brief Re-emits the native other mouse dragged callback on its unicast dispatcher
	 */
	void DispatchOtherMouseDragged(NS::Event* event) override;
	/**
	 * @brief Re-emits the native mouse moved callback on its unicast dispatcher
	 */
	void DispatchMouseMoved(NS::Event* event) override;
	/**
	 * @brief Re-emits the native mouse entered callback on its unicast dispatcher
	 */
	void DispatchMouseEntered(NS::Event* event) override;
	/**
	 * @brief Re-emits the native mouse exited callback on its unicast dispatcher
	 */
	void DispatchMouseExited(NS::Event* event) override;
	/**
	 * @brief Re-emits the native scroll wheel callback on its unicast dispatcher
	 */
	void DispatchScrollWheel(NS::Event* event) override;

	/**
	 * @brief Re-emits the native key down callback on its unicast dispatcher
	 */
	void DispatchKeyDown(NS::Event* event) override;
	/**
	 * @brief Re-emits the native key up callback on its unicast dispatcher
	 */
	void DispatchKeyUp(NS::Event* event) override;
	/**
	 * @brief Re-emits the native flags changed callback on its unicast dispatcher
	 */
	void DispatchFlagsChanged(NS::Event* event) override;

	/**
	 * @brief Re-emits the native view did move to window callback on its unicast dispatcher
	 */
	void DispatchViewDidMoveToWindow() override;
	/**
	 * @brief Re-emits the native view did move to superview callback on its unicast dispatcher
	 */
	void DispatchViewDidMoveToSuperview() override;
	/**
	 * @brief Re-emits the native view did layout callback on its unicast dispatcher
	 */
	void DispatchViewDidLayout() override;
	/**
	 * @brief Re-emits the native view did end live resize callback on its unicast dispatcher
	 */
	void DispatchViewDidEndLiveResize() override;

public:
	MouseEvents mouseEvents;
	KeyboardEvents keyboardEvents;
	ViewStateEvents stateEvents;
};

}

#endif //CE_NATIVE_APPLE_METALCPP_APPKIT_NSVIEWEVENTDISPATCHER_HPP
