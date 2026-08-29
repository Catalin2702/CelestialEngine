//
// Module: CelestialEngine/Tests/Engine/Modules/Events
// File: GenericEventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include <Events/GenericEvent.hpp>
#include <Events/I_Event.hpp>

#include <gtest/gtest.h>

using namespace CE::Events;

namespace {

/**
 * @brief Test fixture for GenericEvent tests
 */
class GenericEventTest: public ::testing::Test {};

}

// ============================================================================
// ErrorEvent Tests
// ============================================================================

/**
 * @brief Test that ErrorEvent stores the code and the description
 */
TEST_F(GenericEventTest, ErrorEvent_Constructor_StoresCodeAndDescription) {
	constexpr int errorCode = 65543;
	constexpr auto description = "GLFW: Failed to create window";

	const ErrorEvent event{errorCode, description};

	EXPECT_EQ(event.GetErrorCode(), errorCode);
	EXPECT_STREQ(event.GetDescription(), description);
}

/**
 * @brief Test that ErrorEvent has correct event type and name
 */
TEST_F(GenericEventTest, ErrorEvent_GetEventType_ReturnsCorrectType) {
	const ErrorEvent event{0, "no error"};

	EXPECT_EQ(event.GetEventType(), EventType::Error);
	EXPECT_EQ(ErrorEvent::GetStaticType(), EventType::Error);
	EXPECT_STREQ(event.GetName(), "Error");
}

/**
 * @brief Test that ErrorEvent is in the Application category
 */
TEST_F(GenericEventTest, ErrorEvent_IsInCategory_ApplicationCategory) {
	const ErrorEvent event{1, "boom"};

	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryApplication));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryWindow));
}

/**
 * @brief Test that ErrorEvent ToString contains both the code and the description
 */
TEST_F(GenericEventTest, ErrorEvent_ToString_ContainsCodeAndDescription) {
	const ErrorEvent event{42, "something went wrong"};
	const std::string str = event.ToString();

	EXPECT_NE(str.find("42"), std::string::npos);
	EXPECT_NE(str.find("something went wrong"), std::string::npos);
}

/**
 * @brief Test that ErrorEvent is immutable by default
 * @details Errors are broadcast to every subscriber: no handler may consume one and hide it from the subscribers
 *			that come after it, so the event defaults to immutable.
 */
TEST_F(GenericEventTest, ErrorEvent_IsImmutableByDefault) {
	const ErrorEvent event{1, "boom"};

	EXPECT_FALSE(event.IsMutable());

	event.Consume();

	EXPECT_FALSE(event.IsHandled());
}

/**
 * @brief Test that an explicitly mutable ErrorEvent can be consumed
 */
TEST_F(GenericEventTest, ErrorEvent_ExplicitlyMutable_CanBeConsumed) {
	const ErrorEvent event{1, "boom", true};

	EXPECT_TRUE(event.IsMutable());

	event.Consume();

	EXPECT_TRUE(event.IsHandled());
}
