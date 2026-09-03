//
// Module: CelestialEngine/Tests/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: NsViewEventDispatcherTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Apple/MetalCpp/AppKit/NsViewEventDispatcher.hpp>

#include <Tools/Tools.hpp>

#include <gtest/gtest.h>

using CE::Native::NsViewEventDispatcher;

namespace {

/**
 * @brief Listener recording the native event pointer it was handed
 * @details The dispatcher only forwards the pointer, it never dereferences it, so the tests can pass an opaque
 *			non-null address instead of a real NS::Event (which would require a running NSApplication).
 */
class NativeEventListener {
public:
	void OnEvent(const NS::Event* event) {
		++calls;
		lastEvent = event;
	}

	void OnStateChange() { ++calls; }

public:
	int calls = 0;
	const NS::Event* lastEvent = nullptr;
};

/**
 * @brief Opaque, never-dereferenced stand-in for a native event
 */
NS::Event* MakeFakeEvent() { return reinterpret_cast<NS::Event*>(0xF00D); }

/**
 * @brief Test fixture for NsViewEventDispatcher tests
 * @details This dispatcher is the entry point of the macOS input pipeline: the native view forwards each AppKit
 *			callback to it, and it fans the callback out to the single bound listener (the Cocoa event hub).
 */
class NsViewEventDispatcherTest: public ::testing::Test {
protected:
	void SetUp() override { CE::Tools::Log::Init(); }
	void TearDown() override { CE::Tools::Log::Shutdown(); }
};

}

// ============================================================================
// Mouse Event Routing Tests
// ============================================================================

/**
 * @brief Test that mouse-down is routed to the mouse-down listener with the original event
 */
TEST_F(NsViewEventDispatcherTest, DispatchMouseDown_ForwardsEventToListener) {
	NativeEventListener listener;
	NsViewEventDispatcher dispatcher;
	dispatcher.mouseEvents.mouseDownDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&listener)
	);

	auto* event = MakeFakeEvent();
	dispatcher.DispatchMouseDown(event);

	EXPECT_EQ(listener.calls, 1);
	EXPECT_EQ(listener.lastEvent, event);
}

/**
 * @brief Test that each mouse callback reaches its own channel and no other
 */
TEST_F(NsViewEventDispatcherTest, MouseCallbacks_AreRoutedToTheirOwnChannel) {
	NativeEventListener down;
	NativeEventListener up;
	NativeEventListener dragged;
	NativeEventListener moved;
	NativeEventListener scrolled;

	NsViewEventDispatcher dispatcher;
	dispatcher.mouseEvents.mouseDownDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&down));
	dispatcher.mouseEvents.mouseUpDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&up));
	dispatcher.mouseEvents.mouseDraggedDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&dragged));
	dispatcher.mouseEvents.mouseMovedDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&moved));
	dispatcher.mouseEvents.scrollWheelDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&scrolled));

	dispatcher.DispatchMouseUp(MakeFakeEvent());

	EXPECT_EQ(up.calls, 1);
	EXPECT_EQ(down.calls, 0);
	EXPECT_EQ(dragged.calls, 0);
	EXPECT_EQ(moved.calls, 0);
	EXPECT_EQ(scrolled.calls, 0);

	dispatcher.DispatchScrollWheel(MakeFakeEvent());

	EXPECT_EQ(scrolled.calls, 1);
	EXPECT_EQ(up.calls, 1);
}

/**
 * @brief Test that the right and other mouse buttons have their own channels
 */
TEST_F(NsViewEventDispatcherTest, RightAndOtherMouseCallbacks_AreRoutedSeparately) {
	NativeEventListener rightDown;
	NativeEventListener otherDown;

	NsViewEventDispatcher dispatcher;
	dispatcher.mouseEvents.rightMouseDownDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&rightDown));
	dispatcher.mouseEvents.otherMouseDownDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&otherDown));

	dispatcher.DispatchRightMouseDown(MakeFakeEvent());
	dispatcher.DispatchOtherMouseDown(MakeFakeEvent());
	dispatcher.DispatchOtherMouseDown(MakeFakeEvent());

	EXPECT_EQ(rightDown.calls, 1);
	EXPECT_EQ(otherDown.calls, 2);
}

/**
 * @brief Test that dispatching without a bound listener is a no-op
 */
TEST_F(NsViewEventDispatcherTest, Dispatch_WithoutListener_IsNoOp) {
	NsViewEventDispatcher dispatcher;

	EXPECT_NO_THROW(dispatcher.DispatchMouseDown(MakeFakeEvent()));
	EXPECT_NO_THROW(dispatcher.DispatchKeyDown(MakeFakeEvent()));
	EXPECT_NO_THROW(dispatcher.DispatchViewDidLayout());
}

// ============================================================================
// Keyboard Event Routing Tests
// ============================================================================

/**
 * @brief Test that key-down, key-up and flags-changed reach their own channels
 */
TEST_F(NsViewEventDispatcherTest, KeyboardCallbacks_AreRoutedToTheirOwnChannel) {
	NativeEventListener keyDown;
	NativeEventListener keyUp;
	NativeEventListener flagsChanged;

	NsViewEventDispatcher dispatcher;
	dispatcher.keyboardEvents.keyDownDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&keyDown));
	dispatcher.keyboardEvents.keyUpDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&keyUp));
	dispatcher.keyboardEvents.flagsChangedDispatcher.Bind(
		EventDelegate<const NS::Event*>::FromMethod<NativeEventListener, &NativeEventListener::OnEvent>(&flagsChanged));

	dispatcher.DispatchKeyDown(MakeFakeEvent());
	dispatcher.DispatchFlagsChanged(MakeFakeEvent());

	EXPECT_EQ(keyDown.calls, 1);
	EXPECT_EQ(keyUp.calls, 0);
	EXPECT_EQ(flagsChanged.calls, 1);
}

// ============================================================================
// View State Routing Tests
// ============================================================================

/**
 * @brief Test that the parameterless view-state callbacks reach their listeners
 */
TEST_F(NsViewEventDispatcherTest, ViewStateCallbacks_AreRoutedToTheirOwnChannel) {
	NativeEventListener didMoveToWindow;
	NativeEventListener didLayout;

	NsViewEventDispatcher dispatcher;
	dispatcher.stateEvents.viewDidMoveToWindowDispatcher.Bind(
		EventDelegate<>::FromMethod<NativeEventListener, &NativeEventListener::OnStateChange>(&didMoveToWindow));
	dispatcher.stateEvents.viewDidLayoutDispatcher.Bind(
		EventDelegate<>::FromMethod<NativeEventListener, &NativeEventListener::OnStateChange>(&didLayout));

	dispatcher.DispatchViewDidLayout();
	dispatcher.DispatchViewDidLayout();

	EXPECT_EQ(didLayout.calls, 2);
	EXPECT_EQ(didMoveToWindow.calls, 0);
}
