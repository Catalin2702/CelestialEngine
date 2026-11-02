//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Hub/Events
// File: EventHubDispatcherTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-05
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "TestEventHub.hpp"

#include <Tools/Tools.hpp>

#include <gtest/gtest.h>

#include <optional>

using CE::Core::I_EventHubDispatcher;

namespace Events = CE::Events;

namespace {

/**
 * @brief Subscriber counting the events it receives
 */
template <typename EventType>
class ChannelSubscriber {
public:
	void OnEvent(EventType& event) {
		++calls;
		lastEvent.emplace(event);
	}

public:
	int calls = 0;
	std::optional<EventType> lastEvent;
};

/**
 * @brief Test fixture exercising the hub through its interface rather than a backend
 * @details The per-backend tests cover the Receive* half - raw platform callback in, engine event out. This one
 *			covers the other half, which is shared: the Dispatch* methods are implemented once on
 *			I_EventHubDispatcher and reach their channel through the Get*EventHub accessors, so a subscriber never
 *			has to name a backend. The GLFW hub is used only as the concrete object; nothing here is GLFW-specific,
 *			and nothing initialises GLFW.
 */
class EventHubDispatcherTest: public ::testing::Test {
protected:
	void SetUp() override { CE::Tools::Log::Init(); }
	void TearDown() override { CE::Tools::Log::Shutdown(); }

	/// Held as the interface on purpose: every assertion below has to hold for any backend.
	[[nodiscard]] I_EventHubDispatcher& Hub() { return _hub; }

protected:
	CE::Tests::TestEventHubDispatcher _hub;
};

}

// ============================================================================
// Accessor Tests
// ============================================================================

/**
 * @brief Test that the accessors hand out the concrete hub's own channels
 * @details The whole point of them: a subscriber holding the interface reaches the same structs the backend holds
 *			by value, so it never has to downcast to subscribe.
 */
TEST_F(EventHubDispatcherTest, Accessors_HandOutTheConcreteHubsOwnChannels) {
	EXPECT_EQ(&Hub().GetApplicationEventHub(), &_hub.applicationEventHub);
	EXPECT_EQ(&Hub().GetKeyboardEventHub(), &_hub.keyboardEventHub);
	EXPECT_EQ(&Hub().GetMouseEventHub(), &_hub.mouseEventHub);
	EXPECT_EQ(&Hub().GetWindowEventHub(), &_hub.windowEventHub);
	EXPECT_EQ(&Hub().GetRenderEventHub(), &_hub.renderEventHub);
}

/**
 * @brief Test that the const and non-const accessors name the same object
 */
TEST_F(EventHubDispatcherTest, Accessors_ConstAndNonConstAgree) {
	const auto& constHub = static_cast<const I_EventHubDispatcher&>(_hub);

	EXPECT_EQ(&constHub.GetRenderEventHub(), &Hub().GetRenderEventHub());
	EXPECT_EQ(&constHub.GetWindowEventHub(), &Hub().GetWindowEventHub());
}

/**
 * @brief Test that the hub reports which backend feeds it
 * @details What a subscriber checks before reaching for a backend-only channel, instead of guessing at a downcast.
 */
TEST_F(EventHubDispatcherTest, ReportsTheBackendThatFeedsIt) {
	EXPECT_EQ(Hub().GetWindowApi(), CE::Tests::TestEventHubWindowApi);
}

// ============================================================================
// Dispatch Routing Tests
// ============================================================================

/**
 * @brief Test that the VSync change reaches the rendering channel
 * @details The channel this used to be called RenderContext, back when a context owned the swap behaviour. The
 *			route is what matters and it is unchanged: Dispatch on the interface, arrive on the hub the accessor
 *			hands out.
 */
TEST_F(EventHubDispatcherTest, DispatchRenderChangeVSyncEvent_ReachesTheRenderChannel) {
	ChannelSubscriber<Events::VSyncEvent> subscriber;
	Hub().GetRenderEventHub().onChangeVSyncDispatcher.Subscribe(
		EventDelegate<Events::VSyncEvent&>::FromMethod<ChannelSubscriber<Events::VSyncEvent>,
			&ChannelSubscriber<Events::VSyncEvent>::OnEvent>(&subscriber)
	);

	Events::VSyncEvent event{true};
	Hub().DispatchRenderChangeVSyncEvent(event);

	ASSERT_EQ(subscriber.calls, 1);
	ASSERT_TRUE(subscriber.lastEvent.has_value());
	EXPECT_TRUE(subscriber.lastEvent->GetState());
}

/**
 * @brief Test that a window close reaches the window channel and nothing else
 */
