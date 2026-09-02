//
// Module: CelestialEngine/Tests/Engine/Modules/Core/Hub/Events/Platforms/Common/Glfw
// File: GlfwEventHubDispatcherTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include <Core/Hub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp>

#include <Tools/Log/Log.hpp>
#include <Types/KeyCode/KeyboardKeyCode.hpp>
#include <Types/KeyCode/MouseButtonCode.hpp>

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include <optional>
#include <string>

using CE::Core::GlfwEventHubDispatcher;
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
 * @brief Test fixture for the GLFW event hub
 * @details The hub is the translation layer between the platform and the engine: raw GLFW callbacks enter through
 *			the Receive* methods, are turned into engine events and multicast to the subscribers. None of that needs
 *			an initialized GLFW or a window, so the whole translation is testable headlessly.
 */
class GlfwEventHubDispatcherTest: public ::testing::Test {
protected:
	void SetUp() override { CE::Tools::Log::Init(); }
	void TearDown() override { CE::Tools::Log::Shutdown(); }

protected:
	GlfwEventHubDispatcher _hub;
};

}

// ============================================================================
// Keyboard Translation Tests
// ============================================================================

/**
 * @brief Test that a GLFW press becomes a KeyPressedEvent with the mapped key code
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveKeyEvent_Press_ProducesKeyPressedEvent) {
	HubSubscriber<Events::KeyPressedEvent> subscriber;
	_hub.keyboardEventHub.onPressedMulticastDispatcher.Subscribe(
		EventDelegate<Events::KeyPressedEvent&>::FromMethod<HubSubscriber<Events::KeyPressedEvent>,
			&HubSubscriber<Events::KeyPressedEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveKeyEvent(GLFW_KEY_A, GLFW_PRESS, 0, 0);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetKeyCode(), KeyboardKeyCode::A);
	EXPECT_EQ(subscriber.Last().GetRepeatCount(), 0);
}

/**
 * @brief Test that a GLFW repeat becomes a KeyPressedEvent carrying a non-zero repeat count
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveKeyEvent_Repeat_ProducesRepeatingKeyPressedEvent) {
	HubSubscriber<Events::KeyPressedEvent> subscriber;
	_hub.keyboardEventHub.onPressedMulticastDispatcher.Subscribe(
		EventDelegate<Events::KeyPressedEvent&>::FromMethod<HubSubscriber<Events::KeyPressedEvent>,
			&HubSubscriber<Events::KeyPressedEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveKeyEvent(GLFW_KEY_W, GLFW_REPEAT, 0, 0);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetKeyCode(), KeyboardKeyCode::W);
	EXPECT_GT(subscriber.Last().GetRepeatCount(), 0);
}

/**
 * @brief Test that a GLFW release becomes a KeyReleasedEvent and not a press
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveKeyEvent_Release_ProducesKeyReleasedEvent) {
	HubSubscriber<Events::KeyPressedEvent> pressed;
	HubSubscriber<Events::KeyReleasedEvent> released;
	_hub.keyboardEventHub.onPressedMulticastDispatcher.Subscribe(
		EventDelegate<Events::KeyPressedEvent&>::FromMethod<HubSubscriber<Events::KeyPressedEvent>,
			&HubSubscriber<Events::KeyPressedEvent>::OnEvent>(&pressed));
	_hub.keyboardEventHub.onReleasedMulticastDispatcher.Subscribe(
		EventDelegate<Events::KeyReleasedEvent&>::FromMethod<HubSubscriber<Events::KeyReleasedEvent>,
			&HubSubscriber<Events::KeyReleasedEvent>::OnEvent>(&released));

	_hub.ReceiveKeyEvent(GLFW_KEY_ESCAPE, GLFW_RELEASE, 0, 0);

	EXPECT_EQ(pressed.calls, 0);
	ASSERT_EQ(released.calls, 1);
	EXPECT_EQ(released.Last().GetKeyCode(), KeyboardKeyCode::Escape);
}

/**
 * @brief Test that a character callback becomes a KeyTypedEvent carrying the code point
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveCharEvent_ProducesKeyTypedEvent) {
	HubSubscriber<Events::KeyTypedEvent> subscriber;
	_hub.keyboardEventHub.onTypedMulticastDispatcher.Subscribe(
		EventDelegate<Events::KeyTypedEvent&>::FromMethod<HubSubscriber<Events::KeyTypedEvent>,
			&HubSubscriber<Events::KeyTypedEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveCharEvent('z');

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetKeyCode(), static_cast<unsigned int>('z'));
}

// ============================================================================
// Mouse Translation Tests
// ============================================================================

/**
 * @brief Test that a GLFW button press becomes a MouseButtonPressedEvent with the mapped button
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveMouseButtonEvent_Press_ProducesMouseButtonPressedEvent) {
	HubSubscriber<Events::MouseButtonPressedEvent> subscriber;
	_hub.mouseEventHub.onButtonPressedMulticastDispatcher.Subscribe(
		EventDelegate<Events::MouseButtonPressedEvent&>::FromMethod<HubSubscriber<Events::MouseButtonPressedEvent>,
			&HubSubscriber<Events::MouseButtonPressedEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveMouseButtonEvent(GLFW_MOUSE_BUTTON_RIGHT, GLFW_PRESS, 0);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetMouseButton(), MouseButtonCode::Right);
}

/**
 * @brief Test that a GLFW button release becomes a MouseButtonReleasedEvent
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveMouseButtonEvent_Release_ProducesMouseButtonReleasedEvent) {
	HubSubscriber<Events::MouseButtonReleasedEvent> subscriber;
	_hub.mouseEventHub.onButtonReleasedMulticastDispatcher.Subscribe(
		EventDelegate<Events::MouseButtonReleasedEvent&>::FromMethod<HubSubscriber<Events::MouseButtonReleasedEvent>,
			&HubSubscriber<Events::MouseButtonReleasedEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveMouseButtonEvent(GLFW_MOUSE_BUTTON_LEFT, GLFW_RELEASE, 0);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetMouseButton(), MouseButtonCode::Left);
}

/**
 * @brief Test that a cursor position callback becomes a MouseMovedEvent with the same coordinates
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveMousePositionEvent_ProducesMouseMovedEvent) {
	HubSubscriber<Events::MouseMovedEvent> subscriber;
	_hub.mouseEventHub.onMovedMulticastDispatcher.Subscribe(
		EventDelegate<Events::MouseMovedEvent&>::FromMethod<HubSubscriber<Events::MouseMovedEvent>,
			&HubSubscriber<Events::MouseMovedEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveMousePositionEvent(100.5, 200.25);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_FLOAT_EQ(subscriber.Last().GetX(), 100.5f);
	EXPECT_FLOAT_EQ(subscriber.Last().GetY(), 200.25f);
}

/**
 * @brief Test that a drag callback becomes a MouseDraggedEvent carrying both button and position
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveMouseDraggedEvent_ProducesMouseDraggedEvent) {
	HubSubscriber<Events::MouseDraggedEvent> subscriber;
	_hub.mouseEventHub.onDraggedMulticastDispatcher.Subscribe(
		EventDelegate<Events::MouseDraggedEvent&>::FromMethod<HubSubscriber<Events::MouseDraggedEvent>,
			&HubSubscriber<Events::MouseDraggedEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveMouseDraggedEvent(GLFW_MOUSE_BUTTON_LEFT, GLFW_PRESS, 0, 12.0, 34.0);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetMouseButton(), MouseButtonCode::Left);
	EXPECT_FLOAT_EQ(subscriber.Last().GetX(), 12.0f);
	EXPECT_FLOAT_EQ(subscriber.Last().GetY(), 34.0f);
}

/**
 * @brief Test that a scroll callback becomes a MouseWheelScrolledEvent with the same offsets
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveMouseWheelScrollEvent_ProducesMouseWheelScrolledEvent) {
	HubSubscriber<Events::MouseWheelScrolledEvent> subscriber;
	_hub.mouseEventHub.onWheelScrolledMulticastDispatcher.Subscribe(
		EventDelegate<Events::MouseWheelScrolledEvent&>::FromMethod<HubSubscriber<Events::MouseWheelScrolledEvent>,
			&HubSubscriber<Events::MouseWheelScrolledEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveMouseWheelScrollEvent(-1.5, 2.5);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_FLOAT_EQ(subscriber.Last().GetXOffset(), -1.5f);
	EXPECT_FLOAT_EQ(subscriber.Last().GetYOffset(), 2.5f);
}

// ============================================================================
// Window Translation Tests
// ============================================================================

/**
 * @brief Test that a resize callback becomes a WindowResizeEvent with the new size
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveWindowResizeEvent_ProducesWindowResizeEvent) {
	HubSubscriber<Events::WindowResizeEvent> subscriber;
	_hub.windowEventHub.onResizeMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowResizeEvent&>::FromMethod<HubSubscriber<Events::WindowResizeEvent>,
			&HubSubscriber<Events::WindowResizeEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveWindowResizeEvent(1600, 900);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetWidth(), 1600u);
	EXPECT_EQ(subscriber.Last().GetHeight(), 900u);
}

/**
 * @brief Test that a close callback becomes a WindowCloseEvent
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveWindowCloseEvent_ProducesWindowCloseEvent) {
	HubSubscriber<Events::WindowCloseEvent> subscriber;
	_hub.windowEventHub.onCloseMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowCloseEvent&>::FromMethod<HubSubscriber<Events::WindowCloseEvent>,
			&HubSubscriber<Events::WindowCloseEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveWindowCloseEvent();

	EXPECT_EQ(subscriber.calls, 1);
}

/**
 * @brief Test that the GLFW focus flag is translated into the engine focus state
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveWindowFocusEvent_TranslatesFocusFlag) {
	HubSubscriber<Events::WindowFocusEvent> subscriber;
	_hub.windowEventHub.onFocusMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowFocusEvent&>::FromMethod<HubSubscriber<Events::WindowFocusEvent>,
			&HubSubscriber<Events::WindowFocusEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveWindowFocusEvent(GLFW_TRUE);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_TRUE(subscriber.Last().IsFocused());

	_hub.ReceiveWindowFocusEvent(GLFW_FALSE);

	ASSERT_EQ(subscriber.calls, 2);
	EXPECT_FALSE(subscriber.Last().IsFocused());
}

/**
 * @brief Test that a window error becomes an immutable ErrorEvent carrying code and description
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveWindowErrorEvent_ProducesImmutableErrorEvent) {
	HubSubscriber<Events::ErrorEvent> subscriber;
	_hub.windowEventHub.onErrorMulticastDispatcher.Subscribe(
		EventDelegate<Events::ErrorEvent&>::FromMethod<HubSubscriber<Events::ErrorEvent>,
			&HubSubscriber<Events::ErrorEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveWindowErrorEvent(65543, "GLFW window error");

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_EQ(subscriber.Last().GetErrorCode(), 65543);
	EXPECT_STREQ(subscriber.Last().GetDescription(), "GLFW window error");
	EXPECT_FALSE(subscriber.Last().IsMutable());
}

// ============================================================================
// Application / Render Context Translation Tests
// ============================================================================

/**
 * @brief Test that the application tick, update and render callbacks reach their own channels
 */
