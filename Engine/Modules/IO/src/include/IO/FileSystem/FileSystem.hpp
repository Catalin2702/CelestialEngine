//
// Module: CelestialEngine/Engine/Modules/IO/FileSystem
// File: FileSystem.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-23
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#pragma once

#ifndef CE_IO_FILESYSTEM_FILESYSTEM_HPP
#define CE_IO_FILESYSTEM_FILESYSTEM_HPP

#include "FileSystemError.hpp"
#include "VirtualPath.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <cstddef>
#include <expected>
#include <filesystem>
#include <span>
#include <string>
#include <string_view>
#include <vector>


namespace CE::IO {

/// Read-only: the files the engine ships inside every application (shaders). Contents/Resources/Engine in a bundle.
inline constexpr std::string_view ENGINE_MOUNT = "engine";

/// Read-only: the files shipped with the application (shaders, textures). Inside the bundle on macOS.
inline constexpr std::string_view RESOURCES_MOUNT = "resources";

/// Read-write: what the running engine keeps for the user (settings, the ImGui layout). Outside the bundle.
inline constexpr std::string_view CONFIG_MOUNT = "config";

/// Read-write: what the application can rebuild at any time (pipeline caches, shaders compiled for this GPU), one folder
/// per application. Outside the bundle, skipped by backups, and the system may empty it: nothing here may be the only
/// copy of anything.
inline constexpr std::string_view CACHE_MOUNT = "cache";

/**
 * @enum MountAccess
 * @brief Whether a mount accepts writes
 */
enum class MountAccess: u8 {
	ReadOnly,
	ReadWrite
};

/**
 * @class FileSystem
 * @brief The engine's one way to files: virtual paths, resolved through named mounts
 * @details Every file is named as "<mount>://<relative path>" - "engine://Shaders/OpenGL/CompositeVertex.glsl",
 *			"config://imgui.ini" - and a mount maps the name to a directory on disk. Where the packaging put things,
 *			which differs between a macOS bundle and a plain folder, is answered once, by Initialize, instead of by
 *			every caller that wants a file. A mount can later point at an archive instead of a directory, and no caller
 *			would notice.
 *
 *			Nothing here throws, and nothing logs a failure: every operation that can fail returns std::expected with
 *			a FileSystemError, and the caller - who knows what the file was for - decides whether it is fatal.
 *
 *			The mount table is shared and guarded: mounting takes it exclusively, every resolution shares it. Reads
 *			and writes themselves take no lock, so two threads touching the same file is the callers' business.
 */
class CE_IO_API FileSystem {
public:
	FileSystem() = delete;

public:
	/**
	 * @brief Mounts the standard locations, from where the executable is and which application it is
	 * @param executablePath argv[0]; empty falls back to the working directory
	 * @param applicationName The folder the application's user data goes in, CE_APPLICATION_NAME: one path segment
	 * @details The packaging puts the shipped files in one directory (Contents/Resources in a bundle, Resources/ beside
	 *			the executable elsewhere), split by owner: ENGINE_MOUNT on its Engine/ folder, RESOURCES_MOUNT on its
	 *			Content/ folder, both read-only. CONFIG_MOUNT and CACHE_MOUNT go read-write on <applicationName> under
	 *			the platform's per-user data and cache directories, created if missing, so two applications built on the
	 *			engine never share settings or caches.
	 *
	 *			Called once by main(), before the Application: the path is made absolute right away, because GLFW
	 *			changes the working directory a relative argv[0] would be resolved against. A location that cannot be
	 *			mounted is logged and skipped, not fatal: the paths under it then answer UnknownMount, and the caller
	 *			that needed them reports what it was trying to do.
	 *
	 *			Only an engine application calls it. A command-line tool linking CE_IO - a shader compiler, an asset
	 *			converter - has no bundle and no user data: it mounts the directories it works on with Mount instead.
	 */
	static void Initialize(std::string_view executablePath, std::string_view applicationName);

	/**
	 * @brief Maps a mount name to a directory, replacing any mount of the same name
	 * @param name The mount name, see VirtualPath::IsValidMountName
	 * @param directory An existing directory; stored absolute
	 * @param access Whether writes through this mount are allowed
	 * @return std::expected<void, FileSystemError> InvalidPath for a bad name, NotFound / NotAFile when the directory
	 *		   is missing or is not one
	 */
	[[nodiscard]] static std::expected<void, FileSystemError> Mount(std::string_view name, const std::filesystem::path& directory, MountAccess access);

	/**
	 * @brief Removes a mount; nothing happens when there is none of that name
	 */
	static void Unmount(std::string_view name);

	[[nodiscard]] static bool IsMounted(std::string_view name);

public:
	/**
	 * @brief Resolves a virtual path to the operating system's path
	 * @return std::expected<std::filesystem::path, FileSystemError> The absolute native path - whether anything exists
	 *		   there is not checked - or InvalidPath / UnknownMount
	 * @details For the APIs that insist on opening the file themselves: Metal's library loader, ImGui's ini file. Any
	 *			code that only needs the content should read it through ReadBytes / ReadText instead, which keeps it
	 *			working when a mount stops being a directory.
	 */
	[[nodiscard]] static std::expected<std::filesystem::path, FileSystemError> ToNativePath(std::string_view virtualPath);

	/**
	 * @brief Checks whether anything exists at a virtual path; false as well when the path does not resolve
	 */
	[[nodiscard]] static bool Exists(std::string_view virtualPath);

	/**
	 * @brief Reads a whole file as raw bytes
	 * @return std::expected<std::vector<std::byte>, FileSystemError> The content, or why it could not be read
	 * @details Sized once from the file's length and filled with a single read - the binary path every asset (images,
	 *			meshes, compiled shaders) goes through.
	 */
	[[nodiscard]] static std::expected<std::vector<std::byte>, FileSystemError> ReadBytes(std::string_view virtualPath);

	/**
	 * @brief Reads a whole file as text, byte for byte: no encoding conversion, no newline translation
	 */
	[[nodiscard]] static std::expected<std::string, FileSystemError> ReadText(std::string_view virtualPath);

	/**
	 * @brief Writes a whole file, replacing it atomically
	 * @return std::expected<void, FileSystemError> ReadOnly on a read-only mount, WriteFailed (or AccessDenied /
	 *		   NotAFile) when the file could not be written
	 * @details Creates the missing parent directories, writes the content to a temporary file beside the target and
	 *			renames it over the target: a crash halfway through leaves the old file or the new one, never half of
	 *			each.
	 */
	[[nodiscard]] static std::expected<void, FileSystemError> WriteBytes(std::string_view virtualPath, std::span<const std::byte> bytes);

	/**
	 * @brief Writes a whole file from text, see WriteBytes
	 */
	[[nodiscard]] static std::expected<void, FileSystemError> WriteText(std::string_view virtualPath, std::string_view text);
};

}

#endif //CE_IO_FILESYSTEM_FILESYSTEM_HPP
