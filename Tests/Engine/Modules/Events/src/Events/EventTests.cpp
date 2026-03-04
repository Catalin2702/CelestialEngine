//
// Module: CelestialEngine/Tests/Engine/Modules/Events
// File: EventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <Events/ApplicationEvent.hpp>
#include <Events/I_Event.hpp>

#include <gtest/gtest.h>

using namespace CE::Events;

/**
 * @brief Test fixture for Event tests
 */
class EventTest: public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {}
};

// ============================================================================
// EventDispatcher Tests
// ============================================================================

/**
 * @brief Test that EventDispatcher dispatches to correct handler
 */
TEST_F(EventTest, EventDispatcher_Dispatch_CallsCorrectHandler) {
	WindowResizeEvent event{1920, 1080};
	bool handlerCalled = false;

	EventDispatcher dispatcher{event};
	dispatcher.Dispatch<WindowResizeEvent>([&handlerCalled]([[maybe_unused]] WindowResizeEvent& e) {
		handlerCalled = true;
		return false;
	});

	EXPECT_TRUE(handlerCalled);
}

/**
 * @brief Test that EventDispatcher does not call wrong handler
 */
TEST_F(EventTest, EventDispatcher_Dispatch_DoesNotCallWrongHandler) {
	WindowResizeEvent event{1920, 1080};
	bool wrongHandlerCalled = false;

	EventDispatcher dispatcher{event};
	dispatcher.Dispatch<WindowCloseEvent>([&wrongHandlerCalled]([[maybe_unused]] WindowCloseEvent& e) {
		wrongHandlerCalled = true;
		return false;
	});

	EXPECT_FALSE(wrongHandlerCalled);
}

/**
 * @brief Test that EventDispatcher sets handled flag when handler returns true
 */
TEST_F(EventTest, EventDispatcher_Dispatch_SetsHandledFlag) {
	WindowResizeEvent event{1920, 1080};

	EventDispatcher dispatcher{event};
	dispatcher.Dispatch<WindowResizeEvent>([]([[maybe_unused]] WindowResizeEvent& e) {
		return true; // Mark as handled
	});

	EXPECT_TRUE(event.IsHandled());
}

/**
 * @brief Test that EventDispatcher does not set handled flag when handler returns false
 */
TEST_F(EventTest, EventDispatcher_Dispatch_DoesNotSetHandledFlagWhenFalse) {
	WindowResizeEvent event{1920, 1080};

	EventDispatcher dispatcher{event};
	const auto handler = []([[maybe_unused]] WindowResizeEvent& e) {
		return false; // Not handled
	};
	dispatcher.Dispatch<WindowResizeEvent>(handler);

	EXPECT_FALSE(event.IsHandled());
}

/**
 * @brief Test that EventDispatcher passes correct event data to handler
 */
TEST_F(EventTest, EventDispatcher_Dispatch_PassesCorrectEventData) {
	constexpr unsigned int expectedWidth = 1920;
	constexpr unsigned int expectedHeight = 1080;
	WindowResizeEvent event{expectedWidth, expectedHeight};

	unsigned int receivedWidth = 0;
	unsigned int receivedHeight = 0;

	EventDispatcher dispatcher{event};
	const auto handler = [&](const WindowResizeEvent& e) {
		receivedWidth = e.GetWidth();
		receivedHeight = e.GetHeight();
		return false;
	};
	dispatcher.Dispatch<WindowResizeEvent>(handler);

	EXPECT_EQ(receivedWidth, expectedWidth);
	EXPECT_EQ(receivedHeight, expectedHeight);
}