TEST_F(GlfwEventHubDispatcherTest, ApplicationCallbacks_AreRoutedToTheirOwnChannel) {
	HubSubscriber<Events::AppTickEvent> tick;
	HubSubscriber<Events::AppUpdateEvent> update;
	HubSubscriber<Events::AppRenderEvent> render;

	_hub.applicationEventHub.onTickMulticastDispatcher.Subscribe(
		EventDelegate<Events::AppTickEvent&>::FromMethod<HubSubscriber<Events::AppTickEvent>,
			&HubSubscriber<Events::AppTickEvent>::OnEvent>(&tick));
	_hub.applicationEventHub.onUpdateMulticastDispatcher.Subscribe(
		EventDelegate<Events::AppUpdateEvent&>::FromMethod<HubSubscriber<Events::AppUpdateEvent>,
			&HubSubscriber<Events::AppUpdateEvent>::OnEvent>(&update));
	_hub.applicationEventHub.onRenderMulticastDispatcher.Subscribe(
		EventDelegate<Events::AppRenderEvent&>::FromMethod<HubSubscriber<Events::AppRenderEvent>,
			&HubSubscriber<Events::AppRenderEvent>::OnEvent>(&render));

	_hub.ReceiveAppTickEvent(0.016f);
	_hub.ReceiveAppUpdateEvent();
	_hub.ReceiveAppUpdateEvent();

	EXPECT_EQ(tick.calls, 1);
	EXPECT_EQ(update.calls, 2);
	EXPECT_EQ(render.calls, 0);
}

