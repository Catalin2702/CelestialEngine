//
// Module: CelestialEngine/Tests/Engine/Modules/Utility/FileSystem
// File: FileSystemTests.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-13
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include <Utility/FileSystem/File.hpp>
#include <Utility/FileSystem/FileSystem.hpp>

#include <Tools/Log/Log.hpp>
#include <Types/FileSystem/File.hpp>

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

using CE::Utility::File;
using CE::Utility::FileSystem;
using CE::Types::FileLoadState;

namespace fs = std::filesystem;

namespace {

/**
 * @brief Test fixture for FileSystem / File tests
 * @details Every test works inside its own temporary directory, and file paths are absolute so that the root
 *			directory prefixing done by FileSystem resolves to exactly that directory.
 */
class FileSystemTest: public ::testing::Test {
protected:
	void SetUp() override {
		CE::Tools::Log::Init();

		_tempDirectory = fs::temp_directory_path() / ("CE_FileSystemTests_" + std::to_string(::testing::UnitTest::GetInstance()->random_seed()) + "_" +
													::testing::UnitTest::GetInstance()->current_test_info()->name());
		fs::remove_all(_tempDirectory);
		fs::create_directories(_tempDirectory);

		_previousRoot = FileSystem::GetRootDirectory();
		FileSystem::SetRootDirectory(_tempDirectory);
	}

	void TearDown() override {
		FileSystem::SetRootDirectory(_previousRoot);
		fs::remove_all(_tempDirectory);

		CE::Tools::Log::Shutdown();
	}

	/**
	 * @brief Builds an absolute path inside the test's temporary directory
	 */
	[[nodiscard]] fs::path PathFor(const std::string& name) const { return _tempDirectory / name; }

	/**
	 * @brief Reads a file straight from disk, bypassing the engine file abstraction
	 */
	static std::string ReadFromDisk(const fs::path& path) {
		const std::ifstream stream(path, std::ios::binary);
		std::ostringstream content;
		content << stream.rdbuf();
		return content.str();
	}

protected:
	fs::path _tempDirectory;
	fs::path _previousRoot;
};

}

// ============================================================================
// Root Directory Tests
// ============================================================================

/**
 * @brief Test that the root directory can be changed and read back
 */
TEST_F(FileSystemTest, SetRootDirectory_ChangesRootDirectory) {
	EXPECT_EQ(FileSystem::GetRootDirectory(), _tempDirectory);
}

// ============================================================================
// StCreate Tests
// ============================================================================

/**
 * @brief Test that StCreate writes the file to disk with its content
 */
TEST_F(FileSystemTest, StCreate_WritesFileToDisk) {
	const auto path = PathFor("created.txt");

	const File file = FileSystem::StCreate(path, "hello engine");

	EXPECT_TRUE(fs::exists(path));
	EXPECT_EQ(ReadFromDisk(path), "hello engine");
	EXPECT_TRUE(file.IsLoaded());
	EXPECT_EQ(file.GetContentString(), "hello engine");
}

/**
 * @brief Test that StCreate on an existing file loads it instead of truncating it
 */
TEST_F(FileSystemTest, StCreate_ExistingFile_LoadsItInstead) {
	const auto path = PathFor("existing.txt");
	FileSystem::StCreate(path, "original");

	const File file = FileSystem::StCreate(path, "replacement");

	EXPECT_EQ(ReadFromDisk(path), "original");
	EXPECT_EQ(file.GetContentString(), "original");
}

/**
 * @brief Test that StCreate can produce an empty file
 */
TEST_F(FileSystemTest, StCreate_EmptyContent_CreatesEmptyFile) {
	const auto path = PathFor("empty.txt");

	const File file = FileSystem::StCreate(path);

	EXPECT_TRUE(fs::exists(path));
	EXPECT_TRUE(file.GetContentString().empty());
}

// ============================================================================
// StNew Tests
// ============================================================================

/**
 * @brief Test that StNew builds an in-memory file without touching the disk
 */
TEST_F(FileSystemTest, StNew_DoesNotTouchDisk) {
	const auto path = PathFor("in-memory.txt");

	const File file = FileSystem::StNew(path, "not on disk");

	EXPECT_FALSE(fs::exists(path));
	EXPECT_TRUE(file.IsLoaded());
	EXPECT_TRUE(file.IsChanged());
	EXPECT_FALSE(file.IsSaved());
	EXPECT_EQ(file.GetContentString(), "not on disk");
}

// ============================================================================
// StLoad / StExists Tests
// ============================================================================

