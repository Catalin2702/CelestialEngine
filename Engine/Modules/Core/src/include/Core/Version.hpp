//
// Module: CelestialEngine/Engine/Modules/Core
// File: Version.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-03
//

#pragma once

#ifndef CE_CORE_VERSION_HPP
#define CE_CORE_VERSION_HPP

#include "Define/DynamicLinker.hpp"


namespace CE {

/**
 * @brief Returns the version of the engine libraries loaded at runtime, as "MAJOR.MINOR.PATCH"
 * @details Defined in CE_Core rather than inline in a header: a header would report the version the application was
 *			compiled against, while this reports the one actually loaded - the two differ once a shared library is
 *			swapped under an existing executable. It lives in Core because CE_Engine is an INTERFACE library with no
 *			binary of its own.
 */
[[nodiscard]] CE_CORE_API const char* GetEngineVersion();

}

#endif //CE_CORE_VERSION_HPP
