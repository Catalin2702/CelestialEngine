//
// Module: CelestialEngine/Engine/Modules/Utility/FileSystem
// File: File.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_UTILITY_FILESYSTEM_FILE_HPP
#define CE_UTILITY_FILESYSTEM_FILE_HPP

#include "Define/DynamicLinker.hpp"

#include "Types/FileSystem/File.hpp"

#include <filesystem>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace CE::Utility {

class CE_API File {
	friend class FileSystem;
public:
	/**
	 * @brief Constructs a File object from a given file path
	 * @param path The file path to the file
	 * @param loadState The initial load state of the file (e.g., NotLoaded, Loaded, LazyLoading)
	 * @param autoSave If true, the file will be automatically saved to disk when closed. Defaults to false.
	 * @throws std::runtime_error if the file does not exist or cannot be read
	 * @details Initializes the File object by extracting the file name, extension, directory, and content from the provided file path.
	 *			If the file does not exist or cannot be read, an exception is thrown.
	 */
	explicit File(std::filesystem::path path, Types::FileLoadState loadState, bool autoSave = false);

	/**
	 * @brief Copy constructor
	 * @param other The File object to copy from
	 * @details Creates a new File by copying the path and the autoSave option. The content is not copied. The new File will attempt to load the content when accessed.
	 */
	File(const File& other);

	/**
	 * @brief Move constructor
	 * @param other The File object to move from
	 * @details Creates a new File by moving the path and content from the provided File object. After the move, the original File object will be left in a valid but unspecified state.
	 */
	File(File&& other) noexcept;

	/**
	 * @brief Destructor
	 * @throws std::runtime_error if autoSave is enabled and the file content has been modified but cannot be saved to disk
	 * @details Cleans up any resources associated with the File object.
	 *			If autoSave is enabled and the file content has been modified, the destructor will attempt to save the file to disk before destruction.
	 *			If autoSave is disabled, the destructor simply cleans up without saving.
	 *			If saving fails, an exception is thrown to indicate that the file could not be saved.
	 */
	~File();

public:
	/**
	 * @brief Copy assignment operator
	 * @param other The File object to copy from
	 * @return File& A reference to the current File object after copying
	 * @details Copies the file path from the provided File object. The content is not copied. The current File will attempt to load the content when accessed.
	 */
	File & operator=(const File& other);

	/**
	 * @brief Move assignment operator
	 * @param other The File object to move from
	 * @return File& A reference to the current File object after moving
	 * @details Moves the file path and content from the provided File object. After the move, the original File object will be left in a valid but unspecified state.
	 */
	File & operator=(File&& other) noexcept;

public:
	/**
	 * @brief Loads the file content from disk
	 * @throws std::runtime_error if the file cannot be loaded from disk
	 * @details Reads the content of the file from disk and updates the File object's content and related properties.
	 *			If the file cannot be loaded (e.g., if it does not exist, has been moved, or if there are permissions issues), an exception is thrown to indicate the failure.
	 */
	void Load();

	/**
	 * @brief Unloads the file content from memory
	 * @details Clears the content of the File object from memory and resets related properties. This does not affect the file on disk; it simply frees up memory used by the File object.
	 */
	void UnLoad();

	/**
	 * @brief Reloads the file content from disk
	 * @throws std::runtime_error if the file cannot be reloaded from disk
	 * @details Reads the content of the file from disk again, updating the File object's content and related properties.
	 *			If the file cannot be reloaded (e.g., if it has been deleted, moved, or if there are permissions issues), an exception is thrown to indicate the failure.
	 */
	void Reload();

	/**
	 * @brief Saves the file content to disk
	 * @throws std::runtime_error if the file cannot be saved to disk
	 * @details Writes the current content of the File object back to the original file path on disk.
	 *			If the file cannot be saved (e.g., due to permissions issues, disk errors, etc.), an exception is thrown to indicate the failure.
	 */
	void Save() const;

public:
	/**
	 * @brief Sets the content of the file as a string
	 * @param content The new content to set for the file as a string
	 * @details Updates the content of the File object with the provided string. This does not automatically save the file to disk; you must call Save() to persist changes.
	 */
	void SetContent(const std::string& content);

	/**
	 * @brief Sets the content of the file as a byte array
	 * @param content The new content to set for the file as a byte array
	 * @details Updates the content of the File object with the provided byte array. This does not automatically save the file to disk; you must call Save() to persist changes.
	 */
	void SetContent(const std::vector<uint8_t>& content);

	/**
	 * @brief Sets the content of the file as a byte array using move semantics
	 * @param content The new content to set for the file as a byte array, passed as an rvalue reference
	 * @details Updates the content of the File object with the provided byte array using move semantics to avoid unnecessary copying. This does not automatically save the file to disk; you must call Save() to persist changes.
	 */
	void SetContent(std::vector<uint8_t>&& content);

