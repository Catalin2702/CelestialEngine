//
// Module: CelestialEngine/Engine/Modules/Utility/FileSystem
// File: FileSystem.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-05-16
//

#include "Utility/FileSystem/FileSystem.hpp"

#include "Tools/Log/Log.hpp"
#include "Types/FileSystem/File.hpp"
#include "Utility/FileSystem/File.hpp"

#include <filesystem>
#include <fstream>
#include <vector>

namespace CE::Utility::FileSystem {

fs::path FileSystem::_rootDirectory = fs::current_path();

File FileSystem::StCreate(const fs::path& path, const std::string& content, const bool autoSave) {
	const auto fullPath = _rootDirectory / path;
	if (StExists(path)) {
		const auto message = "FileSystem::StCreate: Cannot create file because it already exists at path: " + fullPath.string();
		CE_CORE_WARN(message);
		return StLoad(path, autoSave);
	}

	std::ofstream outputFile(fullPath, std::ios::binary);
	if (not outputFile) {
		const auto errorMessage = "FileSystem::StCreate: Failed to create file at path: " + fullPath.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	outputFile.write(content.data(), static_cast<std::streamsize>(content.size()));
	if (not outputFile) {
		const auto errorMessage = "FileSystem::StCreate: Failed to write content to file at path: " + fullPath.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	File file(path, CE::FileSystem::FileLoadState::Loaded, autoSave);
	file.SetContent(content);
	return file;
}

File FileSystem::StNew(const fs::path& path, const std::string& content, const bool autoSave) {
	File file(path, CE::FileSystem::FileLoadState::Loaded, autoSave);
	file.SetContent(content);
	return file;
}

File FileSystem::StLoad(const fs::path& path, const bool autoSave) {
	const auto fullPath = _rootDirectory / path;
	if (not StExists(fullPath)) {
		const auto errorMessage = "FileSystem::StLoad: Cannot load file because it does not exist at path: " + fullPath.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	std::ifstream inputFile(fullPath, std::ios::binary);
	if (not inputFile) {
		const auto errorMessage = "FileSystem::StLoad: Failed to open file for reading at path: " + fullPath.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	std::vector<uint8_t> bytes(
		std::istreambuf_iterator(inputFile),
		std::istreambuf_iterator<char>{}
	);

	File file(path, CE::FileSystem::FileLoadState::Loaded, autoSave);
	file.SetContent(std::move(bytes));
	return file;
}

File FileSystem::StLoad(File& file) {
	if (not file._isLoaded)
		return StReload(file);
	return file;
}

File FileSystem::StReload(File& file) {
	File reloadedFile = StLoad(file._path, file._autoSave);
	file = std::move(reloadedFile);
	return file;
}

File FileSystem::StUnload(File& file) {
	file._content.clear();
	file._content.shrink_to_fit();
	return file;
}

void FileSystem::StSave(const File& file) {
	if (not file._isLoaded) {
		const auto errorMessage = "FileSystem::StSave: Cannot save file because it is not loaded. File path: " + file._path.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	std::ofstream outputFile(file._path, std::ios::binary);
	if (not outputFile) {
		const auto errorMessage = "FileSystem::StSave: Failed to open file for writing at path: " + file._path.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	outputFile.write(reinterpret_cast<const char*>(file._content.data()), static_cast<std::streamsize>(file._content.size()));
	if (not outputFile) {
		const auto errorMessage = "FileSystem::StSave: Failed to write content to file at path: " + file._path.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}
}

bool FileSystem::StExists(const fs::path& path) {
	const auto fullPath = _rootDirectory / path;
	return fs::exists(fullPath);
}

bool FileSystem::StExists(const File& file) {
	return StExists(file._path);
}

}
