//
// Module: CelestialEngine/Tests/Engine/Modules/Events
// File: ApplicationEventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include <Events/ApplicationEvent.hpp>
#include <Events/I_Event.hpp>

#include <gtest/gtest.h>

using namespace CE::Events;

namespace {

/**
 * @brief Test fixture for ApplicationEvent tests
 */
class ApplicationEventTest: public ::testing::Test {};

}

// ============================================================================
// AppTickEvent Tests
// ============================================================================

/**
 * @brief Test that AppTickEvent has correct event type and name
 */
TEST_F(ApplicationEventTest, AppTickEvent_GetEventType_ReturnsCorrectType) {
	const AppTickEvent event{0.016f};

	EXPECT_EQ(event.GetEventType(), EventType::AppTick);
	EXPECT_EQ(AppTickEvent::GetStaticType(), EventType::AppTick);
	EXPECT_STREQ(event.GetName(), "AppTick");
}

/**
 * @brief Test that AppTickEvent is in the Application category only
 */
TEST_F(ApplicationEventTest, AppTickEvent_IsInCategory_ApplicationCategory) {
	const AppTickEvent event{0.016f};

	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryApplication));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryInput));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryWindow));
}

// ============================================================================
// AppUpdateEvent Tests
// ============================================================================

/**
 * @brief Test that AppUpdateEvent has correct event type and name
 */
TEST_F(ApplicationEventTest, AppUpdateEvent_GetEventType_ReturnsCorrectType) {
	const AppUpdateEvent event;

	EXPECT_EQ(event.GetEventType(), EventType::AppUpdate);
	EXPECT_EQ(AppUpdateEvent::GetStaticType(), EventType::AppUpdate);
	EXPECT_STREQ(event.GetName(), "AppUpdate");
}

/**
 * @brief Test that AppUpdateEvent is in the Application category
 */
TEST_F(ApplicationEventTest, AppUpdateEvent_IsInCategory_ApplicationCategory) {
	const AppUpdateEvent event;

	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryApplication));
}

// ============================================================================
// AppRenderEvent Tests
// ============================================================================

/**
 * @brief Test that AppRenderEvent has correct event type and name
 */
TEST_F(ApplicationEventTest, AppRenderEvent_GetEventType_ReturnsCorrectType) {
	const AppRenderEvent event;

	EXPECT_EQ(event.GetEventType(), EventType::AppRender);
	EXPECT_EQ(AppRenderEvent::GetStaticType(), EventType::AppRender);
	EXPECT_STREQ(event.GetName(), "AppRender");
}

/**
 * @brief Test that AppRenderEvent is in the Application category
 */
TEST_F(ApplicationEventTest, AppRenderEvent_IsInCategory_ApplicationCategory) {
	const AppRenderEvent event;

	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryApplication));
}

// ============================================================================
// Mutability Tests
// ============================================================================

/**
 * @brief Test that application events are mutable by default and can be consumed
 */
TEST_F(ApplicationEventTest, ApplicationEvents_AreMutableByDefault) {
	const AppTickEvent tickEvent{0.016f};
	const AppUpdateEvent updateEvent;
	const AppRenderEvent renderEvent;

	EXPECT_TRUE(tickEvent.IsMutable());
	EXPECT_TRUE(updateEvent.IsMutable());
	EXPECT_TRUE(renderEvent.IsMutable());

	tickEvent.Consume();
	EXPECT_TRUE(tickEvent.IsHandled());
}

/**
 * @brief Test that application events created as immutable cannot be consumed
 */
TEST_F(ApplicationEventTest, ApplicationEvents_ImmutableCannotBeConsumed) {
	const AppUpdateEvent event{false};

	EXPECT_FALSE(event.IsMutable());
	event.Consume();
	EXPECT_FALSE(event.IsHandled());
}