/**
 * @brief Test that StLoad reads back the content written to disk
 */
TEST_F(FileSystemTest, StLoad_ReadsContentFromDisk) {
	const auto path = PathFor("loadable.txt");
	FileSystem::StCreate(path, "content on disk");

	const File file = FileSystem::StLoad(path);

	EXPECT_TRUE(file.IsLoaded());
	EXPECT_EQ(file.GetContentString(), "content on disk");
}

/**
 * @brief Test that loading a missing file throws
 */
TEST_F(FileSystemTest, StLoad_MissingFile_Throws) {
	EXPECT_THROW(FileSystem::StLoad(PathFor("missing.txt")), std::runtime_error);
}

/**
 * @brief Test that StExists reports both existing and missing files
 */
TEST_F(FileSystemTest, StExists_ReportsFilePresence) {
	const auto path = PathFor("present.txt");

	EXPECT_FALSE(FileSystem::StExists(path));

	const File file = FileSystem::StCreate(path, "here");

	EXPECT_TRUE(FileSystem::StExists(path));
	EXPECT_TRUE(FileSystem::StExists(file));
	EXPECT_TRUE(file.Exists());
}

// ============================================================================
// Save / Reload / Unload Tests
// ============================================================================

/**
 * @brief Test that Save flushes the in-memory content to disk
 */
TEST_F(FileSystemTest, Save_WritesContentToDisk) {
	const auto path = PathFor("saved.txt");
	File file = FileSystem::StCreate(path, "first");

	file.SetContent(std::string{"second"});

	EXPECT_TRUE(file.IsChanged());
	EXPECT_FALSE(file.IsSaved());

	file.Save();

	EXPECT_TRUE(file.IsSaved());
	EXPECT_FALSE(file.IsChanged());
	EXPECT_EQ(ReadFromDisk(path), "second");
}

/**
 * @brief Test that Reload discards the unsaved in-memory changes
 */
TEST_F(FileSystemTest, Reload_DiscardsUnsavedChanges) {
	const auto path = PathFor("reloaded.txt");
	File file = FileSystem::StCreate(path, "on disk");

	file.SetContent(std::string{"only in memory"});
	file.Reload();

	EXPECT_EQ(file.GetContentString(), "on disk");
}

/**
 * @brief Test that UnLoad drops the content but keeps the file on disk
 */
TEST_F(FileSystemTest, UnLoad_DropsContentButKeepsFile) {
	const auto path = PathFor("unloaded.txt");
	File file = FileSystem::StCreate(path, "content");

	file.UnLoad();

	EXPECT_FALSE(file.IsLoaded());
	EXPECT_TRUE(file.GetContentBytes().empty());
	EXPECT_TRUE(fs::exists(path));
}

/**
 * @brief Test that the lazy getter reloads the content of an unloaded file
 */
TEST_F(FileSystemTest, GetContentStringLazy_ReloadsUnloadedFile) {
	const auto path = PathFor("lazy.txt");
	File file = FileSystem::StCreate(path, "lazy content");
	file.UnLoad();

	EXPECT_EQ(file.GetContentStringLazy(), "lazy content");
	EXPECT_TRUE(file.IsLoaded());
}

/**
 * @brief Test that the non-lazy getter returns nothing while the file is unloaded
 */
TEST_F(FileSystemTest, GetContentString_WhileUnloaded_ReturnsEmpty) {
	const auto path = PathFor("not-loaded.txt");
	File file = FileSystem::StCreate(path, "content");
	file.UnLoad();

	EXPECT_TRUE(file.GetContentString().empty());
}

/**
 * @brief Test that saving an unloaded file throws
 */
TEST_F(FileSystemTest, Save_WhileUnloaded_Throws) {
	const auto path = PathFor("unsavable.txt");
	File file = FileSystem::StCreate(path, "content");
	file.UnLoad();

	EXPECT_THROW(file.Save(), std::runtime_error);
}

/**
 * @brief Test that an auto-saving file flushes its changes when destroyed
 */
TEST_F(FileSystemTest, AutoSave_FlushesChangesOnDestruction) {
	const auto path = PathFor("autosaved.txt");

	{
		File file = FileSystem::StCreate(path, "before", true);
		EXPECT_TRUE(file.IsAutoSaveEnabled());
		file.SetContent(std::string{"after"});
	}

	EXPECT_EQ(ReadFromDisk(path), "after");
}

/**
 * @brief Test that a file without auto-save keeps the on-disk content untouched
 */
