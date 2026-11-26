//
// Module: CelestialEngine/Engine/Tests/Runtime/Core/Layers/ImGui
// File: ImGuiKeyTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include <Core/Layers/ImGui/ImGuiHelpers.hpp>

#include <gtest/gtest.h>

using namespace CE::Types;
using namespace CE::Core;

namespace {

/**
 * @brief Test fixture for the engine-to-ImGui key and mouse-button conversions
 * @details Moved from the Types tests together with the conversions: Types knows nothing of ImGui.
 */
class ImGuiKeyTest: public ::testing::Test {};

}

// ============================================================================
// Keyboard
// ============================================================================

/**
 * @brief Test that the engine key codes map onto the ImGui key enum
 */
TEST_F(ImGuiKeyTest, ImGuiKeyFromKeyboard_MapsKnownKeys) {
	EXPECT_EQ(ImGuiKeyFromKeyboard(KeyboardKeyCode::A), ImGuiKey_A);
	EXPECT_EQ(ImGuiKeyFromKeyboard(KeyboardKeyCode::Space), ImGuiKey_Space);
	EXPECT_EQ(ImGuiKeyFromKeyboard(KeyboardKeyCode::Enter), ImGuiKey_Enter);
}

/**
 * @brief Test that an unknown key maps to the ImGui "none" key
 */
TEST_F(ImGuiKeyTest, ImGuiKeyFromKeyboard_UnknownKey_ReturnsNone) {
	EXPECT_EQ(ImGuiKeyFromKeyboard(KeyboardKeyCode::Unknown), ImGuiKey_None);
}

// ============================================================================
// Mouse buttons
// ============================================================================

/**
 * @brief Test that the engine button codes map onto the ImGui mouse button indices
 */
TEST_F(ImGuiKeyTest, ImGuiKeyFromMouseButton_MapsMainButtons) {
	EXPECT_EQ(ImGuiKeyFromMouseButton(MouseButtonCode::Left), 0);
	EXPECT_EQ(ImGuiKeyFromMouseButton(MouseButtonCode::Right), 1);
	EXPECT_EQ(ImGuiKeyFromMouseButton(MouseButtonCode::Middle), 2);
}

/**
 * @brief Test that the wheel axes are reported on the middle-button slot
 * @details ImGui models the wheel as part of the middle button, so both wheel axes fold onto index 2.
 */
TEST_F(ImGuiKeyTest, ImGuiKeyFromMouseButton_WheelAxes_MapToMiddleButton) {
	EXPECT_EQ(ImGuiKeyFromMouseButton(MouseButtonCode::WheelX), 2);
	EXPECT_EQ(ImGuiKeyFromMouseButton(MouseButtonCode::WheelY), 2);
}
