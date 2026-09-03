//
// Module: CelestialEngine/Engine/Modules/Utility/FileSystem
// File: FileSystem.cpp
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#include "Utility/FileSystem/FileSystem.hpp"

#include "Tools/Tools.hpp"
#include "Types/FileSystem/File.hpp"
#include "Types/Var/Vars.hpp"
#include "Utility/FileSystem/File.hpp"

#include <filesystem>
#include <fstream>
#include <vector>


namespace CE::Utility {

fs::path FileSystem::_rootDirectory = fs::current_path();

fs::path FileSystem::GetRootDirectory() { return _rootDirectory; }

void FileSystem::SetRootDirectory(const fs::path& rootDirectory) {
	// Stored absolute, resolved against the working directory as it is right now. A relative root
	// silently breaks the moment something changes the working directory afterwards - and GLFW does
	// exactly that on macOS: glfwInit() chdirs into the bundle's Contents/Resources, so a root taken
	// from a relative argv[0] would stop resolving right after the window is created.
	std::error_code error;
	auto absoluteRoot = fs::absolute(rootDirectory, error);
	if (error) [[unlikely]] {
		CE_CORE_WARN("FileSystem::SetRootDirectory: Failed to make the root directory absolute, keeping it as given: " + rootDirectory.string() + " (" + error.message() + ")");
		absoluteRoot = rootDirectory;
	}

	_rootDirectory = std::move(absoluteRoot);
}

namespace {

constexpr auto configFolderName = "CelestialEngine";

// Reads an environment variable, returning an empty path when it is unset or empty, so the caller
// can fall through to the next candidate instead of building a path rooted at nothing
fs::path EnvironmentPath(const char* name) {
	const char* value = std::getenv(name);
	if (value == nullptr or *value == '\0')
		return {};
	return {value};
}

fs::path ResolveConfigDirectory() {
#if CE_PLATFORM_MACOS
	fs::path base;
	if (const auto home = EnvironmentPath("HOME"); not home.empty())
	base = home / "Library" / "Application Support";
#elif CE_PLATFORM_WINDOWS
	auto base = EnvironmentPath("APPDATA");
	if (base.empty())
		base = EnvironmentPath("USERPROFILE");
#else
	auto base = EnvironmentPath("XDG_CONFIG_HOME");
	if (base.empty()) {
		if (const auto home = EnvironmentPath("HOME"); not home.empty())
			base = home / ".config";
	}
#endif

	// No usable home: fall back next to the executable rather than handing back an empty path. It is
	// not writable in every deployment, but it keeps callers on a valid path instead of "/CelestialEngine"
	if (base.empty()) [[unlikely]] {
		CE_CORE_WARN("FileSystem::GetConfigDirectory: No per-user data directory found, falling back to the root directory.");
		return FileSystem::GetRootDirectory() / configFolderName;
	}

	return base / configFolderName;
}

}

fs::path FileSystem::GetConfigDirectory() {
	// Resolved once: the environment does not change under the running process, and the create call
	// below only needs to happen on first use
	static const fs::path configDirectory = [] {
		auto directory = ResolveConfigDirectory();

		// The outcome is checked with exists() rather than the error_code: create_directories reports
		// "already exists" as a failure on some standard libraries, which would log an error on every
		// run but the first
		std::error_code error;
		fs::create_directories(directory, error);
		// Braces are load-bearing: CE_CORE_ERROR expands to two statements, so a braceless if would
		// log unconditionally
		if (not fs::exists(directory)) [[unlikely]] {
			CE_CORE_ERROR("FileSystem::GetConfigDirectory: Failed to create the configuration directory at path: " + directory.string() + " (" + error.message() + ")");
		}

		return directory;
	}();

	return configDirectory;
}

File FileSystem::StCreate(const fs::path& path, const std::string& content, const bool autoSave) {
	const auto fullPath = _rootDirectory / path;
	if (StExists(path)) [[unlikely]] {
		const auto message = "FileSystem::StCreate: Cannot create file because it already exists at path: " + fullPath.string();
		CE_CORE_WARN(message);
		return StLoad(path, autoSave);
	}

	std::ofstream outputFile(fullPath, std::ios::binary);
	if (not outputFile) [[unlikely]] {
		const auto errorMessage = "FileSystem::StCreate: Failed to create file at path: " + fullPath.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	outputFile.write(content.data(), static_cast<std::streamsize>(content.size()));
	if (not outputFile) [[unlikely]] {
		const auto errorMessage = "FileSystem::StCreate: Failed to write content to file at path: " + fullPath.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	File file(path, Types::FileLoadState::Loaded, autoSave);
	file.SetContent(content);
	return file;
}

File FileSystem::StNew(const fs::path& path, const std::string& content, const bool autoSave) {
	File file(path, Types::FileLoadState::Loaded, autoSave);
	file.SetContent(content);
	return file;
}

File FileSystem::StLoad(const fs::path& path, const bool autoSave) {
	const auto fullPath = _rootDirectory / path;
	if (not StExists(path)) [[unlikely]] {
		const auto errorMessage = "FileSystem::StLoad: Cannot load file because it does not exist at path: " + fullPath.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	std::ifstream inputFile(fullPath, std::ios::binary);
	if (not inputFile) [[unlikely]] {
		const auto errorMessage = "FileSystem::StLoad: Failed to open file for reading at path: " + fullPath.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	std::vector<u8> bytes(
		std::istreambuf_iterator(inputFile),
		std::istreambuf_iterator<char>{}
	);

	File file(path, Types::FileLoadState::Loaded, autoSave);
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
	if (not file._isLoaded) [[unlikely]] {
		const auto errorMessage = "FileSystem::StSave: Cannot save file because it is not loaded. File path: " + file._path.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	// A File keeps the path it was created with, which is relative to the root directory: resolve it here as every
	// other operation does, otherwise a relative path would be written next to the working directory instead
	const auto fullPath = _rootDirectory / file._path;

	std::ofstream outputFile{fullPath, std::ios::binary};
	if (not outputFile) [[unlikely]] {
		const auto errorMessage = "FileSystem::StSave: Failed to open file for writing at path: " + fullPath.string();
		CE_CORE_ERROR(errorMessage);
		throw std::runtime_error(errorMessage);
	}

	outputFile.write(reinterpret_cast<const char*>(file._content.data()), static_cast<std::streamsize>(file._content.size()));
	if (not outputFile) [[unlikely]] {
		const auto errorMessage = "FileSystem::StSave: Failed to write content to file at path: " + fullPath.string();
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