TEST_F(FileSystemTest, WithoutAutoSave_DestructionDoesNotFlushChanges) {
	const auto path = PathFor("not-autosaved.txt");

	{
		File file = FileSystem::StCreate(path, "before");
		EXPECT_FALSE(file.IsAutoSaveEnabled());
		file.SetContent(std::string{"after"});
	}

	EXPECT_EQ(ReadFromDisk(path), "before");
}

// ============================================================================
// Relative Path Tests
// ============================================================================

/**
 * @brief Test that a relative path is resolved against the root directory, not the working directory
 * @details A File keeps the path it was created with, so every operation must resolve it the same way: creating a
 *			file under a relative path and saving it back has to land on the same file on disk.
 */
TEST_F(FileSystemTest, RelativePath_IsResolvedAgainstTheRootDirectory) {
	const fs::path relativePath = "relative.txt";

	const File file = FileSystem::StCreate(relativePath, "content");

	EXPECT_TRUE(fs::exists(_tempDirectory / relativePath));
	EXPECT_FALSE(fs::exists(fs::current_path() / relativePath));
	EXPECT_TRUE(FileSystem::StExists(relativePath));
	EXPECT_TRUE(file.Exists());
}

/**
 * @brief Test that saving a file created with a relative path writes back to the root directory
 */
TEST_F(FileSystemTest, RelativePath_SaveWritesBackToTheRootDirectory) {
	const fs::path relativePath = "saved-relative.txt";
	File file = FileSystem::StCreate(relativePath, "before");

	file.SetContent(std::string{"after"});
	file.Save();

	EXPECT_EQ(ReadFromDisk(_tempDirectory / relativePath), "after");
	EXPECT_FALSE(fs::exists(fs::current_path() / relativePath));
}

/**
 * @brief Test that a relative path survives a save/load round trip
 */
TEST_F(FileSystemTest, RelativePath_SurvivesSaveLoadRoundTrip) {
	const fs::path relativePath = "round-trip.txt";
	File file = FileSystem::StCreate(relativePath);

	file.SetContent(std::string{"round trip"});
	file.Save();

	const File reloaded = FileSystem::StLoad(relativePath);

	EXPECT_EQ(reloaded.GetContentString(), "round trip");
	EXPECT_EQ(reloaded.GetPath(), relativePath);
}

/**
 * @brief Test that an auto-saving file created with a relative path flushes to the root directory
 */
TEST_F(FileSystemTest, RelativePath_AutoSaveWritesBackToTheRootDirectory) {
	const fs::path relativePath = "autosaved-relative.txt";

	{
		File file = FileSystem::StCreate(relativePath, "before", true);
		file.SetContent(std::string{"after"});
	}

	EXPECT_EQ(ReadFromDisk(_tempDirectory / relativePath), "after");
	EXPECT_FALSE(fs::exists(fs::current_path() / relativePath));
}

// ============================================================================
// Content / Path Accessor Tests
// ============================================================================

/**
 * @brief Test that binary content survives a save/load round trip
 */
TEST_F(FileSystemTest, SetContent_BinaryContent_SurvivesRoundTrip) {
	const auto path = PathFor("binary.bin");
	const std::vector<u8> bytes{0x00, 0x01, 0x7F, 0x80, 0xFF};

	File file = FileSystem::StCreate(path);
	file.SetContent(bytes);
	file.Save();

	const File reloaded = FileSystem::StLoad(path);

	EXPECT_EQ(reloaded.GetContentBytes(), bytes);
}

/**
 * @brief Test that the path accessors decompose the file path
 */
TEST_F(FileSystemTest, PathAccessors_DecomposeThePath) {
	const auto path = PathFor("shader.vert.glsl");
	const File file = FileSystem::StCreate(path, "");

	EXPECT_EQ(file.GetPath(), path);
	EXPECT_EQ(file.GetDirectory(), _tempDirectory);
	EXPECT_EQ(file.GetName(), "shader.vert.glsl");
	EXPECT_EQ(file.GetStem(), "shader.vert");
	EXPECT_EQ(file.GetExtension(), ".glsl");
}

/**
 * @brief Test that SetAutoSave toggles the flag
 */
TEST_F(FileSystemTest, SetAutoSave_TogglesFlag) {
	File file = FileSystem::StNew(PathFor("toggle.txt"), "content");

	EXPECT_FALSE(file.IsAutoSaveEnabled());

	file.SetAutoSave(true);
	EXPECT_TRUE(file.IsAutoSaveEnabled());

	file.SetAutoSave(false);
	EXPECT_FALSE(file.IsAutoSaveEnabled());
}
