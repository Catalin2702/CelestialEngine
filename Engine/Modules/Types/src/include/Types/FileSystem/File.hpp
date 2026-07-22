//
// Module: CelestialEngine/Engine/Modules/Types/FileSystem
// File: File.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
//

#pragma once

#ifndef CE_TYPES_FILESYSTEM_FILE_HPP
#define CE_TYPES_FILESYSTEM_FILE_HPP

#include <cstdint>

namespace CE::Types {

/**
 * @brief Enumeration of the loading states of a file managed by the FileSystem
 * @details NotLoaded: only the path is known; Loaded: the content is in memory; LazyLoading: the content will be
 *			loaded on first access.
 */
enum class FileLoadState: uint8_t {
	NotLoaded,
	Loaded,
	LazyLoading,
};

}

#endif //CE_TYPES_FILESYSTEM_FILE_HPP