/**
 * @brief Test that a VSync change becomes a VSyncEvent carrying the new state
 */
TEST_F(GlfwEventHubDispatcherTest, ReceiveContextChangeVSyncEvent_ProducesVSyncEvent) {
	HubSubscriber<Events::VSyncEvent> subscriber;
	_hub.renderContextEventHub.onChangeVSyncDispatcher.Subscribe(
		EventDelegate<Events::VSyncEvent&>::FromMethod<HubSubscriber<Events::VSyncEvent>,
			&HubSubscriber<Events::VSyncEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveContextChangeVSyncEvent(true);

	ASSERT_EQ(subscriber.calls, 1);
	EXPECT_TRUE(subscriber.Last().GetState());
}

// ============================================================================
// Fan-out Tests
// ============================================================================

/**
 * @brief Test that every subscriber of a channel receives the same event instance
 * @details Sharing the instance is what makes consuming meaningful: a subscriber that consumes the event must be
 *			visible to the subscribers that come after it.
 */
TEST_F(GlfwEventHubDispatcherTest, Dispatch_FansOutTheSameEventToEverySubscriber) {
	HubSubscriber<Events::WindowResizeEvent> first;
	HubSubscriber<Events::WindowResizeEvent> second;

	_hub.windowEventHub.onResizeMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowResizeEvent&>::FromMethod<HubSubscriber<Events::WindowResizeEvent>,
			&HubSubscriber<Events::WindowResizeEvent>::OnEvent>(&first));
	_hub.windowEventHub.onResizeMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowResizeEvent&>::FromMethod<HubSubscriber<Events::WindowResizeEvent>,
			&HubSubscriber<Events::WindowResizeEvent>::OnEvent>(&second));

	_hub.ReceiveWindowResizeEvent(320, 240);

	ASSERT_EQ(first.calls, 1);
	ASSERT_EQ(second.calls, 1);
	EXPECT_EQ(first.lastEventAddress, second.lastEventAddress);
}

