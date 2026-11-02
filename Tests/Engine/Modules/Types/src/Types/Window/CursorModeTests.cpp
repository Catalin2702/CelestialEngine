//
// Module: CelestialEngine/Tests/Engine/Modules/Types/Window
// File: CursorModeTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include <Types/Types.hpp>

#include <gtest/gtest.h>

#include <format>
#include <string_view>

using CE::Types::CursorMode;

// ============================================================================
// Naming Tests
// ============================================================================

/**
 * @brief Test that every enumerator is named by format_as, the hook fmt/spdlog look up by ADL
 */
TEST(CursorModeTest, FormatAs_NamesEveryEnumerator) {
	EXPECT_EQ(format_as(CursorMode::Unknown), "Unknown");
	EXPECT_EQ(format_as(CursorMode::Normal), "Normal");
	EXPECT_EQ(format_as(CursorMode::Captured), "Captured");
}

/**
 * @brief Test that std::format goes through the same single switch as format_as
 */
TEST(CursorModeTest, StdFormat_MatchesFormatAs) {
	EXPECT_EQ(std::format("{}", CursorMode::Normal), "Normal");
	EXPECT_EQ(std::format("{}", CursorMode::Captured), "Captured");
	EXPECT_EQ(std::format("[{:>8}]", CursorMode::Normal), "[  Normal]");
}

/**
 * @brief Test that a value outside the enum is named Unknown instead of reading garbage
 */
TEST(CursorModeTest, FormatAs_OutOfRangeValueIsUnknown) {
	EXPECT_EQ(format_as(static_cast<CursorMode>(200)), "Unknown");
}

/**
 * @brief Test that Normal is not the zero value, so a forgotten initialisation never reads as a valid mode
 */
TEST(CursorModeTest, ValueInitialisedMode_IsUnknown) {
	EXPECT_EQ(CursorMode{}, CursorMode::Unknown);
}
