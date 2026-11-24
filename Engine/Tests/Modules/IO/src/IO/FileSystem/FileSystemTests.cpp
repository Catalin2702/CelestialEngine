//
// Module: CelestialEngine/Engine/Tests/Modules/IO/FileSystem
// File: FileSystemTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-24
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#include <IO/FileSystem/FileSystem.hpp>

#include <gtest/gtest.h>

#include <filesystem>
#include <format>
#include <fstream>
#include <string>

using CE::IO::FileSystem;
using CE::IO::FileSystemError;
using CE::IO::MountAccess;

namespace fs = std::filesystem;

namespace {

/**
 * @brief Test fixture for FileSystem
 * @details Every test gets its own temporary directory, mounted twice: "test" read-write and "readonly" read-only. The
 *			mount table is process-wide, so both are removed again in TearDown; the standard mounts are never touched,
 *			which keeps these tests off the real per-user directory.
 */
class FileSystemTest: public ::testing::Test {
protected:
	void SetUp() override {
		const auto* const test = ::testing::UnitTest::GetInstance()->current_test_info();
		_directory = fs::temp_directory_path() / std::format("CE_FileSystemTests_{}", test->name());
		fs::remove_all(_directory);
		fs::create_directories(_directory);

		ASSERT_TRUE(FileSystem::Mount("test", _directory, MountAccess::ReadWrite).has_value());
		ASSERT_TRUE(FileSystem::Mount("readonly", _directory, MountAccess::ReadOnly).has_value());
	}

	void TearDown() override {
		FileSystem::Unmount("test");
		FileSystem::Unmount("readonly");
		fs::remove_all(_directory);
	}

	/**
	 * @brief Writes a file straight to disk, bypassing the engine
	 */
	void WriteToDisk(const std::string& name, const std::string& content) const {
		std::ofstream stream(_directory / name, std::ios::binary);
		stream << content;
	}

protected:
	fs::path _directory;
};

}

// ============================================================================
// Mount Tests
// ============================================================================

/**
 * @brief Test that a mount resolves to its directory, with the relative part appended
 */
TEST_F(FileSystemTest, ToNativePath_AppendsRelativeToTheMountDirectory) {
	const auto path = FileSystem::ToNativePath("test://Textures/Logo.png");

	ASSERT_TRUE(path.has_value());
	EXPECT_EQ(*path, (_directory / "Textures/Logo.png").lexically_normal());
}

/**
 * @brief Test that a directory which does not exist cannot be mounted
 */
TEST_F(FileSystemTest, Mount_MissingDirectory_IsNotFound) {
	EXPECT_EQ(FileSystem::Mount("missing", _directory / "nope", MountAccess::ReadOnly).error(), FileSystemError::NotFound);
	EXPECT_FALSE(FileSystem::IsMounted("missing"));
}

/**
 * @brief Test that a path through a mount nobody made answers UnknownMount
 */
TEST_F(FileSystemTest, ReadBytes_UnknownMount_IsUnknownMount) {
	EXPECT_EQ(FileSystem::ReadBytes("nowhere://a.txt").error(), FileSystemError::UnknownMount);
}

// ============================================================================
// Read Tests
// ============================================================================

/**
 * @brief Test that ReadText returns the file exactly as it is on disk
 */
TEST_F(FileSystemTest, ReadText_ExistingFile_ReturnsItsContent) {
	WriteToDisk("shader.glsl", "#version 410 core\r\nvoid main() {}\n");

	const auto text = FileSystem::ReadText("test://shader.glsl");

	ASSERT_TRUE(text.has_value());
	EXPECT_EQ(*text, "#version 410 core\r\nvoid main() {}\n");
}

/**
 * @brief Test that ReadBytes returns every byte, zeros included
 */
TEST_F(FileSystemTest, ReadBytes_BinaryFile_ReturnsEveryByte) {
	WriteToDisk("data.bin", std::string("\x89PNG\0\x01", 6));

	const auto bytes = FileSystem::ReadBytes("test://data.bin");

	ASSERT_TRUE(bytes.has_value());
	ASSERT_EQ(bytes->size(), 6u);
	EXPECT_EQ((*bytes)[0], std::byte{0x89});
	EXPECT_EQ((*bytes)[4], std::byte{0x00});
}

/**
 * @brief Test that a missing file and a directory are told apart
 */
TEST_F(FileSystemTest, ReadBytes_MissingFileOrDirectory_SaysWhich) {
	fs::create_directories(_directory / "Folder");

	EXPECT_EQ(FileSystem::ReadBytes("test://missing.bin").error(), FileSystemError::NotFound);
	EXPECT_EQ(FileSystem::ReadBytes("test://Folder").error(), FileSystemError::NotAFile);
}

// ============================================================================
// Write Tests
// ============================================================================

/**
 * @brief Test that what WriteText writes, ReadText reads back - parent folders included
 */
TEST_F(FileSystemTest, WriteText_ThenReadText_RoundTrips) {
	ASSERT_TRUE(FileSystem::WriteText("test://Settings/editor.ini", "[Window]\nwidth=1280\n").has_value());

	EXPECT_EQ(FileSystem::ReadText("test://Settings/editor.ini").value_or(""), "[Window]\nwidth=1280\n");
}

/**
 * @brief Test that a second write replaces the file and leaves no temporary file behind
 */
TEST_F(FileSystemTest, WriteText_ExistingFile_IsReplacedWithNoLeftovers) {
	ASSERT_TRUE(FileSystem::WriteText("test://imgui.ini", "a much longer first version").has_value());
	ASSERT_TRUE(FileSystem::WriteText("test://imgui.ini", "short").has_value());

	EXPECT_EQ(FileSystem::ReadText("test://imgui.ini").value_or(""), "short");
	EXPECT_FALSE(fs::exists(_directory / "imgui.ini.tmp"));
}

/**
 * @brief Test that a read-only mount refuses writes and creates nothing
 */
TEST_F(FileSystemTest, WriteText_ReadOnlyMount_IsRefused) {
	EXPECT_EQ(FileSystem::WriteText("readonly://new.txt", "nope").error(), FileSystemError::ReadOnly);
	EXPECT_FALSE(FileSystem::Exists("test://new.txt"));
}

/**
 * @brief Test that a path climbing out of its mount is refused before anything is written
 */
TEST_F(FileSystemTest, WriteText_EscapingPath_IsRefused) {
	EXPECT_EQ(FileSystem::WriteText("test://../escaped.txt", "nope").error(), FileSystemError::InvalidPath);
	EXPECT_FALSE(fs::exists(_directory.parent_path() / "escaped.txt"));
}
