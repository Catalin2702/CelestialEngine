//
// Module: CelestialEngine/Tests/Engine/Modules/Types/KeyCode
// File: MouseButtonCodeTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include <Types/KeyCode/MouseButtonCode.hpp>

#include <GLFW/glfw3.h>
#include <gtest/gtest.h>

#include <format>


using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for MouseButtonCode tests
 */
class MouseButtonCodeTest: public ::testing::Test {};

}

// ============================================================================
// Conversion Tests
// ============================================================================

/**
 * @brief Test that the integer conversions preserve the enum value
 */
TEST_F(MouseButtonCodeTest, ToInt_PreservesEnumValue) {
	EXPECT_EQ(ToInt(MouseButtonCode::Left), 0);
	EXPECT_EQ(ToInt(MouseButtonCode::Right), 1);
	EXPECT_EQ(ToInt(MouseButtonCode::Middle), 2);
	EXPECT_EQ(ToUInt(MouseButtonCode::Unknown), 255u);
}

/**
 * @brief Test that formatting a button gives its name
 */
TEST_F(MouseButtonCodeTest, Format_NamesTheButton) {
	EXPECT_EQ(std::format("{}", MouseButtonCode::Left), "Left");
	EXPECT_EQ(std::format("{}", MouseButtonCode::Right), "Right");
	EXPECT_EQ(std::format("{}", MouseButtonCode::Middle), "Middle");
	EXPECT_EQ(std::format("{}", MouseButtonCode::Unknown), "Unknown");
}

// ============================================================================
// GLFW Mapping Tests
// ============================================================================

/**
 * @brief Test that GLFW button codes map onto the engine button codes
 */
TEST_F(MouseButtonCodeTest, MouseButtonKeyCodeFromGlfw_MapsKnownButtons) {
	EXPECT_EQ(MouseButtonKeyCodeFromGlfw(GLFW_MOUSE_BUTTON_LEFT), MouseButtonCode::Left);
	EXPECT_EQ(MouseButtonKeyCodeFromGlfw(GLFW_MOUSE_BUTTON_RIGHT), MouseButtonCode::Right);
	EXPECT_EQ(MouseButtonKeyCodeFromGlfw(GLFW_MOUSE_BUTTON_MIDDLE), MouseButtonCode::Middle);
	EXPECT_EQ(MouseButtonKeyCodeFromGlfw(GLFW_MOUSE_BUTTON_8), MouseButtonCode::Button8);
}

/**
 * @brief Test that an unmapped GLFW button becomes Unknown
 */
TEST_F(MouseButtonCodeTest, MouseButtonKeyCodeFromGlfw_UnmappedButton_ReturnsUnknown) {
	EXPECT_EQ(MouseButtonKeyCodeFromGlfw(42), MouseButtonCode::Unknown);
}

/**
 * @brief Test that the GLFW mapping round trips for the physical buttons
 */
TEST_F(MouseButtonCodeTest, GlfwMapping_RoundTripsForPhysicalButtons) {
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
TEST_F(MouseButtonCodeTest, GlfwKeyCodeFromMouseButton_Unknown_ReturnsUnknownKey) {
	EXPECT_EQ(GlfwKeyCodeFromMouseButton(MouseButtonCode::Unknown), GLFW_KEY_UNKNOWN);
}

// ============================================================================
// Cocoa Mapping Tests
// ============================================================================

/**
 * @brief Test that macOS button numbers map onto the engine button codes
 */
TEST_F(MouseButtonCodeTest, MouseButtonKeyCodeFromCocoa_MapsKnownButtons) {
	EXPECT_EQ(MouseButtonKeyCodeFromCocoa(0), MouseButtonCode::Left);
	EXPECT_EQ(MouseButtonKeyCodeFromCocoa(1), MouseButtonCode::Right);
	EXPECT_EQ(MouseButtonKeyCodeFromCocoa(2), MouseButtonCode::Middle);
	EXPECT_EQ(MouseButtonKeyCodeFromCocoa(7), MouseButtonCode::Unknown);
}

/**
 * @brief Test that the Cocoa mapping round trips for the three main buttons
 */
TEST_F(MouseButtonCodeTest, CocoaMapping_RoundTripsForMainButtons) {
	for (const auto button: {MouseButtonCode::Left, MouseButtonCode::Right, MouseButtonCode::Middle}) {
		EXPECT_EQ(MouseButtonKeyCodeFromCocoa(CocoaButtonNumberFromMouseButton(button)), button)
			<< "Button: " << format_as(button);
	}
}

/**
 * @brief Test that an unknown button has no macOS button number
 */
TEST_F(MouseButtonCodeTest, CocoaButtonNumberFromMouseButton_Unknown_ReturnsInvalid) {
	EXPECT_EQ(CocoaButtonNumberFromMouseButton(MouseButtonCode::Unknown), -1);
}

// ============================================================================
// ImGui Mapping Tests
// ============================================================================

/**
 * @brief Test that the engine button codes map onto the ImGui mouse button indices
 */
TEST_F(MouseButtonCodeTest, ImGuiKeyFromMouseButton_MapsMainButtons) {
	EXPECT_EQ(ImGuiKeyFromMouseButton(MouseButtonCode::Left), 0);
	EXPECT_EQ(ImGuiKeyFromMouseButton(MouseButtonCode::Right), 1);
	EXPECT_EQ(ImGuiKeyFromMouseButton(MouseButtonCode::Middle), 2);
}

/**
 * @brief Test that the wheel axes are reported on the middle-button slot
 * @details ImGui models the wheel as part of the middle button, so both wheel axes fold onto index 2.
 */
TEST_F(MouseButtonCodeTest, ImGuiKeyFromMouseButton_WheelAxes_MapToMiddleButton) {
	EXPECT_EQ(ImGuiKeyFromMouseButton(MouseButtonCode::WheelX), 2);
	EXPECT_EQ(ImGuiKeyFromMouseButton(MouseButtonCode::WheelY), 2);
}

// ============================================================================
// Formatting Tests
// ============================================================================

/**
 * @brief Test that a button code formats as its name
 */
TEST_F(MouseButtonCodeTest, Formatter_FormatsAsButtonName) {
	EXPECT_EQ(std::format("{}", MouseButtonCode::Left), "Left");
}
