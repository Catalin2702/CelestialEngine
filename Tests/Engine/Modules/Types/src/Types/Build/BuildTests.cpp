//
// Module: CelestialEngine/Tests/Engine/Modules/Types/Build
// File: BuildTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-09-03
//

#include <Types/Build/Build.hpp>

#include <gtest/gtest.h>

#include <format>


using namespace CE::Types;

namespace {

/**
 * @brief Test fixture for BuildType tests
 */
class BuildTypeTest: public ::testing::Test {};

}

/**
 * @brief Test that every build type has a name
 */
TEST_F(BuildTypeTest, Format_NamesTheBuildType) {
	EXPECT_EQ(std::format("{}", BuildType::Debug), "Debug");
	EXPECT_EQ(std::format("{}", BuildType::Release), "Release");
	EXPECT_EQ(std::format("{}", BuildType::Dist), "Dist");
}

/**
 * @brief Test that the current build type matches the configuration macro
 * @details The build type is resolved at compile time from the CE_DEBUG / CE_RELEASE / CE_DIST macros CMake sets
 *			per configuration, so the test binary must report the configuration it was built in.
 */
TEST_F(BuildTypeTest, GetCurrentBuildType_MatchesTheConfigurationMacro) {
#if CE_DEBUG
	EXPECT_EQ(GetCurrentBuildType(), BuildType::Debug);
#elif CE_RELEASE
	EXPECT_EQ(GetCurrentBuildType(), BuildType::Release);
#elif CE_DIST
	EXPECT_EQ(GetCurrentBuildType(), BuildType::Dist);
#endif
}

/**
 * @brief Test that the current build type string matches the current build type
 */
TEST_F(BuildTypeTest, GetCurrentBuildTypeString_MatchesGetCurrentBuildType) {
	EXPECT_EQ(GetCurrentBuildTypeString(), std::format("{}", GetCurrentBuildType()));
}
