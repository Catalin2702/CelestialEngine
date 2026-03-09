//
// Module: CelestialEngine/Tests/Engine/Modules/Events
// File: KeyEventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-09
//

#include <Events/I_Event.hpp>
#include <Events/KeyEvent.hpp>
#include <Types/KeyCode/KeyboardKeyCode.hpp>

#include <gtest/gtest.h>

using namespace CE::Events;
using namespace CE::KeyCode;


/**
 * @brief Test fixture for KeyEvent tests
 */
class KeyEventTest: public ::testing::Test {};

// ============================================================================
// KeyPressedEvent Tests
// ============================================================================

/**
 * @brief Test that KeyPressedEvent stores keycode correctly
 */
TEST_F(KeyEventTest, KeyPressedEvent_Constructor_StoresKeyCode) {
	constexpr auto keycode = KeyboardKeyCode::A;
	constexpr int repeatCount = 0;

	const KeyPressedEvent event{keycode, repeatCount};

	EXPECT_EQ(event.GetKeyCode(), keycode);
}

/**
 * @brief Test that KeyPressedEvent stores repeat count correctly
 */
TEST_F(KeyEventTest, KeyPressedEvent_Constructor_StoresRepeatCount) {
	constexpr auto keycode = KeyboardKeyCode::A;
	constexpr int repeatCount = 5;

	const KeyPressedEvent event{keycode, repeatCount};

	EXPECT_EQ(event.GetRepeatCount(), repeatCount);
}

/**
 * @brief Test that KeyPressedEvent has correct event type
 */
TEST_F(KeyEventTest, KeyPressedEvent_GetEventType_ReturnsCorrectType) {
	const KeyPressedEvent event{KeyboardKeyCode::A, 0};

	EXPECT_EQ(event.GetEventType(), EventType::KeyPressed);
}

/**
 * @brief Test that KeyPressedEvent is in both Keyboard and Input categories
 */
TEST_F(KeyEventTest, KeyPressedEvent_IsInCategory_KeyboardAndInputCategories) {
	const KeyPressedEvent event{KeyboardKeyCode::A, 0};

	EXPECT_TRUE(event.IsInCategory(EventCategoryKeyboard));
	EXPECT_TRUE(event.IsInCategory(EventCategoryInput));
	EXPECT_FALSE(event.IsInCategory(EventCategoryApplication));
}

// ============================================================================
// KeyReleasedEvent Tests
// ============================================================================

/**
 * @brief Test that KeyReleasedEvent stores keycode correctly
 */
TEST_F(KeyEventTest, KeyReleasedEvent_Constructor_StoresKeyCode) {
	constexpr auto keycode = KeyboardKeyCode::Space; // Space key

	const KeyReleasedEvent event{keycode};

	EXPECT_EQ(event.GetKeyCode(), keycode);
}

/**
 * @brief Test that KeyReleasedEvent has correct event type
 */
TEST_F(KeyEventTest, KeyReleasedEvent_GetEventType_ReturnsCorrectType) {
	const KeyReleasedEvent event{KeyboardKeyCode::Space};

	EXPECT_EQ(event.GetEventType(), EventType::KeyReleased);
}

// ============================================================================
// KeyTypedEvent Tests
// ============================================================================

/**
 * @brief Test that KeyTypedEvent stores character code correctly
 */
TEST_F(KeyEventTest, KeyTypedEvent_Constructor_StoresCharCode) {
	constexpr auto charCode = KeyboardCharsCode::a;

	const KeyTypedEvent event{charCode};

	EXPECT_EQ(event.GetKeyCode(), charCode);
}

/**
 * @brief Test that KeyTypedEvent has correct event type
 */
TEST_F(KeyEventTest, KeyTypedEvent_GetEventType_ReturnsCorrectType) {
	const KeyTypedEvent event{KeyboardCharsCode::a};

	EXPECT_EQ(event.GetEventType(), EventType::KeyTyped);
}