TEST_F(EventHubDispatcherTest, DispatchWindowCloseEvent_ReachesOnlyTheWindowChannel) {
	ChannelSubscriber<Events::WindowCloseEvent> closeSubscriber;
	ChannelSubscriber<Events::WindowResizeEvent> resizeSubscriber;

	Hub().GetWindowEventHub().onCloseMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowCloseEvent&>::FromMethod<ChannelSubscriber<Events::WindowCloseEvent>,
			&ChannelSubscriber<Events::WindowCloseEvent>::OnEvent>(&closeSubscriber)
	);
	Hub().GetWindowEventHub().onResizeMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowResizeEvent&>::FromMethod<ChannelSubscriber<Events::WindowResizeEvent>,
			&ChannelSubscriber<Events::WindowResizeEvent>::OnEvent>(&resizeSubscriber)
	);

	Events::WindowCloseEvent event{};
	Hub().DispatchWindowCloseEvent(event);

	EXPECT_EQ(closeSubscriber.calls, 1);
	EXPECT_EQ(resizeSubscriber.calls, 0);
}

/**
 * @brief Test that the four application lifecycle events stay on their own channels
 * @details They share a hub and differ only in type, which is exactly the shape where a copy-paste in the Dispatch
 *			implementations goes unnoticed.
 */
TEST_F(EventHubDispatcherTest, DispatchApplicationEvents_StayOnTheirOwnChannels) {
	ChannelSubscriber<Events::AppTickEvent> tick;
	ChannelSubscriber<Events::AppUpdateEvent> update;
	ChannelSubscriber<Events::AppRenderEvent> render;

	auto& hub = Hub().GetApplicationEventHub();
	hub.onTickMulticastDispatcher.Subscribe(
		EventDelegate<Events::AppTickEvent&>::FromMethod<ChannelSubscriber<Events::AppTickEvent>,
			&ChannelSubscriber<Events::AppTickEvent>::OnEvent>(&tick));
	hub.onUpdateMulticastDispatcher.Subscribe(
		EventDelegate<Events::AppUpdateEvent&>::FromMethod<ChannelSubscriber<Events::AppUpdateEvent>,
			&ChannelSubscriber<Events::AppUpdateEvent>::OnEvent>(&update));
	hub.onRenderMulticastDispatcher.Subscribe(
		EventDelegate<Events::AppRenderEvent&>::FromMethod<ChannelSubscriber<Events::AppRenderEvent>,
			&ChannelSubscriber<Events::AppRenderEvent>::OnEvent>(&render));

	Events::AppTickEvent tickEvent{0.016f};
	Events::AppUpdateEvent updateEvent{};

	Hub().DispatchAppTickEvent(tickEvent);
	Hub().DispatchAppUpdateEvent(updateEvent);

	EXPECT_EQ(tick.calls, 1);
	EXPECT_EQ(update.calls, 1);
	EXPECT_EQ(render.calls, 0);
}

/**
 * @brief Test that dispatching to a channel with no subscriber is silent
 * @details Every channel spends most of the application's life empty; a hub that only worked once something was
 *			listening would fail exactly during startup and shutdown.
 */
TEST_F(EventHubDispatcherTest, Dispatch_WithNoSubscriber_IsSilent) {
	Events::VSyncEvent vsync{false};
	Events::WindowCloseEvent close{};
	Events::AppUpdateEvent update{};

	EXPECT_NO_THROW(Hub().DispatchRenderChangeVSyncEvent(vsync));
	EXPECT_NO_THROW(Hub().DispatchWindowCloseEvent(close));
	EXPECT_NO_THROW(Hub().DispatchAppUpdateEvent(update));
}

// ============================================================================
// Factory Tests
// ============================================================================

/**
 * @brief Test that the factory builds a hub reporting the backend it was asked for
 */
TEST_F(EventHubDispatcherTest, Factory_BuildsTheHubForTheRequestedBackend) {
	const auto hub = I_EventHubDispatcher::MakeEventHubDispatcher(CE::Tests::TestEventHubWindowApi);

	ASSERT_NE(hub, nullptr);
	EXPECT_EQ(hub->GetWindowApi(), CE::Tests::TestEventHubWindowApi);
}

/**
 * @brief Test that a cursor mode change reaches the window channel and nothing else
 */
TEST_F(EventHubDispatcherTest, DispatchWindowCursorModeEvent_ReachesTheCursorModeChannel) {
	ChannelSubscriber<Events::WindowCursorModeEvent> cursorSubscriber;
	ChannelSubscriber<Events::WindowFocusEvent> focusSubscriber;

	Hub().GetWindowEventHub().onCursorModeMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowCursorModeEvent&>::FromMethod<ChannelSubscriber<Events::WindowCursorModeEvent>,
			&ChannelSubscriber<Events::WindowCursorModeEvent>::OnEvent>(&cursorSubscriber)
	);
	Hub().GetWindowEventHub().onFocusMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowFocusEvent&>::FromMethod<ChannelSubscriber<Events::WindowFocusEvent>,
			&ChannelSubscriber<Events::WindowFocusEvent>::OnEvent>(&focusSubscriber)
	);

	Events::WindowCursorModeEvent event{CE::Types::CursorMode::Captured};
	Hub().DispatchWindowCursorModeEvent(event);

	ASSERT_EQ(cursorSubscriber.calls, 1);
	ASSERT_TRUE(cursorSubscriber.lastEvent.has_value());
	EXPECT_TRUE(cursorSubscriber.lastEvent->IsCaptured());
	EXPECT_EQ(focusSubscriber.calls, 0);
}