/**
 * @brief Test that a channel without subscribers swallows the callback silently
 */
TEST_F(GlfwEventHubDispatcherTest, Receive_WithoutSubscribers_IsNoOp) {
	EXPECT_NO_THROW(_hub.ReceiveWindowCloseEvent());
	EXPECT_NO_THROW(_hub.ReceiveKeyEvent(GLFW_KEY_A, GLFW_PRESS, 0, 0));
	EXPECT_NO_THROW(_hub.ReceiveMousePositionEvent(1.0, 2.0));
}

/**
 * @brief Test that an unsubscribed listener stops receiving events from the hub
 */
TEST_F(GlfwEventHubDispatcherTest, Unsubscribe_StopsDeliveringEvents) {
	HubSubscriber<Events::WindowCloseEvent> subscriber;
	const auto handle = _hub.windowEventHub.onCloseMulticastDispatcher.Subscribe(
		EventDelegate<Events::WindowCloseEvent&>::FromMethod<HubSubscriber<Events::WindowCloseEvent>,
			&HubSubscriber<Events::WindowCloseEvent>::OnEvent>(&subscriber)
	);

	_hub.ReceiveWindowCloseEvent();
	_hub.windowEventHub.onCloseMulticastDispatcher.Unsubscribe(handle);
	_hub.ReceiveWindowCloseEvent();

	EXPECT_EQ(subscriber.calls, 1);
}
