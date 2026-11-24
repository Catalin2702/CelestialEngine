//
// Module: CelestialEngine/Engine/Modules/IO/FileSystem
// File: FileSystem.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-23
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#include "IO/FileSystem/FileSystem.hpp"
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


namespace CE::IO {

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

#pragma region StandardMounts

// The shipped files, one folder per owner. ce_configure_application puts them there: the two names are a contract with
// it, not a choice this file can make alone.
constexpr auto EngineFolderName = "Engine";
constexpr auto ContentFolderName = "Content";

// Reads an environment variable, returning an empty path when it is unset or empty, so the caller can fall through to
// the next candidate instead of building a path rooted at nothing.
fs::path EnvironmentPath(const char* name) {
	// Only read during startup, before anything could call setenv concurrently
	const char* value = std::getenv(name); // NOLINT(concurrency-mt-unsafe)
	if (value == nullptr or *value == '\0')
		return {};
	return {value};
}

/**
 * @brief Where the packaging puts the files shipped with the application
 * @details A bundle keeps its executable in Contents/MacOS and its read-only files in Contents/Resources, so the two are
 *			siblings rather than nested; everywhere else the resources sit beside the executable. Normalised so the ".."
 *			is resolved here and never reaches a log line.
 */
fs::path ResolvePackageDirectory(const fs::path& executableDirectory) {
#if CE_PLATFORM_MACOS
	return (executableDirectory / ".." / "Resources").lexically_normal();
#else
	return (executableDirectory / "Resources").lexically_normal();
#endif
}

/**
 * @brief The platform's per-user application data directory; empty when there is none
 * @details `~/Library/Application Support` on macOS, `%APPDATA%` on Windows, `$XDG_CONFIG_HOME` (or `~/.config`) on
 *			Linux. Deliberately outside the bundle: writing into `Contents/` breaks the code signature, and the bundle is
 *			often read-only anyway.
 */
fs::path ResolveUserDataDirectory() {
#if CE_PLATFORM_MACOS
	if (const auto home = EnvironmentPath("HOME"); not home.empty())
		return home / "Library" / "Application Support";
	return {};
#elif CE_PLATFORM_WINDOWS
	if (auto appData = EnvironmentPath("APPDATA"); not appData.empty())
		return appData;
	return EnvironmentPath("USERPROFILE");
#elif CE_PLATFORM_LINUX
	if (auto configHome = EnvironmentPath("XDG_CONFIG_HOME"); not configHome.empty())
		return configHome;
	if (const auto home = EnvironmentPath("HOME"); not home.empty())
		return home / ".config";
	return {};
#else
	#error "ResolveUserDataDirectory: Unsupported platform!";
#endif
}

/**
 * @brief The platform's per-user cache directory; empty when there is none
 * @details `~/Library/Caches` on macOS, `%LOCALAPPDATA%` on Windows, `$XDG_CACHE_HOME` (or `~/.cache`) on Linux: the
 *			places backups skip and the system may empty when it runs short of space - which is exactly the contract of
 *			a cache, and the reason it is not a subfolder of the user data directory.
 */
fs::path ResolveCacheDirectory() {
#if CE_PLATFORM_MACOS
	if (const auto home = EnvironmentPath("HOME"); not home.empty())
		return home / "Library" / "Caches";
	return {};
#elif CE_PLATFORM_WINDOWS
	return EnvironmentPath("LOCALAPPDATA");
#elif CE_PLATFORM_LINUX
	if (auto cacheHome = EnvironmentPath("XDG_CACHE_HOME"); not cacheHome.empty())
		return cacheHome;
	if (const auto home = EnvironmentPath("HOME"); not home.empty())
		return home / ".cache";
	return {};
#else
	#error "ResolveCacheDirectory: Unsupported platform!";
#endif
}

/// One folder name, so it cannot climb out of the user data directory or land in a subfolder: not empty, not "." or
/// "..", and none of the separators any of the platforms uses.
bool IsValidFolderName(const std::string_view name) {
	return not name.empty() and name != "." and name != ".." and name.find_first_of("/\\:") == std::string_view::npos;
}

/**
 * @brief Mounts <base>/<applicationName> read-write under a name, creating it first
 * @details Shared by the two per-user mounts. A failure is logged and the mount skipped, as for the packaged ones.
 */
void MountUserFolder(const std::string_view name, const fs::path& base, const std::string_view applicationName) {
	// Created before it is mounted: Mount only accepts a directory that exists, and on a first run this one does not.
	const auto directory = base / applicationName;
	std::error_code errorCode;
	fs::create_directories(directory, errorCode);
	if (const auto mounted = FileSystem::Mount(name, directory, MountAccess::ReadWrite); not mounted) [[unlikely]] {
		CE_CORE_ERROR("MountUserFolder: Cannot mount '{}://' on {} ({})!", name, directory.string(), mounted.error());
	}
}

#pragma endregion

}

