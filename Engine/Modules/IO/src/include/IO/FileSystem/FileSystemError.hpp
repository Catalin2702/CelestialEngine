//
// Module: CelestialEngine/Engine/Modules/IO/FileSystem
// File: FileSystemError.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-23
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#pragma once

#ifndef CE_IO_FILESYSTEM_FILESYSTEMERROR_HPP
#define CE_IO_FILESYSTEM_FILESYSTEMERROR_HPP

#include "Types/Var/Vars.hpp"

#include <format>
#include <string_view>

namespace CE::IO {

/**
 * @enum FileSystemError
 * @brief Why a FileSystem operation failed
 * @details Returned through std::expected, never thrown: a missing file is an answer a loader has to be ready for, not
 *			an exceptional state, and only the caller knows whether it is fatal (a shader) or expected (a settings file
 *			on the first run). It carries no message on purpose: the caller adds the context it has - which asset, for
 *			what - and logs once, instead of the file system logging a path nobody asked about.
 */
enum class FileSystemError: u8 {
	Unknown = 0,
	InvalidPath, ///< Not "<mount>://<relative>", or the relative part leaves its mount (absolute, "..")
	UnknownMount, ///< The mount named before "://" was never mounted
	ReadOnly, ///< A write aimed at a read-only mount
	NotFound, ///< Nothing exists at the resolved path
	NotAFile, ///< Something exists there, but it is a directory (or another non-regular file)
	AccessDenied, ///< The operating system refused the access
	ReadFailed, ///< The file opened, but reading it failed or came up short
	WriteFailed, ///< Creating, writing or replacing the file failed
};

/**
 * @brief Names a FileSystemError, for fmt/spdlog and - through the formatter below - for std::format
 * @param error The value to name
 * @return std::string_view The enumerator's name, or "Unknown" for a value outside the enum
 */
constexpr std::string_view format_as(const FileSystemError error) {
	switch (error) {
		case FileSystemError::InvalidPath: return "InvalidPath";
		case FileSystemError::UnknownMount: return "UnknownMount";
		case FileSystemError::ReadOnly: return "ReadOnly";
		case FileSystemError::NotFound: return "NotFound";
		case FileSystemError::NotAFile: return "NotAFile";
		case FileSystemError::AccessDenied: return "AccessDenied";
		case FileSystemError::ReadFailed: return "ReadFailed";
		case FileSystemError::WriteFailed: return "WriteFailed";
		case FileSystemError::Unknown:
		default:
			return "Unknown";
	}
}

}

template <>
struct std::formatter<CE::IO::FileSystemError>: std::formatter<std::string_view> {
	auto format(const CE::IO::FileSystemError value, std::format_context& ctx) const {
		return std::formatter<std::string_view>::format(format_as(value), ctx);
	}
};

#endif //CE_IO_FILESYSTEM_FILESYSTEMERROR_HPP
