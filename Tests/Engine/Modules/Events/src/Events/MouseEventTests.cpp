//
// Module: CelestialEngine/Tests/Engine/Modules/Events
// File: MouseEventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Events/I_Event.hpp>
#include <Events/MouseEvent.hpp>
#include <Types/KeyCode/MouseButtonCode.hpp>

#include <gtest/gtest.h>

using namespace CE::Events;
using CE::Types::MouseButtonCode;

namespace {

/**
 * @brief Test fixture for MouseEvent tests
 */
class MouseEventTest: public ::testing::Test {};

}

// ============================================================================
// MouseButtonPressedEvent Tests
// ============================================================================

/**
 * @brief Test that MouseButtonPressedEvent stores button code correctly
 */
TEST_F(MouseEventTest, MouseButtonPressedEvent_Constructor_StoresButtonCode) {
	constexpr auto button = MouseButtonCode::Left;

	const MouseButtonPressedEvent event{button};

	EXPECT_EQ(event.GetMouseButton(), button);
}

/**
 * @brief Test that MouseButtonPressedEvent has correct event type and name
 */
TEST_F(MouseEventTest, MouseButtonPressedEvent_GetEventType_ReturnsCorrectType) {
	const MouseButtonPressedEvent event{MouseButtonCode::Left};

	EXPECT_EQ(event.GetEventType(), EventType::MouseButtonPressed);
	EXPECT_EQ(MouseButtonPressedEvent::GetStaticType(), EventType::MouseButtonPressed);
	EXPECT_STREQ(event.GetName(), "MouseButtonPressed");
}

/**
 * @brief Test that MouseButtonPressedEvent is in the Mouse, MouseButton and Input categories
 */
TEST_F(MouseEventTest, MouseButtonPressedEvent_IsInCategory_MouseAndInputCategories) {
	const MouseButtonPressedEvent event{MouseButtonCode::Left};

	EXPECT_TRUE(event.IsInCategory(EventCategoryMouse));
	EXPECT_TRUE(event.IsInCategory(EventCategoryMouseButton));
	EXPECT_TRUE(event.IsInCategory(EventCategoryInput));
	EXPECT_FALSE(event.IsInCategory(EventCategoryKeyboard));
}

/**
 * @brief Test that MouseButtonPressedEvent ToString contains the button name
 */
TEST_F(MouseEventTest, MouseButtonPressedEvent_ToString_ContainsButton) {
	const MouseButtonPressedEvent event{MouseButtonCode::Middle};

	EXPECT_NE(event.ToString().find(CE::Types::ToString(MouseButtonCode::Middle)), std::string::npos);
}

// ============================================================================
// MouseButtonReleasedEvent Tests
// ============================================================================

/**
 * @brief Test that MouseButtonReleasedEvent stores button code correctly
 */
TEST_F(MouseEventTest, MouseButtonReleasedEvent_Constructor_StoresButtonCode) {
	constexpr auto button = MouseButtonCode::Right;

	const MouseButtonReleasedEvent event{button};

	EXPECT_EQ(event.GetMouseButton(), button);
}

/**
 * @brief Test that MouseButtonReleasedEvent has correct event type and name
 */
