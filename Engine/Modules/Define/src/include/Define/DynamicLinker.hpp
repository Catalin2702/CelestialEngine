//
// Module: CelestialEngine/Engine/Modules/Define
// File: DynamicLinker.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_DEFINE_DYNAMICLINKER_HPP
#define CE_DEFINE_DYNAMICLINKER_HPP

/**
 * @def CE_API_EXPORT / CE_API_IMPORT
 * @brief Platform-specific building blocks for exporting/importing symbols from shared libraries
 * @details On Windows: __declspec(dllexport) / __declspec(dllimport).
 *			On macOS/Linux: __attribute__((visibility("default"))) for both, since ELF/Mach-O
 *			shared libraries do not distinguish export from import at the symbol declaration.
 */
#if defined(CE_PLATFORM_WINDOWS)
	#define CE_API_EXPORT __declspec(dllexport)
	#define CE_API_IMPORT __declspec(dllimport)
#elif defined(CE_PLATFORM_MACOS) || defined(CE_PLATFORM_LINUX)
	#define CE_API_EXPORT __attribute__((visibility("default")))
	#define CE_API_IMPORT __attribute__((visibility("default")))
#else
	#error CelestialEngine supports only Windows, MacOS and Linux
#endif

/**
 * @def CE_CORE_API / CE_EVENTS_API / CE_TOOLS_API / CE_TYPES_API / CE_UTILITY_API / CE_APPLE_API
 * @brief Per-module export/import macros
 * @details Each shared-library module defines its own CE_<MODULE>_EXPORTS (PRIVATE compile
 *			definition) only while building that module. Every other module, which only ever
 *			imports the symbols, leaves it undefined and gets CE_API_IMPORT instead. A single
 *			shared macro cannot work here: a module that exports its own classes would also
 *			(incorrectly) export classes it merely includes from another module.
 */
#ifdef CE_CORE_EXPORTS
	#define CE_CORE_API CE_API_EXPORT
#else
	#define CE_CORE_API CE_API_IMPORT
#endif

#ifdef CE_EVENTS_EXPORTS
	#define CE_EVENTS_API CE_API_EXPORT
#else
	#define CE_EVENTS_API CE_API_IMPORT
#endif

#ifdef CE_TOOLS_EXPORTS
	#define CE_TOOLS_API CE_API_EXPORT
#else
	#define CE_TOOLS_API CE_API_IMPORT
#endif

#ifdef CE_TYPES_EXPORTS
	#define CE_TYPES_API CE_API_EXPORT
#else
	#define CE_TYPES_API CE_API_IMPORT
#endif

#ifdef CE_UTILITY_EXPORTS
	#define CE_UTILITY_API CE_API_EXPORT
#else
	#define CE_UTILITY_API CE_API_IMPORT
#endif

#ifdef CE_APPLE_EXPORTS
	#define CE_APPLE_API CE_API_EXPORT
#else
	#define CE_APPLE_API CE_API_IMPORT
#endif

#endif //CE_DEFINE_DYNAMICLINKER_HPP
