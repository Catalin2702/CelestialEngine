//
// Module: CelestialEngine/Tests/Engine/Modules/Types/Build
// File: BuildTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-13
//

#include <Types/Build/Build.hpp>

#include <gtest/gtest.h>

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
TEST_F(BuildTypeTest, GetBuildTypeString_NamesTheBuildType) {
	EXPECT_EQ(GetBuildTypeString(BuildType::Debug), "Debug");
	EXPECT_EQ(GetBuildTypeString(BuildType::Release), "Release");
	EXPECT_EQ(GetBuildTypeString(BuildType::Dist), "Dist");
}

/**
 * @brief Test that the current build type matches the configuration macro
 * @details The build type is resolved at compile time from the CE_DEBUG / CE_RELEASE / CE_DIST macros CMake sets
 *			per configuration, so the test binary must report the configuration it was built in.
 */
TEST_F(BuildTypeTest, GetCurrentBuildType_MatchesTheConfigurationMacro) {
#if defined(CE_DEBUG)
	EXPECT_EQ(GetCurrentBuildType(), BuildType::Debug);
#elif defined(CE_RELEASE)
	EXPECT_EQ(GetCurrentBuildType(), BuildType::Release);
#elif defined(CE_DIST)
	EXPECT_EQ(GetCurrentBuildType(), BuildType::Dist);
#endif
}

/**
 * @brief Test that the current build type string matches the current build type
 */
TEST_F(BuildTypeTest, GetCurrentBuildTypeString_MatchesGetCurrentBuildType) {
	EXPECT_EQ(GetCurrentBuildTypeString(), GetBuildTypeString(GetCurrentBuildType()));
}
