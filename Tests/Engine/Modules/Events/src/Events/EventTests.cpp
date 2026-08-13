//
// Module: CelestialEngine/Tests/Engine/Modules/Events
// File: EventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Events/ApplicationEvent.hpp>
#include <Events/I_Event.hpp>
#include <Events/KeyEvent.hpp>
#include <Events/MouseEvent.hpp>
#include <Events/WindowEvent.hpp>

#include <gtest/gtest.h>

#include <sstream>

using namespace CE::Events;

namespace {

/**
 * @brief Test fixture for the I_Event base contract
 * @details The engine no longer routes events through an EventDispatcher: raw platform callbacks are converted into
 *			event objects by the event hub, which multicasts them to subscribers. What is left on I_Event itself is the
 *			identification (type/name/category) and the handled/mutable contract, which is what this file covers.
 */
class EventTest: public ::testing::Test {};

}

// ============================================================================
// Identification Tests
// ============================================================================

/**
 * @brief Test that the static type matches the runtime type
 */
TEST_F(EventTest, GetStaticType_MatchesRuntimeType) {
	const WindowResizeEvent event{1920, 1080};

	EXPECT_EQ(WindowResizeEvent::GetStaticType(), EventType::WindowResize);
	EXPECT_EQ(event.GetEventType(), WindowResizeEvent::GetStaticType());
}

/**
 * @brief Test that the runtime type is reachable through a base-class reference
 */
TEST_F(EventTest, GetEventType_IsPolymorphic) {
	const WindowResizeEvent resizeEvent{1920, 1080};
	const KeyPressedEvent keyEvent{CE::Types::KeyboardKeyCode::A, 0};

	const I_Event& resizeBase = resizeEvent;
	const I_Event& keyBase = keyEvent;

	EXPECT_EQ(resizeBase.GetEventType(), EventType::WindowResize);
	EXPECT_EQ(keyBase.GetEventType(), EventType::KeyPressed);
}

/**
 * @brief Test that GetName returns the event type name
 */
TEST_F(EventTest, GetName_ReturnsEventTypeName) {
	const WindowCloseEvent closeEvent;
	const AppTickEvent tickEvent;

	EXPECT_STREQ(closeEvent.GetName(), "WindowClose");
	EXPECT_STREQ(tickEvent.GetName(), "AppTick");
}

// ============================================================================
// Category Tests
// ============================================================================

/**
 * @brief Test that IsInCategory matches a single category
 */
TEST_F(EventTest, IsInCategory_SingleCategory) {
	const WindowCloseEvent event;

	EXPECT_TRUE(event.IsInCategory(EventCategoryWindow));
	EXPECT_FALSE(event.IsInCategory(EventCategoryInput));
	EXPECT_FALSE(event.IsInCategory(EventCategoryApplication));
}

/**
 * @brief Test that IsInCategory matches every category of a combined mask
 */
TEST_F(EventTest, IsInCategory_CombinedCategories) {
	const MouseButtonPressedEvent event{CE::Types::MouseButtonCode::Left};

	EXPECT_TRUE(event.IsInCategory(EventCategoryMouse));
	EXPECT_TRUE(event.IsInCategory(EventCategoryMouseButton));
	EXPECT_TRUE(event.IsInCategory(EventCategoryInput));
	EXPECT_FALSE(event.IsInCategory(EventCategoryWindow));

	constexpr auto expectedFlags = EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput;
	EXPECT_EQ(event.GetCategoryFlags(), expectedFlags);
}

// ============================================================================
// Handled / Mutability Tests
// ============================================================================

/**
 * @brief Test that a freshly created event is not handled
 */
TEST_F(EventTest, IsHandled_InitiallyFalse) {
	const WindowResizeEvent event{800, 600};

	EXPECT_FALSE(event.IsHandled());
}

/**
 * @brief Test that a mutable event can be consumed
 */
TEST_F(EventTest, Consume_MutableEvent_MarksAsHandled) {
	const WindowResizeEvent event{800, 600};

	ASSERT_TRUE(event.IsMutable());
	event.Consume();

	EXPECT_TRUE(event.IsHandled());
}

/**
 * @brief Test that an immutable event can never be consumed
 * @details Immutable events (errors, for instance) are multicast to every subscriber: no handler may hide them
 *			from the subscribers that come after it.
 */
TEST_F(EventTest, Consume_ImmutableEvent_StaysUnhandled) {
	const WindowResizeEvent event{800, 600, false};

	ASSERT_FALSE(event.IsMutable());
	event.Consume();

	EXPECT_FALSE(event.IsHandled());
}

/**
 * @brief Test that consuming twice is idempotent
 */
TEST_F(EventTest, Consume_CalledTwice_StaysHandled) {
	const WindowCloseEvent event;

	event.Consume();
	event.Consume();

	EXPECT_TRUE(event.IsHandled());
}

/**
 * @brief Test that events default to mutable
 */
TEST_F(EventTest, IsMutable_DefaultsToTrue) {
	const WindowCloseEvent closeEvent;
	const AppUpdateEvent updateEvent;
	const MouseMovedEvent mouseEvent{1.0f, 2.0f};

	EXPECT_TRUE(closeEvent.IsMutable());
	EXPECT_TRUE(updateEvent.IsMutable());
	EXPECT_TRUE(mouseEvent.IsMutable());
}

// ============================================================================
// Formatting Tests
// ============================================================================

/**
 * @brief Test that ToString defaults to the event name
 */
TEST_F(EventTest, ToString_DefaultsToEventName) {
	const WindowCloseEvent event;

	EXPECT_EQ(event.ToString(), "WindowClose");
}

/**
 * @brief Test that the stream operator writes the ToString representation
 */
TEST_F(EventTest, StreamOperator_WritesToString) {
	const WindowResizeEvent event{1024, 768};

	std::ostringstream stream;
	stream << event;

	EXPECT_EQ(stream.str(), event.ToString());
}

/**
 * @brief Test that format_as forwards to ToString (used by the spdlog formatter)
 */
TEST_F(EventTest, FormatAs_ForwardsToToString) {
	const WindowResizeEvent event{1024, 768};

	EXPECT_EQ(format_as(event), event.ToString());
}
