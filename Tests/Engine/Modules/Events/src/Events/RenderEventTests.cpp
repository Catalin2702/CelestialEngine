//
// Module: CelestialEngine/Tests/Engine/Modules/Events
// File: RenderEventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include <Events/I_Event.hpp>
#include <Events/RenderEvent.hpp>

#include <gtest/gtest.h>

using namespace CE::Events;

namespace {

/**
 * @brief Test fixture for RenderEvent tests
 */
class RenderEventTest: public ::testing::Test {};

}

// ============================================================================
// VSyncEvent Tests
// ============================================================================

/**
 * @brief Test that VSyncEvent stores the enabled state
 */
TEST_F(RenderEventTest, VSyncEvent_Constructor_StoresEnabledState) {
	const VSyncEvent event{true};

	EXPECT_TRUE(event.GetState());
}

/**
 * @brief Test that VSyncEvent stores the disabled state
 */
TEST_F(RenderEventTest, VSyncEvent_Constructor_StoresDisabledState) {
	const VSyncEvent event{false};

	EXPECT_FALSE(event.GetState());
}

/**
 * @brief Test that VSyncEvent has correct event type and name
 */
TEST_F(RenderEventTest, VSyncEvent_GetEventType_ReturnsCorrectType) {
	const VSyncEvent event{true};

	EXPECT_EQ(event.GetEventType(), EventType::VSyncChange);
	EXPECT_EQ(VSyncEvent::GetStaticType(), EventType::VSyncChange);
	EXPECT_STREQ(event.GetName(), "VSyncChange");
}

/**
 * @brief Test that VSyncEvent is in the Render category only
 */
TEST_F(RenderEventTest, VSyncEvent_IsInCategory_RenderCategory) {
	const VSyncEvent event{true};

	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryRender));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryWindow));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryApplication));
}

/**
 * @brief Test that VSyncEvent ToString reports the new state
 */
TEST_F(RenderEventTest, VSyncEvent_ToString_ReportsState) {
	const VSyncEvent enabled{true};
	const VSyncEvent disabled{false};

	EXPECT_NE(enabled.ToString().find("true"), std::string::npos);
	EXPECT_NE(disabled.ToString().find("false"), std::string::npos);
}

/**
 * @brief Test that VSyncEvent is mutable by default and can be consumed
 */
TEST_F(RenderEventTest, VSyncEvent_IsMutableByDefault) {
	const VSyncEvent event{true};

	EXPECT_TRUE(event.IsMutable());
	event.Consume();
	EXPECT_TRUE(event.IsHandled());
}

/**
 * @brief Test that an immutable VSyncEvent cannot be consumed
 */
TEST_F(RenderEventTest, VSyncEvent_ImmutableCannotBeConsumed) {
	const VSyncEvent event{true, false};

	EXPECT_FALSE(event.IsMutable());
	event.Consume();
	EXPECT_FALSE(event.IsHandled());
}
