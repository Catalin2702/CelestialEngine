//
// Module: CelestialEngine/Engine/Tests/Runtime/Types/KeyCode
// File: MouseButtonCodeTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include <Types/KeyCode/MouseButtonCode.hpp>

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
// Formatting Tests
// ============================================================================

/**
 * @brief Test that a button code formats as its name
 */
TEST_F(MouseButtonCodeTest, Formatter_FormatsAsButtonName) {
	EXPECT_EQ(std::format("{}", MouseButtonCode::Left), "Left");
}
