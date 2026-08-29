//
// Module: CelestialEngine/Tests/Engine/Modules/Events
// File: KeyEventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include <Events/I_Event.hpp>
#include <Events/KeyEvent.hpp>
#include <Types/KeyCode/KeyboardKeyCode.hpp>

#include <gtest/gtest.h>

using namespace CE::Events;
using CE::Types::KeyboardKeyCode;

namespace {

/**
 * @brief Test fixture for KeyEvent tests
 */
class KeyEventTest: public ::testing::Test {};

}

// ============================================================================
// KeyPressedEvent Tests
// ============================================================================

/**
 * @brief Test that KeyPressedEvent stores keycode correctly
 */
TEST_F(KeyEventTest, KeyPressedEvent_Constructor_StoresKeyCode) {
	constexpr auto keycode = KeyboardKeyCode::A;

	const KeyPressedEvent event{keycode, 0};

	EXPECT_EQ(event.GetKeyCode(), keycode);
}

/**
 * @brief Test that KeyPressedEvent stores repeat count correctly
 */
TEST_F(KeyEventTest, KeyPressedEvent_Constructor_StoresRepeatCount) {
	constexpr int repeatCount = 5;

	const KeyPressedEvent event{KeyboardKeyCode::A, repeatCount};

	EXPECT_EQ(event.GetRepeatCount(), repeatCount);
}

/**
 * @brief Test that KeyPressedEvent has correct event type and name
 */
TEST_F(KeyEventTest, KeyPressedEvent_GetEventType_ReturnsCorrectType) {
	const KeyPressedEvent event{KeyboardKeyCode::A, 0};

	EXPECT_EQ(event.GetEventType(), EventType::KeyPressed);
	EXPECT_EQ(KeyPressedEvent::GetStaticType(), EventType::KeyPressed);
	EXPECT_STREQ(event.GetName(), "KeyPressed");
}

/**
 * @brief Test that KeyPressedEvent is in both Keyboard and Input categories
 */
TEST_F(KeyEventTest, KeyPressedEvent_IsInCategory_KeyboardAndInputCategories) {
	const KeyPressedEvent event{KeyboardKeyCode::A, 0};

	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryKeyboard));
	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryInput));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryApplication));
	EXPECT_FALSE(event.IsInCategory(EventCategory::EventCategoryMouse));
}

/**
 * @brief Test that KeyPressedEvent ToString contains the key name and the repeat count
 */
TEST_F(KeyEventTest, KeyPressedEvent_ToString_ContainsKeyAndRepeatCount) {
	const KeyPressedEvent event{KeyboardKeyCode::Space, 3};
	const std::string str = event.ToString();

	EXPECT_NE(str.find(CE::Types::ToString(KeyboardKeyCode::Space)), std::string::npos);
	EXPECT_NE(str.find('3'), std::string::npos);
}

// ============================================================================
// KeyReleasedEvent Tests
// ============================================================================

/**
 * @brief Test that KeyReleasedEvent stores keycode correctly
 */
TEST_F(KeyEventTest, KeyReleasedEvent_Constructor_StoresKeyCode) {
	constexpr auto keycode = KeyboardKeyCode::Space;

	const KeyReleasedEvent event{keycode};

	EXPECT_EQ(event.GetKeyCode(), keycode);
}

/**
 * @brief Test that KeyReleasedEvent has correct event type and name
 */
TEST_F(KeyEventTest, KeyReleasedEvent_GetEventType_ReturnsCorrectType) {
	const KeyReleasedEvent event{KeyboardKeyCode::Space};

	EXPECT_EQ(event.GetEventType(), EventType::KeyReleased);
	EXPECT_EQ(KeyReleasedEvent::GetStaticType(), EventType::KeyReleased);
	EXPECT_STREQ(event.GetName(), "KeyReleased");
}

/**
 * @brief Test that KeyReleasedEvent is in both Keyboard and Input categories
 */
TEST_F(KeyEventTest, KeyReleasedEvent_IsInCategory_KeyboardAndInputCategories) {
	const KeyReleasedEvent event{KeyboardKeyCode::Escape};

	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryKeyboard));
	EXPECT_TRUE(event.IsInCategory(EventCategory::EventCategoryInput));
}

/**
 * @brief Test that KeyReleasedEvent ToString contains the key name
 */
TEST_F(KeyEventTest, KeyReleasedEvent_ToString_ContainsKey) {
	const KeyReleasedEvent event{KeyboardKeyCode::Escape};

	EXPECT_NE(event.ToString().find(CE::Types::ToString(KeyboardKeyCode::Escape)), std::string::npos);
}

// ============================================================================
// KeyTypedEvent Tests
// ============================================================================

/**
 * @brief Test that KeyTypedEvent stores the Unicode code point correctly
 * @details Unlike press/release events, KeyTypedEvent carries the character produced after keyboard layout
 *			processing, so its key code is a raw Unicode code point and not a KeyboardKeyCode.
 */
TEST_F(KeyEventTest, KeyTypedEvent_Constructor_StoresCodePoint) {
	constexpr KeyCharType codePoint = 'a';

	const KeyTypedEvent event{codePoint};

	EXPECT_EQ(event.GetKeyCode(), codePoint);
}

/**
 * @brief Test that KeyTypedEvent has correct event type and name
 */
TEST_F(KeyEventTest, KeyTypedEvent_GetEventType_ReturnsCorrectType) {
	const KeyTypedEvent event{'a'};

	EXPECT_EQ(event.GetEventType(), EventType::KeyTyped);
	EXPECT_EQ(KeyTypedEvent::GetStaticType(), EventType::KeyTyped);
	EXPECT_STREQ(event.GetName(), "KeyTyped");
}

/**
 * @brief Test that KeyTypedEvent handles a non-ASCII code point
 */
TEST_F(KeyEventTest, KeyTypedEvent_Constructor_StoresNonAsciiCodePoint) {
	constexpr KeyCharType codePoint = 0x00E8;	// 'è'

	const KeyTypedEvent event{codePoint};

	EXPECT_EQ(event.GetKeyCode(), codePoint);
}

// ============================================================================
// Mutability Tests
// ============================================================================

/**
 * @brief Test that key events are mutable by default and can be consumed
 */
TEST_F(KeyEventTest, KeyEvents_AreMutableByDefault) {
	const KeyPressedEvent event{KeyboardKeyCode::A, 0};

	EXPECT_TRUE(event.IsMutable());
	event.Consume();
	EXPECT_TRUE(event.IsHandled());
}

/**
 * @brief Test that immutable key events cannot be consumed
 */
TEST_F(KeyEventTest, KeyEvents_ImmutableCannotBeConsumed) {
	const KeyReleasedEvent event{KeyboardKeyCode::A, false};

	EXPECT_FALSE(event.IsMutable());
	event.Consume();
	EXPECT_FALSE(event.IsHandled());
}