	/**
	 * @brief Enables or disables auto-saving of the file
	 * @param autoSave True to enable auto-saving, false to disable
	 * @details Sets the auto-save flag for the File object. If auto-saving is enabled, the file will be automatically saved to disk when modified and when the File object is destroyed.
	 *			If disabled, changes will not be automatically saved, and you must call Save() manually to persist changes.
	 */
	void SetAutoSave(const bool autoSave) { _autoSave = autoSave; }

public:
	/**
	* @brief Gets the content of the file as a string
	 * @return std::string The content of the file as a string
	 * @details Returns the content of the file as a string. If the content is stored as a byte array, it will be converted to a string before being returned.
	 *			If the file content has not been loaded yet and lazy loading is enabled, it will be loaded from disk before returning the content.
	 */
	std::string GetContentStringLazy();

	/**
	 * @brief Gets the content of the file as a byte array
	 * @return std::vector<uint8_t> The content of the file as a byte array
	 * @details Returns the content of the file as a byte array. If the content is stored as a string, it will be converted to a byte array before being returned.
	 *			If the file content has not been loaded yet and lazy loading is enabled, it will be loaded from disk before returning the content.
	 */
	[[nodiscard]] const std::vector<uint8_t>& GetContentBytesLazy();

	/**
	 * @brief Gets the content of the file as a string
	 * @return std::string The content of the file as a string
	 * @details Returns the content of the file as a string. If the content is stored as a byte array, it will be converted to a string before being returned.
	 *			This method does not perform lazy loading; if the content has not been loaded yet, it will return an empty string.
	 */
	std::string GetContentString() const;

	/**
	 * @brief Gets the content of the file as a byte array
	 * @return std::vector<uint8_t> The content of the file as a byte array
	 * @details Returns the content of the file as a byte array. If the content is stored as a string, it will be converted to a byte array before being returned.
	 *			This method does not perform lazy loading; if the content has not been loaded yet, it will return an empty byte array
	 */
	[[nodiscard]] const std::vector<uint8_t>& GetContentBytes() const;

	/**
	 * @brief Gets the full file path
	 * @return std::filesystem::path The full file path as a string
	 */
	[[nodiscard]] const fs::path& GetPath() const { return _path; }

	/**
	 * @brief Gets the directory path without the file name
	 * @return std::filesystem::path The directory path without the file name as a string
	 */
	fs::path GetDirectory() const { return _path.parent_path(); }

	/**
	 * @brief Gets the file name with extension
	 * @return std::filesystem::path The file name
	 */
	fs::path GetName() const { return _path.filename(); }

	/**
	 * @brief Gets the file name without extension
	 * @return std::filesystem::path The file name without extension as a string
	 */
	fs::path GetStem() const { return _path.stem(); }

	/**
	 * @brief Gets the file extension without the dot
	 * @return std::filesystem::path The file extension without the dot as a string
	 */
	fs::path GetExtension() const { return _path.extension(); }

	/**
	 * @brief Checks if the file should be automatically saved to disk when modified
	 * @return bool True if auto-save is enabled, false otherwise
	 */
	[[nodiscard]] bool IsAutoSaveEnabled() const { return _autoSave; }

	/**
	 * @brief Checks if the file content was successfully loaded
	 * @return bool True if the file content was successfully loaded, false otherwise
	 */
	[[nodiscard]] bool IsLoaded() const { return _isLoaded; }

	/**
	 * @brief Checks if the file content has been modified since it was loaded
	 * @return bool True if the file content has been modified, false otherwise
	 */
	[[nodiscard]] bool IsChanged() const { return _isChanged; }

	/**
	 * @brief Checks if the file content has been saved to disk after modification
	 * @return bool True if the file content has been saved to disk, false otherwise
	 */
	[[nodiscard]] bool IsSaved() const { return _isSaved; }

	[[nodiscard]] bool Exists() const;

private:
	fs::path _path;									///< File path

	std::vector<uint8_t> _content;					///< File content as a byte array

	Types::FileLoadState _loadState = Types::FileLoadState::NotLoaded;		///< Load state of the file (e.g., NotLoaded, Loaded, LazyLoading)
	bool _autoSave = false;							///< Flag indicating whether the file should be automatically saved to disk when modified

	mutable bool _isLoaded = false;					///< Flag indicating whether the file content was successfully loaded
	mutable bool _isChanged = false;				///< Flag indicating whether the file content has been modified since it was loaded
	mutable bool _isSaved = false;					///< Flag indicating whether the file content has been saved to disk after modification
};

}

#endif //CE_UTILITY_FILESYSTEM_FILE_HPP
