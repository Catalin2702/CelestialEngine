//
// Module: CelestialEngine/Engine/Tests/Modules/Types/Var
// File: VarsTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-25
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

#include <Types/Var/Vars.hpp>

#include <gtest/gtest.h>

/**
 * @brief Tests for the BIT macro, moved here from the Define tests together with the macro
 */
namespace VarsTests {

// ============================================================================
// BIT Macro Compilation Tests
// ============================================================================

TEST(VarsTests, BIT_Macro_CompileTimeConstants) {
	// Test that BIT macro produces compile-time constants
	constexpr int bit0 = BIT(0);
	constexpr int bit1 = BIT(1);
	constexpr int bit2 = BIT(2);
	constexpr int bit7 = BIT(7);

	// Verify values
	static_assert(bit0 == 1, "BIT(0) should be 1");
	static_assert(bit1 == 2, "BIT(1) should be 2");
	static_assert(bit2 == 4, "BIT(2) should be 4");
	static_assert(bit7 == 128, "BIT(7) should be 128");

	// Runtime verification (redundant but comprehensive)
	EXPECT_EQ(bit0, 1);
	EXPECT_EQ(bit1, 2);
	EXPECT_EQ(bit2, 4);
	EXPECT_EQ(bit7, 128);
}

TEST(VarsTests, BIT_Macro_BitwiseOperations) {
	// Test that BIT macro works correctly with bitwise operations
	constexpr int flags = BIT(0) | BIT(2) | BIT(4);
	static_assert(flags == (1 | 4 | 16), "Bitwise OR should work correctly");
	static_assert(flags == 21, "Combined flags should equal 21");

	EXPECT_EQ(flags, 21);
	EXPECT_TRUE(flags & BIT(0));
	EXPECT_FALSE(flags & BIT(1));
	EXPECT_TRUE(flags & BIT(2));
}

TEST(VarsTests, BIT_Macro_UsableInConstexprContext) {
	// Verify that BIT macro can be used in constexpr contexts
	constexpr auto compute_flags = [] constexpr {
		return BIT(0) | BIT(3) | BIT(5);
	};

	constexpr auto flags = compute_flags();
	static_assert(flags == (1 | 8 | 32), "Should be usable in constexpr context");
	EXPECT_EQ(flags, 41);
}

}
