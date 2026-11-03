//
// Module: CelestialEngine/Engine/Tests/Modules/Core
// File: VersionTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-03
//

#include <Core/Version.hpp>

#include <gtest/gtest.h>

#include <regex>
#include <string>

/**
 * @brief Test that GetEngineVersion reports a non-empty MAJOR.MINOR.PATCH string
 */
TEST(VersionTest, GetEngineVersion_IsMajorMinorPatch) {
	const char* version = CE::GetEngineVersion();

	ASSERT_NE(version, nullptr);
	EXPECT_TRUE(std::regex_match(std::string(version), std::regex(R"(\d+\.\d+\.\d+)"))) << "got \"" << version << '"';
}
