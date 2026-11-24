//
// Module: CelestialEngine/Engine/Tests/Modules/IO/FileSystem
// File: VirtualPathTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#include <IO/FileSystem/FileSystem.hpp>

#include <gtest/gtest.h>

using CE::IO::FileSystemError;
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
