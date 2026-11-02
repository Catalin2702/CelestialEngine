//
// Module: CelestialEngine/Engine/Tests/Modules/Types/KeyCode
// File: KeyboardKeyCodeTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include <Types/KeyCode/KeyboardKeyCode.hpp>

#include <imgui.h>
#include <gtest/gtest.h>

#include <format>


using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for KeyboardKeyCode tests
 * @details The engine key codes are GLFW-based, and each platform layer maps its own native codes onto them, so
 *			the round trips through the platform mappings are what keeps the input layer consistent.
 */
class KeyboardKeyCodeTest: public ::testing::Test {};

}

// ============================================================================
// Conversion Tests
// ============================================================================

/**
 * @brief Test that the integer conversions preserve the enum value
 */
TEST_F(KeyboardKeyCodeTest, ToInt_PreservesEnumValue) {
	EXPECT_EQ(ToInt(KeyboardKeyCode::A), 65);
	EXPECT_EQ(ToUInt(KeyboardKeyCode::A), 65u);
	EXPECT_EQ(ToInt(KeyboardKeyCode::Unknown), 0);
}

/**
 * @brief Test that formatting a key gives its name
 */
TEST_F(KeyboardKeyCodeTest, Format_NamesTheKey) {
	EXPECT_EQ(std::format("{}", KeyboardKeyCode::A), "A");
	EXPECT_EQ(std::format("{}", KeyboardKeyCode::Num0), "0");
	EXPECT_EQ(std::format("{}", KeyboardKeyCode::Space), "Space");
	EXPECT_EQ(std::format("{}", KeyboardKeyCode::F12), "F12");
	EXPECT_EQ(std::format("{}", KeyboardKeyCode::Unknown), "Unknown");
}

// ============================================================================
// ImGui Mapping Tests
// ============================================================================

/**
 * @brief Test that the engine key codes map onto the ImGui key enum
 */
TEST_F(KeyboardKeyCodeTest, ImGuiKeyFromKeyboard_MapsKnownKeys) {
	EXPECT_EQ(ImGuiKeyFromKeyboard(KeyboardKeyCode::A), ImGuiKey_A);
	EXPECT_EQ(ImGuiKeyFromKeyboard(KeyboardKeyCode::Space), ImGuiKey_Space);
	EXPECT_EQ(ImGuiKeyFromKeyboard(KeyboardKeyCode::Enter), ImGuiKey_Enter);
}

/**
 * @brief Test that an unknown key maps to the ImGui "none" key
 */
TEST_F(KeyboardKeyCodeTest, ImGuiKeyFromKeyboard_UnknownKey_ReturnsNone) {
	EXPECT_EQ(ImGuiKeyFromKeyboard(KeyboardKeyCode::Unknown), ImGuiKey_None);
}

// ============================================================================
// Formatting Tests
// ============================================================================

/**
 * @brief Test that a key code formats as its name
 */
TEST_F(KeyboardKeyCodeTest, Formatter_FormatsAsKeyName) {
	EXPECT_EQ(std::format("{}", KeyboardKeyCode::A), "A");
	EXPECT_EQ(std::format("{}", KeyboardKeyCode::Escape), "Escape");
}

/**
 * @brief Test that a key code compares against its raw numeric value
 */
TEST_F(KeyboardKeyCodeTest, ComparisonOperator_ComparesAgainstRawValue) {
	EXPECT_TRUE(64u < KeyboardKeyCode::A);
	EXPECT_TRUE(66u > KeyboardKeyCode::A);
	EXPECT_TRUE(65u <= KeyboardKeyCode::A);
	EXPECT_TRUE(65u >= KeyboardKeyCode::A);
}
