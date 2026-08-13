//
// Module: CelestialEngine/Tests/Engine/Modules/Native/Apple/MetalCpp/AppKit
// File: NsWindowEventDispatcherTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Apple/MetalCpp/AppKit/NsWindowEventDispatcher.hpp>

#include <Tools/Log/Log.hpp>

#include <gtest/gtest.h>

using CE::Native::NsWindowEventDispatcher;

namespace {

/**
 * @brief Listener recording the native notification pointer it was handed
 */
class NotificationListener {
public:
	void OnNotification(const NS::Notification* notification) {
		++calls;
		lastNotification = notification;
	}

public:
	int calls = 0;
	const NS::Notification* lastNotification = nullptr;
};

/**
 * @brief Opaque, never-dereferenced stand-in for a native notification
 */
NS::Notification* MakeFakeNotification() { return reinterpret_cast<NS::Notification*>(0xBEEF); }

/**
 * @brief Test fixture for NsWindowEventDispatcher tests
 * @details The native window forwards its AppKit notifications here; the Cocoa event hub is the single listener,
 *			and turns them into engine events (resize, focus, close).
 */
class NsWindowEventDispatcherTest: public ::testing::Test {
protected:
	void SetUp() override { CE::Tools::Log::Init(); }
	void TearDown() override { CE::Tools::Log::Shutdown(); }
};

}

/**
 * @brief Test that the resize notification is forwarded unchanged
 */
TEST_F(NsWindowEventDispatcherTest, DispatchWindowDidResize_ForwardsNotification) {
	NotificationListener listener;
	NsWindowEventDispatcher dispatcher;
	dispatcher.nsWindowGeometryEvents.didResizeDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<NotificationListener, &NotificationListener::OnNotification>(&listener)
	);

	auto* notification = MakeFakeNotification();
	dispatcher.DispatchWindowDidResize(notification);

	EXPECT_EQ(listener.calls, 1);
	EXPECT_EQ(listener.lastNotification, notification);
}

/**
 * @brief Test that gaining and losing key status reach separate channels
 * @details The input state resets its held keys on focus loss, so the two notifications must never be conflated.
 */
TEST_F(NsWindowEventDispatcherTest, FocusNotifications_AreRoutedSeparately) {
	NotificationListener becameKey;
	NotificationListener resignedKey;

	NsWindowEventDispatcher dispatcher;
	dispatcher.nsWindowFocusEvents.didBecomeKeyDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<NotificationListener, &NotificationListener::OnNotification>(&becameKey));
	dispatcher.nsWindowFocusEvents.didResignKeyDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<NotificationListener, &NotificationListener::OnNotification>(&resignedKey));

	dispatcher.DispatchWindowDidBecomeKey(MakeFakeNotification());

	EXPECT_EQ(becameKey.calls, 1);
	EXPECT_EQ(resignedKey.calls, 0);

	dispatcher.DispatchWindowDidResignKey(MakeFakeNotification());

	EXPECT_EQ(becameKey.calls, 1);
	EXPECT_EQ(resignedKey.calls, 1);
}

/**
 * @brief Test that the window state notifications reach their own channels
 */
TEST_F(NsWindowEventDispatcherTest, StateNotifications_AreRoutedToTheirOwnChannel) {
	NotificationListener miniaturized;
	NotificationListener deminiaturized;
	NotificationListener enteredFullScreen;
	NotificationListener exitedFullScreen;

	NsWindowEventDispatcher dispatcher;
	dispatcher.nsWindowStateEvents.didMiniaturizeDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<NotificationListener, &NotificationListener::OnNotification>(&miniaturized));
	dispatcher.nsWindowStateEvents.didDeminiaturizeDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<NotificationListener, &NotificationListener::OnNotification>(&deminiaturized));
	dispatcher.nsWindowStateEvents.didEnterFullScreenDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<NotificationListener, &NotificationListener::OnNotification>(&enteredFullScreen));
	dispatcher.nsWindowStateEvents.didExitFullScreenDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<NotificationListener, &NotificationListener::OnNotification>(&exitedFullScreen));

	dispatcher.DispatchWindowDidMiniaturize(MakeFakeNotification());
	dispatcher.DispatchWindowDidEnterFullScreen(MakeFakeNotification());

	EXPECT_EQ(miniaturized.calls, 1);
	EXPECT_EQ(deminiaturized.calls, 0);
	EXPECT_EQ(enteredFullScreen.calls, 1);
	EXPECT_EQ(exitedFullScreen.calls, 0);
}

/**
 * @brief Test that the will-close notification is forwarded
 */
TEST_F(NsWindowEventDispatcherTest, DispatchWindowWillClose_ForwardsNotification) {
	NotificationListener listener;
	NsWindowEventDispatcher dispatcher;
	dispatcher.nsWindowLifecycleEvents.willCloseDispatcher.Bind(
		EventDelegate<const NS::Notification*>::FromMethod<NotificationListener, &NotificationListener::OnNotification>(&listener)
	);

	dispatcher.DispatchWindowWillClose(MakeFakeNotification());

	EXPECT_EQ(listener.calls, 1);
}

/**
 * @brief Test that dispatching without a bound listener is a no-op
 */
TEST_F(NsWindowEventDispatcherTest, Dispatch_WithoutListener_IsNoOp) {
	NsWindowEventDispatcher dispatcher;

	EXPECT_NO_THROW(dispatcher.DispatchWindowDidResize(MakeFakeNotification()));
	EXPECT_NO_THROW(dispatcher.DispatchWindowWillClose(MakeFakeNotification()));
}
