//
// Module: CelestialEngine/Engine/Tests/Runtime/Events
// File: EventMacrosTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include <Events/Events.hpp>

#include <type_traits>
#include <gtest/gtest.h>

using namespace CE::Events;

namespace {

// Mock Event Classes for testing
class TestWindowCloseEvent: public I_Event {
public:
	explicit TestWindowCloseEvent(const bool isMutable = true): I_Event(isMutable) {}

	EVENT_CLASS_TYPE(WindowClose)
	EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

class TestKeyPressedEvent: public I_Event {
public:
	explicit TestKeyPressedEvent(const bool isMutable = true): I_Event(isMutable) {}

	EVENT_CLASS_TYPE(KeyPressed)
	EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput)
};

}

/**
 * @brief Compilation tests for the event class macros (Events/EventMacros.hpp)
 * @details They were in the Define tests while the macros lived in Define, which could not compile them without Events.
 */
namespace EventMacrosTests {

// ============================================================================
// EVENT_CLASS_TYPE Macro Compilation Tests
// ============================================================================

TEST(EventMacrosTests, EVENT_CLASS_TYPE_Macro_GeneratesStaticMethod) {
	// Verify that GetStaticType() is callable and returns correct type
	const auto staticType = TestWindowCloseEvent::GetStaticType();
	EXPECT_EQ(staticType, EventType::WindowClose);

	// Verify that it's truly static (callable without instance)
	static_assert(std::is_same_v<
					  decltype(TestWindowCloseEvent::GetStaticType()),
					  EventType>,
		"GetStaticType should return EventType");
}

TEST(EventMacrosTests, EVENT_CLASS_TYPE_Macro_GeneratesVirtualMethod) {
	// Verify that GetEventType() is virtual and returns correct value
	const TestWindowCloseEvent event;
	EXPECT_EQ(event.GetEventType(), EventType::WindowClose);

	// Verify polymorphic behavior
	const auto* const basePtr = &event;
	EXPECT_EQ(basePtr->GetEventType(), EventType::WindowClose);
}

TEST(EventMacrosTests, EVENT_CLASS_TYPE_Macro_GeneratesGetName) {
	const TestWindowCloseEvent event;
	EXPECT_STREQ(event.GetName(), "WindowClose");

	const TestKeyPressedEvent keyEvent;
	EXPECT_STREQ(keyEvent.GetName(), "KeyPressed");
}

// ============================================================================
// EVENT_CLASS_CATEGORY Macro Compilation Tests
// ============================================================================

TEST(EventMacrosTests, EVENT_CLASS_CATEGORY_Macro_SingleCategory) {
	const TestWindowCloseEvent event;
	const auto flags = event.GetCategoryFlags();

	EXPECT_EQ(flags, EventCategory::EventCategoryApplication);
	EXPECT_TRUE(HasAnyFlags(flags, EventCategory::EventCategoryApplication));
	EXPECT_FALSE(HasAnyFlags(flags, EventCategory::EventCategoryInput));
}

TEST(EventMacrosTests, EVENT_CLASS_CATEGORY_Macro_MultipleCategories) {
	const TestKeyPressedEvent event;
	const auto flags = event.GetCategoryFlags();

	// Should have both Keyboard and Input categories
	EXPECT_TRUE(HasAnyFlags(flags, EventCategory::EventCategoryKeyboard));
	EXPECT_TRUE(HasAnyFlags(flags, EventCategory::EventCategoryInput));
	EXPECT_FALSE(HasAnyFlags(flags, EventCategory::EventCategoryApplication));

	// Verify exact flags value
	constexpr auto expectedFlags = EventCategory::EventCategoryKeyboard | EventCategory::EventCategoryInput;
	EXPECT_EQ(flags, expectedFlags);
}

// ============================================================================
// Type Trait Tests - Advanced Compile-Time Verification
// ============================================================================

TEST(EventMacrosTests, TypeTraits_EventClassHasRequiredMethods) {
	// Verify that EVENT_CLASS_TYPE macro generates expected method signatures

	// Check for GetStaticType
	static_assert(std::is_invocable_r_v<EventType, decltype(&TestWindowCloseEvent::GetStaticType)>,
		"GetStaticType should be callable and return EventType");

	// Check for GetEventType
	static_assert(std::is_invocable_r_v<EventType, decltype(&TestWindowCloseEvent::GetEventType), TestWindowCloseEvent>,
		"GetEventType should be callable on instance and return EventType");

	// Check for GetName
	static_assert(std::is_invocable_r_v<const char*, decltype(&TestWindowCloseEvent::GetName), TestWindowCloseEvent>,
		"GetName should be callable on instance and return const char*");

	// Check for GetCategoryFlags
	static_assert(std::is_invocable_r_v<EventCategory, decltype(&TestWindowCloseEvent::GetCategoryFlags), TestWindowCloseEvent>,
		"GetCategoryFlags should be callable on instance and return int");
}

}
