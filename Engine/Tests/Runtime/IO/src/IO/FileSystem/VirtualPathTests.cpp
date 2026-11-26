//
// Module: CelestialEngine/Engine/Tests/Runtime/IO/FileSystem
// File: VirtualPathTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include <IO/FileSystem/FileSystem.hpp>

#include <gtest/gtest.h>

#include <format>

using CE::IO::FileSystemError;
using CE::IO::Mount;
using CE::IO::VirtualPath;

// ============================================================================
// Parse Tests
// ============================================================================

/**
 * @brief Test that a well-formed path splits into its mount and its relative part
 */
TEST(VirtualPathTest, Parse_ValidPath_SplitsMountAndRelative) {
	const auto parsed = VirtualPath::Parse("resources://Shaders/OpenGL/CompositeVertex.glsl");

	ASSERT_TRUE(parsed.has_value());
	EXPECT_EQ(parsed->mount, "resources");
	EXPECT_EQ(parsed->relative, "Shaders/OpenGL/CompositeVertex.glsl");
}

/**
 * @brief Test that a plain relative path, with no mount, is refused
 */
TEST(VirtualPathTest, Parse_NoMount_IsInvalid) {
	EXPECT_EQ(VirtualPath::Parse("Shaders/OpenGL/CompositeVertex.glsl").error(), FileSystemError::InvalidPath);
	EXPECT_EQ(VirtualPath::Parse("://imgui.ini").error(), FileSystemError::InvalidPath);
}

/**
 * @brief Test that a mount name outside lowercase letters, digits and '_' is refused
 */
TEST(VirtualPathTest, Parse_BadMountName_IsInvalid) {
	EXPECT_EQ(VirtualPath::Parse("Resources://a.png").error(), FileSystemError::InvalidPath);
	EXPECT_EQ(VirtualPath::Parse("my-mount://a.png").error(), FileSystemError::InvalidPath);
}

/**
 * @brief Test that every way out of the mount is refused
 */
TEST(VirtualPathTest, Parse_EscapingRelativePart_IsInvalid) {
	EXPECT_EQ(VirtualPath::Parse("config://../secret.txt").error(), FileSystemError::InvalidPath);
	EXPECT_EQ(VirtualPath::Parse("config://a/../../secret.txt").error(), FileSystemError::InvalidPath);
	EXPECT_EQ(VirtualPath::Parse("config:///etc/passwd").error(), FileSystemError::InvalidPath);
	EXPECT_EQ(VirtualPath::Parse("config://C:/Windows/win.ini").error(), FileSystemError::InvalidPath);
	EXPECT_EQ(VirtualPath::Parse("config://a\\b.txt").error(), FileSystemError::InvalidPath);
}

/**
 * @brief Test that ".." is refused as a segment, not as a substring: a name may contain two dots
 */
TEST(VirtualPathTest, Parse_DotsInsideAName_IsValid) {
	const auto parsed = VirtualPath::Parse("resources://Textures/logo..v2.png");

	ASSERT_TRUE(parsed.has_value());
	EXPECT_EQ(parsed->relative, "Textures/logo..v2.png");
}

// ============================================================================
// Mount and Compose Tests
// ============================================================================

/**
 * @brief Test that every standard mount is spelled as the constant FileSystem mounts it under
 */
TEST(VirtualPathTest, Mount_FormatAs_MatchesMountConstants) {
	EXPECT_EQ(format_as(Mount::Engine), CE::IO::ENGINE_MOUNT);
	EXPECT_EQ(format_as(Mount::Resources), CE::IO::RESOURCES_MOUNT);
	EXPECT_EQ(format_as(Mount::Config), CE::IO::CONFIG_MOUNT);
	EXPECT_EQ(format_as(Mount::Cache), CE::IO::CACHE_MOUNT);
	EXPECT_EQ(std::format("{}", Mount::Resources), "resources");
}

/**
 * @brief Test that Compose builds the "<mount>://<path>" form Parse reads back
 */
TEST(VirtualPathTest, Compose_StandardMount_RoundTripsThroughParse) {
	const auto composed = VirtualPath::Compose(Mount::Resources, "Textures/logo.png");
	EXPECT_EQ(composed, "resources://Textures/logo.png");

	const auto parsed = VirtualPath::Parse(composed);
	ASSERT_TRUE(parsed.has_value());
	EXPECT_EQ(parsed->mount, "resources");
	EXPECT_EQ(parsed->relative, "Textures/logo.png");
}

/**
 * @brief Test that a path composed from no mount is one Parse refuses
 * @details Compose does not validate: Mount::None spells as "", so the result has no mount, and the error surfaces
 *			where the path is used (any FileSystem call parses it).
 */
TEST(VirtualPathTest, Compose_NoneMount_IsInvalidPath) {
	EXPECT_EQ(VirtualPath::Parse(VirtualPath::Compose(Mount::None, "imgui.ini")).error(), FileSystemError::InvalidPath);
}
