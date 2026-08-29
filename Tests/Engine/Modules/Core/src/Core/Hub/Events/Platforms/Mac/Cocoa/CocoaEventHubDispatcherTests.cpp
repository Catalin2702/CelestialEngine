//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Hub/Events/Platforms/Mac/Cocoa
// File: CocoaEventHubDispatcherTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include <Core/Hub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp>

#include <Tools/Log/Log.hpp>
#include <Types/KeyCode/KeyboardKeyCode.hpp>
#include <Types/KeyCode/MouseButtonCode.hpp>

#include <gtest/gtest.h>

#include <optional>

using CE::Core::CocoaEventHubDispatcher;
using CE::Types::KeyboardKeyCode;
using CE::Types::MouseButtonCode;

namespace Events = CE::Events;

namespace {

/**
 * @brief Subscriber recording the engine events it receives from the hub
 */
template <typename EventType>
class HubSubscriber {
public:
	void OnEvent(EventType& event) {
		++calls;
		lastEventAddress = &event;
		lastEvent.emplace(event);
	}

	/**
	 * @brief Returns the copy of the last event received
	 * @details The hub builds its events on the stack and they die with the dispatch, so the subscriber keeps a
	 *			copy of the payload and only the address of the original for identity checks.
	 */
	[[nodiscard]] const EventType& Last() const { return *lastEvent; }

public:
	int calls = 0;
	std::optional<EventType> lastEvent;
	const void* lastEventAddress = nullptr;
};

/**
 * @brief Subscribes a HubSubscriber to a hub channel
 */
template <typename EventType, typename Channel>
u32 SubscribeTo(Channel& channel, HubSubscriber<EventType>& subscriber) {
	return channel.Subscribe(
		EventDelegate<EventType&>::template FromMethod<HubSubscriber<EventType>, &HubSubscriber<EventType>::OnEvent>(&subscriber)
	);
}

/**
 * @brief Test fixture for the Cocoa event hub
 * @details Only the callbacks that do not carry a native NS::Event / NS::Notification payload can be exercised
 *			without a running NSApplication: the ones that decode a native event are covered by the app itself.
 *			What is tested here is the translation into engine events and the fan-out to the subscribers.
 */
class CocoaEventHubDispatcherTest: public ::testing::Test {
protected:
	void SetUp() override { CE::Tools::Log::Init(); }
	void TearDown() override { CE::Tools::Log::Shutdown(); }

protected:
	CocoaEventHubDispatcher _hub;
};

}

// ============================================================================
// Window Translation Tests
// ============================================================================

/**
 * @brief Test that a window resize becomes a WindowResizeEvent with the new size
 */
TEST_F(CocoaEventHubDispatcherTest, ReceiveWindowResizeEvent_ProducesWindowResizeEvent) {
	HubSubscriber<Events::WindowResizeEvent> subscriber;
	SubscribeTo(_hub.cocoaWindowEventHub.onResizeMulticastDispatcher, subscriber);

	_hub.ReceiveWindowResizeEvent(1440, 900);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetWidth(), 1440u);
	EXPECT_EQ(subscriber.Last().GetHeight(), 900u);
}

/**
 * @brief Test that the will-close notification becomes an immutable WindowCloseEvent
 * @details AppKit is already tearing the window down when it fires, so no subscriber may consume the event and
 *			pretend the window is still alive.
 */
TEST_F(CocoaEventHubDispatcherTest, ReceiveWindowWillCloseEvent_ProducesImmutableWindowCloseEvent) {
	HubSubscriber<Events::WindowCloseEvent> subscriber;
	SubscribeTo(_hub.cocoaWindowEventHub.onCloseMulticastDispatcher, subscriber);

	_hub.ReceiveWindowWillCloseEvent(nullptr);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_FALSE(subscriber.Last().IsMutable());

	subscriber.Last().Consume();
	EXPECT_FALSE(subscriber.Last().IsHandled());
}

