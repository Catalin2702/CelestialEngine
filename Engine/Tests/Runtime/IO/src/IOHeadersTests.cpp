//
// Module: CelestialEngine/Engine/Tests/Runtime/IO
// File: IOHeadersTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

// The IO umbrella has to compile with nothing included before it.
#include <IO/IO.hpp>

#include <gtest/gtest.h>

#include <type_traits>

/**
 * @brief Test that the IO umbrella exposes the module's main types
 */
TEST(IOHeadersTest, Umbrella_ExposesTheModule) {
	EXPECT_TRUE(std::is_class_v<CE::IO::FileSystem>);
	EXPECT_TRUE(std::is_class_v<CE::IO::VirtualPath>);
	EXPECT_TRUE(std::is_enum_v<CE::IO::FileSystemError>);
}