void FileSystem::Initialize(const std::string_view executablePath, const std::string_view applicationName) {
	// Absolute now, against the working directory as it is at startup: GLFW chdirs into Contents/Resources on macOS,
	// and a relative argv[0] resolved after that would point somewhere else entirely.
	std::error_code error;
	const auto executableDirectory = executablePath.empty() ? fs::current_path(error) : fs::absolute(fs::path(executablePath), error).parent_path();
	if (error) [[unlikely]] {
		CE_CORE_ERROR("FileSystem::Initialize: Cannot locate the executable from '{}' ({}); no standard mount is set up.", executablePath, error.message());
		return;
	}

	// Two read-only mounts on two sibling folders, so the engine's files and the application's never share a name: an
	// application cannot replace an engine shader by shipping a file at the same path, and neither has to know what
	// the other ships.
	const auto packageDirectory = ResolvePackageDirectory(executableDirectory);

	// Braces are load-bearing: the CE_CORE_* macros expand to two statements, so a braceless if would log every time.
	if (const auto mounted = Mount(ENGINE_MOUNT, packageDirectory / EngineFolderName, MountAccess::ReadOnly); not mounted) [[unlikely]] {
		CE_CORE_WARN("FileSystem::Initialize: No engine directory for '{}://' ({}).", ENGINE_MOUNT, mounted.error());
	}

	// An application without content of its own is legitimate (a tool, the first steps of a game): a note, not a warning.
	if (const auto mounted = Mount(RESOURCES_MOUNT, packageDirectory / ContentFolderName, MountAccess::ReadOnly); not mounted) {
		CE_CORE_INFO("FileSystem::Initialize: No content directory for '{}://' ({}).", RESOURCES_MOUNT, mounted.error());
	}

	// The name becomes a folder under the user's data and cache directories: checked first, so a bad one cannot point
	// the writable mounts outside the folders that belong to this application.
	if (not IsValidFolderName(applicationName)) [[unlikely]] {
		CE_CORE_ERROR("FileSystem::Initialize: '{}' cannot name a folder; '{}://' and '{}://' are not mounted.", applicationName, CONFIG_MOUNT, CACHE_MOUNT);
		return;
	}

	// No usable home: beside the executable rather than at the root of the drive. It is not writable in every
	// deployment, but it keeps the mount on a real directory instead of "/<application>".
	auto userDataDirectory = ResolveUserDataDirectory();
	if (userDataDirectory.empty()) [[unlikely]] {
		CE_CORE_WARN("FileSystem::Initialize: No per-user data directory found, falling back to the executable's directory.");
		userDataDirectory = executableDirectory;
	}

	MountUserFolder(CONFIG_MOUNT, userDataDirectory, applicationName);

	// No cache directory: the temporary one, which has the same contract - the system may empty it - and a cache only
	// has to outlive the run that wrote it to be worth something.
	auto cacheDirectory = ResolveCacheDirectory();
	if (cacheDirectory.empty()) [[unlikely]]
		cacheDirectory = fs::temp_directory_path(error);
	if (cacheDirectory.empty()) [[unlikely]] {
		CE_CORE_WARN("FileSystem::Initialize: No cache directory found; '{}://' is not mounted.", CACHE_MOUNT);
		return;
	}

	MountUserFolder(CACHE_MOUNT, cacheDirectory, applicationName);
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

std::expected<void, FileSystemError> FileSystem::WriteBytes(const std::string_view virtualPath, const std::span<const std::byte> bytes) {
	const auto resolved = Resolve(virtualPath);
	if (not resolved) [[unlikely]]
		return std::unexpected(resolved.error());

	if (resolved->access != MountAccess::ReadWrite) [[unlikely]]
		return std::unexpected(FileSystemError::ReadOnly);

	const auto& path = resolved->native;

	// Checked by its outcome, not by the error_code: some standard libraries report "already exists" as a failure.
	std::error_code errorCode;
	fs::create_directories(path.parent_path(), errorCode);
	if (not fs::is_directory(path.parent_path())) [[unlikely]]
		return std::unexpected(FromErrorCode(errorCode, FileSystemError::WriteFailed));

	// Written beside the target, then renamed over it. A rename inside one directory replaces the file in one step, so
	// a crash or a full disk halfway through leaves the old file or the new one - never a truncated mix, which is how a
	// settings file usually gets lost.
	auto temporary = path;
	temporary += ".tmp";

	{
		std::ofstream stream{temporary, std::ios::binary | std::ios::trunc};
		if (not stream) [[unlikely]]
			return std::unexpected(FileSystemError::WriteFailed);

		stream.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));

		// close() flushes, and a flush that fails sets the failbit: checked after it, not after write().
		stream.close();
		if (not stream) [[unlikely]] {
			fs::remove(temporary, errorCode);
			return std::unexpected(FileSystemError::WriteFailed);
		}
	}

	fs::rename(temporary, path, errorCode);
	if (errorCode) [[unlikely]] {
		const auto renameError = FromErrorCode(errorCode, FileSystemError::WriteFailed);
		fs::remove(temporary, errorCode);
		return std::unexpected(renameError);
	}

	return {};
}

std::expected<void, FileSystemError> FileSystem::WriteText(const std::string_view virtualPath, const std::string_view text) {
	return WriteBytes(virtualPath, std::as_bytes(std::span(text.data(), text.size())));
}

}
