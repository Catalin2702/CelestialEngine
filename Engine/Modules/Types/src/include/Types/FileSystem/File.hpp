//
// Module: CelestialEngine/Engine/Modules/Types/FileSystem
// File: File.hpp
// Created by: Catalin Chirosca
// Created: 2026-05-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_TYPES_FILESYSTEM_FILE_HPP
#define CE_TYPES_FILESYSTEM_FILE_HPP

#include <cstdint>

namespace CE::Types {

enum class FileLoadState: uint8_t {
	NotLoaded,
	Loaded,
	LazyLoading,
};

}

#endif //CE_TYPES_FILESYSTEM_FILE_HPP
