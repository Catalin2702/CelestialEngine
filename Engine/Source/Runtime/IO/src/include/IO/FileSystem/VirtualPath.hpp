//
// Module: CelestialEngine/Engine/Source/Runtime/IO/FileSystem
// File: VirtualPath.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-23
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#pragma once

#ifndef CE_IO_FILESYSTEM_VIRTUALPATH_HPP
#define CE_IO_FILESYSTEM_VIRTUALPATH_HPP

#include "FileSystemError.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <expected>
#include <string_view>


namespace CE::IO {

enum class Mount: u8;

/**
 * @struct VirtualPath
 * @brief A path as the engine spells it: the mount it lives in, and where inside that mount
 * @details "resources://Shaders/OpenGL/CompositeVertex.glsl" splits into the mount "resources" and the relative path
 *			"Shaders/OpenGL/CompositeVertex.glsl". Both are views into the string that was parsed, so a VirtualPath
 *			lives no longer than that string: it is a parsing result to use on the spot, not something to store.
 *
 *			The relative part always uses '/', on every platform, and can never leave its mount: an absolute path, a
 *			drive letter, a backslash or a ".." segment is refused here, before any operating-system path sees it.
 */
struct CE_IO_API VirtualPath {
	std::string_view mount;
	std::string_view relative;

	/**
	 * @brief Checks a mount name: not empty, and only lowercase ASCII letters, digits and '_'
	 * @details Narrow on purpose. A name is written into every path that uses it, so it must never need escaping, and
	 *			one spelling per mount ("resources", never "Resources") keeps two strings for the same file impossible.
	 */
	[[nodiscard]] static bool IsValidMountName(std::string_view name);

	/**
	 * @brief Splits a virtual path into its mount and its relative part
	 * @param path The full virtual path, e.g. "config://imgui.ini"
	 * @return std::expected<VirtualPath, FileSystemError> The two parts, or FileSystemError::InvalidPath when there is
	 *		   no "://", the mount name is not valid (see IsValidMountName), or the relative part would leave the mount
	 */
	[[nodiscard]] static std::expected<VirtualPath, FileSystemError> Parse(std::string_view path);

	[[nodiscard]] static std::string Compose(Mount mount, std::string_view path);
};

}

#endif //CE_IO_FILESYSTEM_VIRTUALPATH_HPP
