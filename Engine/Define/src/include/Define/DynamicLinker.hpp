//
// Module: Define
// File: DynamicLinker.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-17
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_DEFINE_DYNAMICLINKER_HPP
#define CE_DEFINE_DYNAMICLINKER_HPP

/**
 * @def CE_API
 * @brief Platform-specific macro for exporting/importing symbols from shared libraries
 * @details On Windows: Uses __declspec(dllexport) when building the engine (CE_CORE_EXPORTS defined),
 *			and __declspec(dllimport) when using the engine.
 *			On macOS/Linux: Uses __attribute__((visibility("default"))) to make symbols visible.
 *			This macro must be used on classes and functions that need to be accessible
 *			from outside the shared library.
 */
#if defined(CE_PLATFORM_WINDOWS)
	#ifdef CE_CORE_EXPORTS
		#define CE_API __declspec(dllexport)
	#else
		#define CE_API __declspec(dllimport)
	#endif
#elif defined(CE_PLATFORM_MACOS) || defined(CE_PLATFORM_LINUX)
	#define CE_API __attribute__((visibility("default")))
#else
	#error CelestialEngine supports only Windows, MacOS and Linux
#endif

#endif //CE_DEFINE_DYNAMICLINKER_HPP
