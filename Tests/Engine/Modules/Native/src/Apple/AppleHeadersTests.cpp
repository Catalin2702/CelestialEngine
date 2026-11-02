//
// Module: CelestialEngine/Tests/Engine/Modules/Native/Apple
// File: AppleHeadersTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

// The Apple umbrellas have to compile with nothing included before them.
#include <Apple/MetalCpp/MetalCpp.hpp>

#include <Apple/Apple.hpp>

#include <gtest/gtest.h>

#include <type_traits>

/**
 * @brief Test that the Apple umbrella exposes the metal-cpp wrappers
 */
TEST(AppleHeadersTest, Umbrella_ExposesTheMetalCppWrappers) {
	EXPECT_TRUE(std::is_class_v<CE::Native::NsViewEventDispatcher>);
	EXPECT_TRUE(std::is_class_v<CE::Native::NsWindowEventDispatcher>);
}