/**
 * @brief Test that becoming and resigning key window produce the matching focus states
 */
TEST_F(CocoaEventHubDispatcherTest, KeyWindowNotifications_ProduceMatchingFocusEvents) {
	HubSubscriber<Events::WindowFocusEvent> subscriber;
	SubscribeTo(_hub.cocoaWindowEventHub.onFocusMulticastDispatcher, subscriber);

	_hub.ReceiveWindowDidBecomeKeyEvent(nullptr);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_TRUE(subscriber.Last().IsFocused());

	_hub.ReceiveWindowDidResignKeyEvent(nullptr);

	ASSERT_EQ(subscriber.calls, 2);
	EXPECT_FALSE(subscriber.Last().IsFocused());
}

/**
 * @brief Test that a window error becomes an immutable ErrorEvent carrying code and description
 */
TEST_F(CocoaEventHubDispatcherTest, ReceiveWindowErrorEvent_ProducesImmutableErrorEvent) {
	HubSubscriber<Events::ErrorEvent> subscriber;
	SubscribeTo(_hub.cocoaWindowEventHub.onErrorMulticastDispatcher, subscriber);

	_hub.ReceiveWindowErrorEvent(7, "Cocoa window error");

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetErrorCode(), 7);
	EXPECT_STREQ(subscriber.Last().GetDescription(), "Cocoa window error");
	EXPECT_FALSE(subscriber.Last().IsMutable());
}

// ============================================================================
// Render Context Translation Tests
// ============================================================================

/**
 * @brief Test that a VSync change becomes a VSyncEvent carrying the new state
 */
TEST_F(CocoaEventHubDispatcherTest, ReceiveContextChangeVSyncEvent_ProducesVSyncEvent) {
	HubSubscriber<Events::VSyncEvent> subscriber;
	SubscribeTo(_hub.metalRenderContextEventHub.onChangeVSyncDispatcher, subscriber);

	_hub.ReceiveContextChangeVSyncEvent(false);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_FALSE(subscriber.Last().GetState());
}

/**
 * @brief Test that a drawable-size change becomes a ViewResizeEvent
 * @details The Metal drawable size is reported in floating point; the event exposes it as integer pixels.
 */
TEST_F(CocoaEventHubDispatcherTest, ReceiveContextResizeViewEvent_ProducesViewResizeEvent) {
	HubSubscriber<Events::ViewResizeEvent> subscriber;
	SubscribeTo(_hub.metalRenderContextEventHub.onResizeViewDispatcher, subscriber);

	_hub.ReceiveContextResizeViewEvent(1280.0, 720.0);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetWidth(), 1280u);
	EXPECT_EQ(subscriber.Last().GetHeight(), 720u);
}

/**
 * @brief Test that the view-resize channel is distinct from the window-resize one
 * @details The Metal view and the Cocoa window are resized by different notifications and their subscribers differ,
 *			so the two channels must not be conflated.
 */
TEST_F(CocoaEventHubDispatcherTest, ViewResizeAndWindowResize_AreDistinctChannels) {
	HubSubscriber<Events::ViewResizeEvent> viewSubscriber;
	HubSubscriber<Events::WindowResizeEvent> windowSubscriber;
	SubscribeTo(_hub.metalRenderContextEventHub.onResizeViewDispatcher, viewSubscriber);
	SubscribeTo(_hub.cocoaWindowEventHub.onResizeMulticastDispatcher, windowSubscriber);

	_hub.ReceiveContextResizeViewEvent(800.0, 600.0);

	EXPECT_EQ(viewSubscriber.calls, 1);
	EXPECT_EQ(windowSubscriber.calls, 0);

	_hub.ReceiveWindowResizeEvent(800, 600);

	EXPECT_EQ(viewSubscriber.calls, 1);
	EXPECT_EQ(windowSubscriber.calls, 1);
}

// ============================================================================
// Application Translation Tests
// ============================================================================

/**
 * @brief Test that the application callbacks reach their own channels
 */