TEST_F(MouseEventTest, MouseButtonReleasedEvent_GetEventType_ReturnsCorrectType) {
	const MouseButtonReleasedEvent event{MouseButtonCode::Right};

	EXPECT_EQ(event.GetEventType(), EventType::MouseButtonReleased);
	EXPECT_EQ(MouseButtonReleasedEvent::GetStaticType(), EventType::MouseButtonReleased);
	EXPECT_STREQ(event.GetName(), "MouseButtonReleased");
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
 * @brief Test that MouseMovedEvent has correct event type and name
 */
TEST_F(MouseEventTest, MouseMovedEvent_GetEventType_ReturnsCorrectType) {
	const MouseMovedEvent event{100.0f, 200.0f};

	EXPECT_EQ(event.GetEventType(), EventType::MouseMoved);
	EXPECT_EQ(MouseMovedEvent::GetStaticType(), EventType::MouseMoved);
	EXPECT_STREQ(event.GetName(), "MouseMoved");
}

/**
 * @brief Test that MouseMovedEvent is in the Mouse and Input categories but not MouseButton
 */
TEST_F(MouseEventTest, MouseMovedEvent_IsInCategory_MouseAndInputCategories) {
	const MouseMovedEvent event{0.0f, 0.0f};

	EXPECT_TRUE(event.IsInCategory(EventCategoryMouse));
	EXPECT_TRUE(event.IsInCategory(EventCategoryInput));
	EXPECT_FALSE(event.IsInCategory(EventCategoryMouseButton));
}

// ============================================================================
// MouseWheelScrolledEvent Tests
// ============================================================================

/**
 * @brief Test that MouseWheelScrolledEvent stores offsets correctly
 */
TEST_F(MouseEventTest, MouseWheelScrolledEvent_Constructor_StoresOffsets) {
	constexpr float xOffset = 1.5f;
	constexpr float yOffset = -2.0f;

	const MouseWheelScrolledEvent event{xOffset, yOffset};

	EXPECT_FLOAT_EQ(event.GetXOffset(), xOffset);
	EXPECT_FLOAT_EQ(event.GetYOffset(), yOffset);
}

/**
 * @brief Test that MouseWheelScrolledEvent has correct event type and name
 */
TEST_F(MouseEventTest, MouseWheelScrolledEvent_GetEventType_ReturnsCorrectType) {
	const MouseWheelScrolledEvent event{0.0f, 1.0f};

	EXPECT_EQ(event.GetEventType(), EventType::MouseScrolled);
	EXPECT_EQ(MouseWheelScrolledEvent::GetStaticType(), EventType::MouseScrolled);
	EXPECT_STREQ(event.GetName(), "MouseScrolled");
}

/**
 * @brief Test that MouseWheelScrolledEvent is in the Mouse and Input categories
 */
TEST_F(MouseEventTest, MouseWheelScrolledEvent_IsInCategory_MouseAndInputCategories) {
	const MouseWheelScrolledEvent event{0.0f, 1.0f};

	EXPECT_TRUE(event.IsInCategory(EventCategoryMouse));
	EXPECT_TRUE(event.IsInCategory(EventCategoryInput));
}

// ============================================================================
// MouseDraggedEvent Tests
// ============================================================================

/**
 * @brief Test that MouseDraggedEvent stores both the button and the cursor position
 */
TEST_F(MouseEventTest, MouseDraggedEvent_Constructor_StoresButtonAndPosition) {
	constexpr auto button = MouseButtonCode::Left;
	constexpr float x = 42.0f;
	constexpr float y = 84.0f;

	const MouseDraggedEvent event{button, x, y};

	EXPECT_EQ(event.GetMouseButton(), button);
	EXPECT_FLOAT_EQ(event.GetX(), x);
	EXPECT_FLOAT_EQ(event.GetY(), y);
}

/**
 * @brief Test that MouseDraggedEvent has correct event type and name
 */
TEST_F(MouseEventTest, MouseDraggedEvent_GetEventType_ReturnsCorrectType) {
	const MouseDraggedEvent event{MouseButtonCode::Left, 0.0f, 0.0f};

	EXPECT_EQ(event.GetEventType(), EventType::MouseDragged);
	EXPECT_EQ(MouseDraggedEvent::GetStaticType(), EventType::MouseDragged);
	EXPECT_STREQ(event.GetName(), "MouseDragged");
}

/**
 * @brief Test that MouseDraggedEvent is in the Mouse, MouseButton and Input categories
 * @details A drag carries a button, so unlike a plain move it also belongs to the MouseButton category.
 */
TEST_F(MouseEventTest, MouseDraggedEvent_IsInCategory_MouseButtonAndInputCategories) {
	const MouseDraggedEvent event{MouseButtonCode::Right, 1.0f, 2.0f};

	EXPECT_TRUE(event.IsInCategory(EventCategoryMouse));
	EXPECT_TRUE(event.IsInCategory(EventCategoryMouseButton));
	EXPECT_TRUE(event.IsInCategory(EventCategoryInput));
}

/**
 * @brief Test that MouseDraggedEvent ToString contains the button and the coordinates
 */
TEST_F(MouseEventTest, MouseDraggedEvent_ToString_ContainsButtonAndPosition) {
	const MouseDraggedEvent event{MouseButtonCode::Right, 12.0f, 34.0f};
	const std::string str = event.ToString();

	EXPECT_NE(str.find(CE::Types::ToString(MouseButtonCode::Right)), std::string::npos);
	EXPECT_NE(str.find("12"), std::string::npos);
	EXPECT_NE(str.find("34"), std::string::npos);
}

// ============================================================================
// Mutability Tests
// ============================================================================

/**
 * @brief Test that mouse events are mutable by default and can be consumed
 */
TEST_F(MouseEventTest, MouseEvents_AreMutableByDefault) {
	const MouseMovedEvent event{1.0f, 1.0f};

	EXPECT_TRUE(event.IsMutable());
	event.Consume();
	EXPECT_TRUE(event.IsHandled());
}

/**
 * @brief Test that immutable mouse events cannot be consumed
 */
TEST_F(MouseEventTest, MouseEvents_ImmutableCannotBeConsumed) {
	const MouseButtonPressedEvent event{MouseButtonCode::Left, false};

	EXPECT_FALSE(event.IsMutable());
	event.Consume();
	EXPECT_FALSE(event.IsHandled());
}
