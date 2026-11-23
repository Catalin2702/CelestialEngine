//
// Module: CelestialEngine/Engine/Modules/Core/FileSystem
// File: FileSystem.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-23
// Updated by: Catalin Chirosca
// Updated: 2026-11-23
//

#include "Core/FileSystem/FileSystem.hpp"
#include "Define/Config.hpp"
#include "Tools/Log/Log.hpp"

#include <cstdlib>
#include <format>
#include <fstream>
#include <functional>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <system_error>
#include <utility>


namespace CE::Core {

namespace fs = std::filesystem;

namespace {

#pragma region MountTable

struct MountPoint {
	fs::path directory;
	MountAccess access = MountAccess::ReadOnly;
};

struct MountTable {
	std::shared_mutex mutex;
	// std::less<> makes the lookups heterogeneous: find() takes the std::string_view a VirtualPath holds, without
	// building a std::string for every resolution.
	std::map<std::string, MountPoint, std::less<>> mounts;
};

struct ResolvedPath {
	fs::path native;
	MountAccess access = MountAccess::ReadOnly;
};

MountTable& Mounts() {
	// Built on first use rather than at load time: nothing guarantees that a static initialiser elsewhere will not
	// ask for a file before this translation unit's statics exist.
	static MountTable mountTable;
	return mountTable;
}

std::expected<ResolvedPath, FileSystemError> Resolve(const std::string_view virtualPath) {
	const auto parsed = VirtualPath::Parse(virtualPath);
	if (not parsed) [[unlikely]]
		return std::unexpected(parsed.error());

	auto& [mutex, mounts] = Mounts();
	const std::shared_lock lock{mutex};

	const auto mount = mounts.find(parsed->mount);
	if (mount == mounts.end()) [[unlikely]]
		return std::unexpected(FileSystemError::UnknownMount);

	return ResolvedPath{
		.native = mount->second.directory / parsed->relative,
		.access = mount->second.access
	};
}

#pragma endregion

#pragma region Errors

FileSystemError FromErrorCode(const std::error_code& error, const FileSystemError fallback) {
	if (error == std::errc::no_such_file_or_directory)
		return FileSystemError::NotFound;

	if (error == std::errc::permission_denied or error == std::errc::operation_not_permitted)
		return FileSystemError::AccessDenied;

	if (error == std::errc::is_a_directory)
		return FileSystemError::NotAFile;

	return fallback;
}

/// Answers "is there a regular file here", each way of saying no with its own error.
std::expected<void, FileSystemError> CheckRegularFile(const fs::path& path) {
	std::error_code errorCode;
	const auto status = fs::status(path, errorCode);

	// A missing file is reported both ways - type not_found and an error_code - so the type is checked first.
	if (status.type() == fs::file_type::not_found)
		return std::unexpected(FileSystemError::NotFound);
	if (errorCode) [[unlikely]]
		return std::unexpected(FromErrorCode(errorCode, FileSystemError::ReadFailed));
	if (not fs::is_regular_file(status))
		return std::unexpected(FileSystemError::NotAFile);

	return {};
}

#pragma endregion

#pragma region Reading

/**
 * @brief Reads a whole file into a contiguous container of one-byte elements
 * @details std::vector<std::byte> for assets, std::string for text: both sized once from the file's length and filled
 *			by a single read. Reading through std::istreambuf_iterator instead copies one character at a time and grows
 *			the buffer as it goes - fine for an ini file, slow for a 30 MB texture.
 */
template <typename Container>
std::expected<Container, FileSystemError> ReadWhole(const std::string_view virtualPath) {
	const auto resolved = Resolve(virtualPath);
	if (not resolved) [[unlikely]]
		return std::unexpected(resolved.error());

	const auto& path = resolved->native;
	if (const auto isFile = CheckRegularFile(path); not isFile)
		return std::unexpected(isFile.error());

	std::error_code error;
	const auto size = fs::file_size(path, error);
	if (error) [[unlikely]]
		return std::unexpected(FromErrorCode(error, FileSystemError::ReadFailed));

	std::ifstream stream(path, std::ios::binary);
	if (not stream) [[unlikely]]
		return std::unexpected(FileSystemError::ReadFailed);

	Container content(static_cast<std::size_t>(size), typename Container::value_type{});
	stream.read(reinterpret_cast<char*>(content.data()), static_cast<std::streamsize>(content.size()));

	// Shorter than file_size said: the file shrank between the two calls, or the read failed. Either way the content
	// is not the file, and handing it back as if it were would be worse than an error.
	if (stream.gcount() != static_cast<std::streamsize>(content.size())) [[unlikely]]
		return std::unexpected(FileSystemError::ReadFailed);

	return content;
}

#pragma endregion

}

std::expected<void, FileSystemError> FileSystem::Mount(const std::string_view name, const std::filesystem::path& directory, MountAccess access) {
	if (not VirtualPath::IsValidMountName(name)) [[unlikely]]
		return std::unexpected(FileSystemError::InvalidPath);

	// Absolute and normalised once, here, so every resolution afterwards is a plain append - and so a relative
	// directory cannot silently follow the working directory around.
	std::error_code errorCode;
	auto absoluteDirectory = fs::absolute(directory, errorCode).lexically_normal();
	if (errorCode) [[unlikely]]
		return std::unexpected(FileSystemError::InvalidPath);

	const auto status = fs::status(absoluteDirectory, errorCode);
	if (status.type() == fs::file_type::not_found)
		return std::unexpected(FileSystemError::NotFound);
	if (errorCode) [[unlikely]]
		return std::unexpected(FromErrorCode(errorCode, FileSystemError::NotFound));
	if (not fs::is_directory(status))
		return std::unexpected(FileSystemError::NotAFile);

	auto& [mutex, mounts] = Mounts();
	const std::unique_lock lock{mutex};
	mounts.insert_or_assign(std::string(name), MountPoint{
		.directory = std::move(absoluteDirectory),
		.access = access
	});

	return {};
}

void FileSystem::Unmount(const std::string_view name) {
	auto& [mutex, mounts] = Mounts();
	const std::unique_lock lock{mutex};

	if (const auto mount = mounts.find(name); mount != mounts.end())
		mounts.erase(mount);
}

bool FileSystem::IsMounted(const std::string_view name) {
	auto& [mutex, mounts] = Mounts();
	const std::shared_lock lock{mutex};

	return mounts.contains(name);
}

std::expected<std::filesystem::path, FileSystemError> FileSystem::ToNativePath(const std::string_view virtualPath) {
	return Resolve(virtualPath).transform([](ResolvedPath resolved) {
		return std::move(resolved.native);
	});
}

bool FileSystem::Exists(const std::string_view virtualPath) {
	const auto resolved = Resolve(virtualPath);
	std::error_code errorCode;
	return resolved and fs::exists(resolved->native, errorCode);
}

std::expected<std::vector<std::byte>, FileSystemError> FileSystem::ReadBytes(const std::string_view virtualPath) {
	return ReadWhole<std::vector<std::byte>>(virtualPath);
}

std::expected<std::string, FileSystemError> FileSystem::ReadText(const std::string_view virtualPath) {
	return ReadWhole<std::string>(virtualPath);
}

}
