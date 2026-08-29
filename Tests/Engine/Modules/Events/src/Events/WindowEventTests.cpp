//
// Module: CelestialEngine/Tests/Engine/Modules/Events
// File: WindowEventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include <Events/I_Event.hpp>
#include <Events/WindowEvent.hpp>

#include <gtest/gtest.h>

#include <type_traits>

using namespace CE::Events;

namespace {

/**
 * @brief Test fixture for WindowEvent tests
 */
class WindowEventTest: public ::testing::Test {};

}

// ============================================================================
// WindowResizeEvent Tests
// ============================================================================

/**
 * @brief Test that WindowResizeEvent stores width and height correctly
 */
TEST_F(WindowEventTest, WindowResizeEvent_Constructor_StoresWidthAndHeight) {
	constexpr unsigned int width = 1920;
	constexpr unsigned int height = 1080;

	const WindowResizeEvent event{width, height};

	EXPECT_EQ(event.GetWidth(), width);
	EXPECT_EQ(event.GetHeight(), height);
}

/**
 * @brief Test that WindowResizeEvent has correct event type and name
 */
TEST_F(WindowEventTest, WindowResizeEvent_GetEventType_ReturnsCorrectType) {
	const WindowResizeEvent event{800, 600};

	EXPECT_EQ(event.GetEventType(), EventType::WindowResize);
	EXPECT_EQ(WindowResizeEvent::GetStaticType(), EventType::WindowResize);
	EXPECT_STREQ(event.GetName(), "WindowResize");
}

/**
 * @brief Test that WindowResizeEvent is in the Window category
 */
TEST_F(WindowEventTest, WindowResizeEvent_IsInCategory_WindowCategory) {
	const WindowResizeEvent event{800, 600};

	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryWindow));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryApplication));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryInput));
}

/**
 * @brief Test that WindowResizeEvent ToString contains the dimensions
 */
TEST_F(WindowEventTest, WindowResizeEvent_ToString_ContainsDimensions) {
	const WindowResizeEvent event{1024, 768};
	const std::string str = event.ToString();

	EXPECT_NE(str.find("1024"), std::string::npos);
	EXPECT_NE(str.find("768"), std::string::npos);
}

/**
 * @brief Test that a zero-sized resize (window miniaturized) is preserved as-is
 */
TEST_F(WindowEventTest, WindowResizeEvent_Constructor_AcceptsZeroSize) {
	const WindowResizeEvent event{0, 0};

	EXPECT_EQ(event.GetWidth(), 0u);
	EXPECT_EQ(event.GetHeight(), 0u);
}

/**
 * @brief Test that ViewResizeEvent is an alias of WindowResizeEvent
 * @details The Metal render context reports drawable-size changes as ViewResizeEvent; the alias keeps the hub
 *			signature readable without introducing a second type.
 */
TEST_F(WindowEventTest, ViewResizeEvent_IsAliasOfWindowResizeEvent) {
	static_assert(std::is_same_v<ViewResizeEvent, WindowResizeEvent>,
		"ViewResizeEvent must stay an alias of WindowResizeEvent");

	const ViewResizeEvent event{640, 480};

	EXPECT_EQ(event.GetEventType(), EventType::WindowResize);
	EXPECT_EQ(event.GetWidth(), 640u);
}

// ============================================================================
// WindowCloseEvent Tests
// ============================================================================

/**
 * @brief Test that WindowCloseEvent has correct event type and name
 */
TEST_F(WindowEventTest, WindowCloseEvent_GetEventType_ReturnsCorrectType) {
	const WindowCloseEvent event;

	EXPECT_EQ(event.GetEventType(), EventType::WindowClose);
	EXPECT_EQ(WindowCloseEvent::GetStaticType(), EventType::WindowClose);
	EXPECT_STREQ(event.GetName(), "WindowClose");
}

/**
 * @brief Test that WindowCloseEvent is in the Window category
 */
TEST_F(WindowEventTest, WindowCloseEvent_IsInCategory_WindowCategory) {
	const WindowCloseEvent event;

	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryWindow));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryApplication));
}

/**
 * @brief Test that WindowCloseEvent starts unhandled and can be consumed by the application
 */
TEST_F(WindowEventTest, WindowCloseEvent_CanBeConsumed) {
	const WindowCloseEvent event;

	EXPECT_FALSE(event.IsHandled());
	event.Consume();
	EXPECT_TRUE(event.IsHandled());
}

// ============================================================================
// WindowFocusEvent Tests
// ============================================================================

/**
 * @brief Test that WindowFocusEvent stores the focus-gained state
 */
TEST_F(WindowEventTest, WindowFocusEvent_Constructor_StoresFocusGained) {
	const WindowFocusEvent event{true};

	EXPECT_TRUE(event.IsFocused());
}

/**
 * @brief Test that WindowFocusEvent stores the focus-lost state
 */
TEST_F(WindowEventTest, WindowFocusEvent_Constructor_StoresFocusLost) {
	const WindowFocusEvent event{false};

	EXPECT_FALSE(event.IsFocused());
}

/**
 * @brief Test that WindowFocusEvent has correct event type and name
 */
TEST_F(WindowEventTest, WindowFocusEvent_GetEventType_ReturnsCorrectType) {
	const WindowFocusEvent event{true};

	EXPECT_EQ(event.GetEventType(), EventType::WindowFocus);
	EXPECT_EQ(WindowFocusEvent::GetStaticType(), EventType::WindowFocus);
	EXPECT_STREQ(event.GetName(), "WindowFocus");
}

/**
 * @brief Test that WindowFocusEvent is in the Window category
 */
TEST_F(WindowEventTest, WindowFocusEvent_IsInCategory_WindowCategory) {
	const WindowFocusEvent event{false};

	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryWindow));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryInput));
}

/**
 * @brief Test that WindowFocusEvent ToString reports the new focus state
 */
TEST_F(WindowEventTest, WindowFocusEvent_ToString_ReportsFocusState) {
	const WindowFocusEvent gained{true};
	const WindowFocusEvent lost{false};

	EXPECT_NE(gained.ToString().find("gained"), std::string::npos);
	EXPECT_NE(lost.ToString().find("lost"), std::string::npos);
}

// ============================================================================
// Mutability Tests
// ============================================================================

/**
 * @brief Test that window events are mutable by default
 */
TEST_F(WindowEventTest, WindowEvents_AreMutableByDefault) {
	const WindowResizeEvent resizeEvent{800, 600};
	const WindowCloseEvent closeEvent;
	const WindowFocusEvent focusEvent{true};

	EXPECT_TRUE(resizeEvent.IsMutable());
	EXPECT_TRUE(closeEvent.IsMutable());
	EXPECT_TRUE(focusEvent.IsMutable());
}

/**
 * @brief Test that immutable window events cannot be consumed
 */
TEST_F(WindowEventTest, WindowEvents_ImmutableCannotBeConsumed) {
	const WindowFocusEvent event{false, false};

	EXPECT_FALSE(event.IsMutable());
	event.Consume();
	EXPECT_FALSE(event.IsHandled());
}
