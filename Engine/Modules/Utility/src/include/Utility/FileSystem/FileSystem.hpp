//
// Module: CelestialEngine/Engine/Modules/Utility/FileSystem
// File: FileSystem.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_UTILITY_FILESYSTEM_FILESYSTEM_HPP
#define CE_UTILITY_FILESYSTEM_FILESYSTEM_HPP

#include "Define/DynamicLinker.hpp"

#include <filesystem>

namespace CE::Utility {

namespace fs = std::filesystem;

class File;

class CE_API FileSystem {
public:
	/**
	 * @brief Creates a new file with the specified content at the given path
	 * @param path The file path where the new file should be created
	 * @param content The content to write to the new file
	 * @param autoSave If true, the file will be automatically saved to disk when closed. Defaults to false.
	 * @return File A File object representing the newly created file
	 * @throws std::runtime_error if the file cannot be created or written to disk
	 * @details Creates a new file at the specified path and writes the provided content to it. If autoSave is enabled, the file will be automatically saved to disk when closed. If the file cannot be created or written to (e.g., due to permissions issues, invalid path, etc.), an exception is thrown to indicate the failure.
	 */
	static File StCreate(const fs::path& path, const std::string& content = "", bool autoSave = false);

	/**
	 * @brief Creates a new file with empty content at the given path
	 * @param path File path where the new file should be created
	 * @param content File content
	 * @param autoSave If true, the file will be automatically saved to disk when closed. Defaults to false.
	 * @return File A File object representing the newly created file
	 * @throws std::runtime_error if the file cannot be created or written to disk
	 * @details Creates a new file at the specified path with empty content. If autoSave is enabled, the file will be automatically saved to disk when closed. If the file cannot be created or written to (e.g., due to permissions issues, invalid path, etc.), an exception is thrown to indicate the failure.
	 */
	static File StNew(const fs::path& path, const std::string& content = "", bool autoSave = false);

	/**
	 * @brief Loads an existing file from disk at the given path
	 * @param path The file path to load the file from
	 * @param autoSave If true, the file will be automatically saved to disk when closed. Defaults to false.
	 * @return File A File object representing the loaded file
	 * @throws std::runtime_error if the file cannot be loaded from disk
	 * @details Loads an existing file from the specified path and initializes a File object with its content and related properties. If autoSave is enabled, the file will be automatically saved to disk when closed. If the file cannot be loaded (e.g., if it does not exist, has been moved, or if there are permissions issues), an exception is thrown to indicate the failure.
	 */
	static File StLoad(const fs::path& path, bool autoSave = false);

	/**
	 * @brief Loads the content of an existing File object from disk
	 * @param file The File object to load
	 * @return File A new File object representing the loaded file with content and related properties
	 * @throws std::runtime_error if the file cannot be loaded from disk
	 * @details Loads the content of the provided File object from disk, creating a new File object with the loaded content and related properties. If the file cannot be loaded (e.g., if it does not exist, has been moved, or if there are permissions issues), an exception is thrown to indicate the failure.
	 */
	static File StLoad(File& file);

	/**
	 * @brief Reloads the content of an existing File object from disk
	 * @param file The File object to reload
	 * @return File A new File object representing the reloaded file with updated content
	 * @throws std::runtime_error if the file cannot be reloaded from disk
	 * @details Reloads the content of the provided File object from disk, creating a new File object with the updated content and related properties. If the file cannot be reloaded (e.g., if it has been deleted, moved, or if there are permissions issues), an exception is thrown to indicate the failure.
	 */
	static File StReload(File& file);

	/**
	 * @brief Unloads the content of an existing File object from memory
	 * @param file The File object to unload
	 * @return File A new File object representing the unloaded file with cleared content
	 * @details Unloads the content of the provided File object from memory, creating a new File object with cleared content and reset related properties. This does not affect the file on disk; it simply frees up memory used by the File object.
	 */
	static File StUnload(File& file);

	/**
	 * @brief Saves the specified file to disk
	 * @param file The File object to save
	 * @throws std::runtime_error if the file cannot be saved to disk
	 * @details Saves the content of the provided File object back to its original file path on disk. If the file cannot be saved (e.g., due to permissions issues, disk errors, etc.), an exception is thrown to indicate the failure.
	 */
	static void StSave(const File& file);

	static bool StExists(const fs::path& path);

	static bool StExists(const File& file);

public:
	/**
	 * @brief Gets the root directory path for the file system
	 * @return std::filesystem::path The root directory path as a string
	 * @details Returns the root directory path for the file system. This is the base directory from which all file paths are resolved. The root directory can be set using the SetRootDirectory method, and it is used as a reference point for relative file paths.
	 */
	[[nodiscard]] static fs::path GetRootDirectory() { return _rootDirectory; }

	/**
	 * @brief Sets the root directory path for the file system
	 * @param rootDirectory The new root directory path to set
	 * @details Sets the root directory path for the file system. This is the base directory from which all file paths are resolved. The provided path will be stored as the new root directory, and it will be used as a reference point for relative file paths.
	 */
	static void SetRootDirectory(const fs::path& rootDirectory) { _rootDirectory = rootDirectory; }

private:
	static fs::path _rootDirectory;
};

}

#endif //CE_UTILITY_FILESYSTEM_FILESYSTEM_HPP
