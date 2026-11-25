//
// Module: CelestialEngine/Engine/Modules/IO/FileSystem
// File: VirtualPath.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-23
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

#include "IO/FileSystem/VirtualPath.hpp"
#include "IO/FileSystem/FileSystem.hpp"

#include <algorithm>
#include <ranges>


namespace CE::IO {

namespace {

/// What separates the mount from the relative part. Spelled like a URL scheme because that is what it reads as.
constexpr std::string_view MountSeparator = "://";

constexpr bool IsMountCharacter(const char character) {
	return (character >= 'a' and character <= 'z') or (character >= '0' and character <= '9') or (character == '_');
}

/**
 * @brief Checks the relative part cannot reach outside the mount it is appended to
 * @details Done on the string as written, segment by segment, rather than by normalising it and checking the result:
 *			"Textures/../Shaders" would land inside the mount, but a path the engine has to rewrite before it can trust
 *			it is not one worth accepting. ':' and '\' are refused because they mean something on Windows - a drive, a
 *			separator - that '/' does not, and a path must resolve the same way on every platform.
 */
bool StaysInsideMount(const std::string_view relative) {
	if (relative.starts_with('/') or relative.contains('\\') or relative.contains(':')) [[unlikely]]
		return false;

	return std::ranges::none_of(relative | std::views::split('/'), [](const auto segment) {
		return std::string_view(segment.begin(), segment.end()) == "..";
	});
}

}

bool VirtualPath::IsValidMountName(std::string_view name) {
	return not name.empty() and std::ranges::all_of(name, IsMountCharacter);
}

std::expected<VirtualPath, FileSystemError> VirtualPath::Parse(std::string_view path) {
	const auto separator = path.find(MountSeparator);
	if (separator == std::string_view::npos) [[unlikely]]
		return std::unexpected(FileSystemError::InvalidPath);

	const auto mount = path.substr(0, separator);
	const auto relative = path.substr(separator + MountSeparator.size());

	if (not IsValidMountName(mount) or not StaysInsideMount(relative)) [[unlikely]]
		return std::unexpected(FileSystemError::InvalidPath);

	return VirtualPath{
		.mount = mount,
		.relative = relative
	};
}

std::string VirtualPath::Compose(const Mount mount, const std::string_view path) {
	return std::format("{}://{}", mount, path);
}

}
