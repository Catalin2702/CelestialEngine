//
// Module: Tests/Engine/Events
// File: KeyEventTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-02
// Updated by: Catalin Chirosca
// Updated: 2026-03-02
//

#include <Events/I_Event.hpp>
#include <Events/KeyEvent.hpp>

#include <gtest/gtest.h>

using namespace CE::Events;


/**
 * @brief Test fixture for KeyEvent tests
 */
class KeyEventTest: public ::testing::Test {
protected:
	void SetUp() override {
		// Setup code if needed
	}

	void TearDown() override {
		// Cleanup code if needed
	}
};

// ============================================================================
// KeyPressedEvent Tests
// ============================================================================

/**
 * @brief Test that KeyPressedEvent stores keycode correctly
 */
TEST_F(KeyEventTest, KeyPressedEvent_Constructor_StoresKeyCode) {
	constexpr int keycode = 65; // 'A' key
	constexpr int repeatCount = 0;

	const KeyPressedEvent event{keycode, repeatCount};

	EXPECT_EQ(event.GetKeyCode(), keycode);
}

/**
 * @brief Test that KeyPressedEvent stores repeat count correctly
 */
TEST_F(KeyEventTest, KeyPressedEvent_Constructor_StoresRepeatCount) {
	constexpr int keycode = 65;
	constexpr int repeatCount = 5;

	const KeyPressedEvent event{keycode, repeatCount};

	EXPECT_EQ(event.GetRepeatCount(), repeatCount);
}

/**
 * @brief Test that KeyPressedEvent has correct event type
 */
TEST_F(KeyEventTest, KeyPressedEvent_GetEventType_ReturnsCorrectType) {
	const KeyPressedEvent event{65, 0};

	EXPECT_EQ(event.GetEventType(), EventType::KeyPressed);
}

/**
 * @brief Test that KeyPressedEvent is in both Keyboard and Input categories
 */
TEST_F(KeyEventTest, KeyPressedEvent_IsInCategory_KeyboardAndInputCategories) {
	const KeyPressedEvent event{65, 0};

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
	constexpr int keycode = 32; // Space key

	const KeyReleasedEvent event{keycode};

	EXPECT_EQ(event.GetKeyCode(), keycode);
}

/**
 * @brief Test that KeyReleasedEvent has correct event type
 */
TEST_F(KeyEventTest, KeyReleasedEvent_GetEventType_ReturnsCorrectType) {
	const KeyReleasedEvent event{32};

	EXPECT_EQ(event.GetEventType(), EventType::KeyReleased);
}

// ============================================================================
// KeyTypedEvent Tests
// ============================================================================

/**
 * @brief Test that KeyTypedEvent stores character code correctly
 */
TEST_F(KeyEventTest, KeyTypedEvent_Constructor_StoresCharCode) {
	constexpr unsigned int charCode = 97; // 'a' character

	const KeyTypedEvent event{charCode};

	EXPECT_EQ(event.GetKeyCode(), charCode);
}

/**
 * @brief Test that KeyTypedEvent has correct event type
 */
TEST_F(KeyEventTest, KeyTypedEvent_GetEventType_ReturnsCorrectType) {
	const KeyTypedEvent event{97};

	EXPECT_EQ(event.GetEventType(), EventType::KeyTyped);
}