TEST_F(CocoaEventHubDispatcherTest, ApplicationCallbacks_AreRoutedToTheirOwnChannel) {
	HubSubscriber<Events::AppTickEvent> tick;
	HubSubscriber<Events::AppUpdateEvent> update;
	HubSubscriber<Events::AppRenderEvent> render;
	SubscribeTo(_hub.cocoaApplicationEventHub.onTickMulticastDispatcher, tick);
	SubscribeTo(_hub.cocoaApplicationEventHub.onUpdateMulticastDispatcher, update);
	SubscribeTo(_hub.cocoaApplicationEventHub.onRenderMulticastDispatcher, render);

	_hub.ReceiveAppTickEvent();
	_hub.ReceiveAppRenderEvent();
	_hub.ReceiveAppRenderEvent();

	EXPECT_EQ(tick.calls, 1);
	EXPECT_EQ(update.calls, 0);
	EXPECT_EQ(render.calls, 2);
}

/**
 * @brief Test that an application error becomes an ErrorEvent on the application channel
 */
TEST_F(CocoaEventHubDispatcherTest, ReceiveAppErrorEvent_ProducesErrorEvent) {
	HubSubscriber<Events::ErrorEvent> appSubscriber;
	HubSubscriber<Events::ErrorEvent> windowSubscriber;
	SubscribeTo(_hub.cocoaApplicationEventHub.onErrorMulticastDispatcher, appSubscriber);
	SubscribeTo(_hub.cocoaWindowEventHub.onErrorMulticastDispatcher, windowSubscriber);

	_hub.ReceiveAppErrorEvent(3, "Cocoa application error");

	ASSERT_EQ(appSubscriber.calls, 1);
	EXPECT_EQ(appSubscriber.Last().GetErrorCode(), 3);
	EXPECT_EQ(windowSubscriber.calls, 0);
}

// ============================================================================
// Fan-out Tests
// ============================================================================

/**
 * @brief Test that every subscriber of a channel receives the same event instance
 */
TEST_F(CocoaEventHubDispatcherTest, Dispatch_FansOutTheSameEventToEverySubscriber) {
	HubSubscriber<Events::WindowResizeEvent> first;
	HubSubscriber<Events::WindowResizeEvent> second;
	SubscribeTo(_hub.cocoaWindowEventHub.onResizeMulticastDispatcher, first);
	SubscribeTo(_hub.cocoaWindowEventHub.onResizeMulticastDispatcher, second);

	_hub.ReceiveWindowResizeEvent(320, 240);

	ASSERT_EQ(first.calls, 1);
	ASSERT_EQ(second.calls, 1);
	EXPECT_EQ(first.lastEventAddress, second.lastEventAddress);
}

/**
 * @brief Test that a channel without subscribers swallows the callback silently
 */
TEST_F(CocoaEventHubDispatcherTest, Receive_WithoutSubscribers_IsNoOp) {
	EXPECT_NO_THROW(_hub.ReceiveWindowResizeEvent(100, 100));
	EXPECT_NO_THROW(_hub.ReceiveWindowWillCloseEvent(nullptr));
	EXPECT_NO_THROW(_hub.ReceiveContextChangeVSyncEvent(true));
}

/**
 * @brief Test that an unsubscribed listener stops receiving events from the hub
 */
TEST_F(CocoaEventHubDispatcherTest, Unsubscribe_StopsDeliveringEvents) {
	HubSubscriber<Events::WindowFocusEvent> subscriber;
	const auto handle = SubscribeTo(_hub.cocoaWindowEventHub.onFocusMulticastDispatcher, subscriber);

	_hub.ReceiveWindowDidBecomeKeyEvent(nullptr);
	_hub.cocoaWindowEventHub.onFocusMulticastDispatcher.Unsubscribe(handle);
	_hub.ReceiveWindowDidBecomeKeyEvent(nullptr);

	EXPECT_EQ(subscriber.calls, 1);
}
