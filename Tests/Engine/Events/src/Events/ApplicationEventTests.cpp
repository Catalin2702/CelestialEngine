//
// Module: Tests/Engine/Events
// File: ApplicationEventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <gtest/gtest.h>

#include <Events/ApplicationEvent.hpp>
#include <Events/I_Event.hpp>

using namespace CE::Events;


/**
 * @brief Test fixture for ApplicationEvent tests
 */
class ApplicationEventTest: public ::testing::Test {
protected:
	void SetUp() override {}

	void TearDown() override {}
};

// ============================================================================
// WindowResizeEvent Tests
// ============================================================================

/**
 * @brief Test that WindowResizeEvent stores width and height correctly
 */
TEST_F(ApplicationEventTest, WindowResizeEvent_Constructor_StoresWidthAndHeight) {
	constexpr unsigned int width = 1920;
	constexpr unsigned int height = 1080;

	const WindowResizeEvent event{width, height};

	EXPECT_EQ(event.GetWidth(), width);
	EXPECT_EQ(event.GetHeight(), height);
}

/**
 * @brief Test that WindowResizeEvent has correct event type
 */
TEST_F(ApplicationEventTest, WindowResizeEvent_GetEventType_ReturnsCorrectType) {
	const WindowResizeEvent event{800, 600};

	EXPECT_EQ(event.GetEventType(), EventType::WindowResize);
}

/**
 * @brief Test that WindowResizeEvent is in Application category
 */
TEST_F(ApplicationEventTest, WindowResizeEvent_IsInCategory_ApplicationCategory) {
	const WindowResizeEvent event{800, 600};

	EXPECT_TRUE(event.IsInCategory(EventCategoryApplication));
	EXPECT_FALSE(event.IsInCategory(EventCategoryInput));
}

/**
 * @brief Test that WindowResizeEvent ToString contains dimensions
 */
TEST_F(ApplicationEventTest, WindowResizeEvent_ToString_ContainsDimensions) {
	const WindowResizeEvent event{1024, 768};
	const std::string str = event.ToString();

	EXPECT_NE(str.find("1024"), std::string::npos);
	EXPECT_NE(str.find("768"), std::string::npos);
}

// ============================================================================
// WindowCloseEvent Tests
// ============================================================================

/**
 * @brief Test that WindowCloseEvent has correct event type
 */
TEST_F(ApplicationEventTest, WindowCloseEvent_GetEventType_ReturnsCorrectType) {
	const WindowCloseEvent event;

	EXPECT_EQ(event.GetEventType(), EventType::WindowClose);
}

/**
 * @brief Test that WindowCloseEvent is in Application category
 */
TEST_F(ApplicationEventTest, WindowCloseEvent_IsInCategory_ApplicationCategory) {
	const WindowCloseEvent event;

	EXPECT_TRUE(event.IsInCategory(EventCategoryApplication));
}
