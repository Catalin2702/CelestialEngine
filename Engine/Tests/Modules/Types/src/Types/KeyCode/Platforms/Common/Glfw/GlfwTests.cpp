//
// Module: CelestialEngine/Engine/Tests/Modules/Types/KeyCode/Platforms/Common/Glfw
// File: GlfwTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include <Types/KeyCode/Platforms/Common/Glfw/Glfw.hpp>

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for the GLFW key and mouse-button conversions
 * @details Compiled only with this window backend (see Engine/Tests/Modules/Types/CMakeLists.txt).
 */
class GlfwKeyCodeTest: public ::testing::Test {};

}

// ============================================================================
// Keyboard Mapping Tests
// ============================================================================

/**
 * @brief Test that GLFW key codes map onto the engine key codes
 */
TEST_F(GlfwKeyCodeTest, KeyboardKeyCodeFromGlfw_MapsKnownKeys) {
	EXPECT_EQ(KeyboardKeyCodeFromGlfw(GLFW_KEY_A), KeyboardKeyCode::A);
	EXPECT_EQ(KeyboardKeyCodeFromGlfw(GLFW_KEY_SPACE), KeyboardKeyCode::Space);
	EXPECT_EQ(KeyboardKeyCodeFromGlfw(GLFW_KEY_ESCAPE), KeyboardKeyCode::Escape);
	EXPECT_EQ(KeyboardKeyCodeFromGlfw(GLFW_KEY_LEFT_SHIFT), KeyboardKeyCode::LeftShift);
}

/**
 * @brief Test that an unmapped GLFW key becomes Unknown
 */
TEST_F(GlfwKeyCodeTest, KeyboardKeyCodeFromGlfw_UnmappedKey_ReturnsUnknown) {
	EXPECT_EQ(KeyboardKeyCodeFromGlfw(GLFW_KEY_UNKNOWN), KeyboardKeyCode::Unknown);
}

/**
 * @brief Test that the GLFW mapping round trips
 */
TEST_F(GlfwKeyCodeTest, GlfwMapping_RoundTrips) {
	for (const auto key: {KeyboardKeyCode::A, KeyboardKeyCode::Z, KeyboardKeyCode::Num5, KeyboardKeyCode::F1,
			KeyboardKeyCode::Space, KeyboardKeyCode::Enter, KeyboardKeyCode::Escape, KeyboardKeyCode::Left,
			KeyboardKeyCode::LeftControl, KeyboardKeyCode::RightSuper}) {
		EXPECT_EQ(KeyboardKeyCodeFromGlfw(GlfwKeyCodeFromKeyboard(key)), key) << "Key: " << format_as(key);
	}
}

// ============================================================================
// Mouse Button Mapping Tests
// ============================================================================

/**
 * @brief Test that GLFW button codes map onto the engine button codes
 */
TEST_F(GlfwKeyCodeTest, MouseButtonKeyCodeFromGlfw_MapsKnownButtons) {
	EXPECT_EQ(MouseButtonKeyCodeFromGlfw(GLFW_MOUSE_BUTTON_LEFT), MouseButtonCode::Left);
	EXPECT_EQ(MouseButtonKeyCodeFromGlfw(GLFW_MOUSE_BUTTON_RIGHT), MouseButtonCode::Right);
	EXPECT_EQ(MouseButtonKeyCodeFromGlfw(GLFW_MOUSE_BUTTON_MIDDLE), MouseButtonCode::Middle);
	EXPECT_EQ(MouseButtonKeyCodeFromGlfw(GLFW_MOUSE_BUTTON_8), MouseButtonCode::Button8);
}

/**
 * @brief Test that an unmapped GLFW button becomes Unknown
 */
TEST_F(GlfwKeyCodeTest, MouseButtonKeyCodeFromGlfw_UnmappedButton_ReturnsUnknown) {
	EXPECT_EQ(MouseButtonKeyCodeFromGlfw(42), MouseButtonCode::Unknown);
}

/**
 * @brief Test that the GLFW mapping round trips for the physical buttons
 */
TEST_F(GlfwKeyCodeTest, GlfwMapping_RoundTripsForPhysicalButtons) {
	for (const auto button: {MouseButtonCode::Left, MouseButtonCode::Right, MouseButtonCode::Middle,
			MouseButtonCode::Button4, MouseButtonCode::Button5, MouseButtonCode::Button6,
			MouseButtonCode::Button7, MouseButtonCode::Button8}) {
		EXPECT_EQ(MouseButtonKeyCodeFromGlfw(GlfwKeyCodeFromMouseButton(button)), button)
			<< "Button: " << format_as(button);
	}
}

/**
 * @brief Test that an unknown button has no GLFW mapping
 */
TEST_F(GlfwKeyCodeTest, GlfwKeyCodeFromMouseButton_Unknown_ReturnsUnknownKey) {
	EXPECT_EQ(GlfwKeyCodeFromMouseButton(MouseButtonCode::Unknown), GLFW_KEY_UNKNOWN);
}
