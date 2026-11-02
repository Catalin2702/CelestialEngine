//
// Module: CelestialEngine/Tests/Engine/Modules/Utility
// File: UtilityHeadersTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

// The Utility umbrella has to compile with nothing included before it.
#include <Utility/Utility.hpp>

#include <gtest/gtest.h>

#include <type_traits>

/**
 * @brief Test that the Utility umbrella exposes the module's main types
 */
TEST(UtilityHeadersTest, Umbrella_ExposesTheModule) {
	EXPECT_TRUE(std::is_class_v<CE::Utility::Config>);
	EXPECT_TRUE(std::is_class_v<CE::Utility::Chronometer>);
}
