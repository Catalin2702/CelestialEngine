//
// Module: Tests/Engine/Modules/Events
// File: MouseEventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <Events/I_Event.hpp>
#include <Events/MouseEvent.hpp>

#include <gtest/gtest.h>

using namespace CE::Events;


/**
 * @brief Test fixture for MouseEvent tests
 */
class MouseEventTest: public ::testing::Test {
protected:
	void SetUp() override {
		// Setup code if needed
	}

	void TearDown() override {
		// Cleanup code if needed
	}
};

// ============================================================================
// MouseButtonEvent Tests
// ============================================================================

/**
 * @brief Test that MouseButtonPressedEvent stores button code correctly
 */
TEST_F(MouseEventTest, MouseButtonPressedEvent_Constructor_StoresButtonCode) {
	constexpr int button = 0; // Left mouse button

	const MouseButtonPressedEvent event{button};

	EXPECT_EQ(event.GetMouseButton(), button);
}

/**
 * @brief Test that MouseButtonPressedEvent has correct event type
 */
TEST_F(MouseEventTest, MouseButtonPressedEvent_GetEventType_ReturnsCorrectType) {
	const MouseButtonPressedEvent event{0};

	EXPECT_EQ(event.GetEventType(), EventType::MouseButtonPressed);
}

/**
 * @brief Test that MouseButtonPressedEvent is in Mouse and Input categories
 */
TEST_F(MouseEventTest, MouseButtonPressedEvent_IsInCategory_MouseAndInputCategories) {
	const MouseButtonPressedEvent event{0};

	EXPECT_TRUE(event.IsInCategory(EventCategoryMouse));
	EXPECT_TRUE(event.IsInCategory(EventCategoryInput));
	EXPECT_TRUE(event.IsInCategory(EventCategoryMouseButton));
}

/**
 * @brief Test that MouseButtonReleasedEvent stores button code correctly
 */
TEST_F(MouseEventTest, MouseButtonReleasedEvent_Constructor_StoresButtonCode) {
	constexpr int button = 1; // Right mouse button

	const MouseButtonReleasedEvent event{button};

	EXPECT_EQ(event.GetMouseButton(), button);
}

// ============================================================================
// MouseMovedEvent Tests
// ============================================================================

/**
 * @brief Test that MouseMovedEvent stores mouse position correctly
 */
TEST_F(MouseEventTest, MouseMovedEvent_Constructor_StoresPosition) {
	constexpr float x = 150.5f;
	constexpr float y = 200.3f;

	const MouseMovedEvent event{x, y};

	EXPECT_FLOAT_EQ(event.GetX(), x);
	EXPECT_FLOAT_EQ(event.GetY(), y);
}

/**
 * @brief Test that MouseMovedEvent has correct event type
 */
TEST_F(MouseEventTest, MouseMovedEvent_GetEventType_ReturnsCorrectType) {
	const MouseMovedEvent event{100.0f, 200.0f};

	EXPECT_EQ(event.GetEventType(), EventType::MouseMoved);
}

// ============================================================================
// MouseScrolledEvent Tests
// ============================================================================

/**
 * @brief Test that MouseScrolledEvent stores offset correctly
 */
TEST_F(MouseEventTest, MouseScrolledEvent_Constructor_StoresOffset) {
	constexpr float xOffset = 1.5f;
	constexpr float yOffset = -2.0f;

	const MouseScrolledEvent event{xOffset, yOffset};

	EXPECT_FLOAT_EQ(event.GetXOffset(), xOffset);
	EXPECT_FLOAT_EQ(event.GetYOffset(), yOffset);
}

/**
 * @brief Test that MouseScrolledEvent has correct event type
 */
TEST_F(MouseEventTest, MouseScrolledEvent_GetEventType_ReturnsCorrectType) {
	const MouseScrolledEvent event{0.0f, 1.0f};

	EXPECT_EQ(event.GetEventType(), EventType::MouseScrolled);
}
