//
// Module: CelestialEngine/Engine/Modules/Define
// File: Config.hpp
// Created by: Catalin Chirosca
// Created: 2026-08-25
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#pragma once

#ifndef CE_DEFINE_CONFIG_HPP
#define CE_DEFINE_CONFIG_HPP

// CMake (CE_Config, see config/cmake/CelestialEngineMacros.cmake) defines every macro below to 0 or 1 in every build, so they
// can be used both by the preprocessor and by `if constexpr`. The fallbacks exist because those two uses fail differently: an
// undefined macro is silently 0 inside #if, but a hard "use of undeclared identifier" inside `if constexpr` - which is exactly
// what a translation unit that forgot to link CE_Config would hit. Defaulting them here keeps that failure mode uniform, and
// the checks below turn a missing or contradictory definition into one readable error instead of a wrong build.

#ifndef CE_DEBUG
	#define CE_DEBUG 0
#endif

#ifndef CE_RELEASE
	#define CE_RELEASE 0
#endif

#ifndef CE_DIST
	#define CE_DIST 0
#endif

#ifndef CE_PLATFORM_WINDOWS
	#define CE_PLATFORM_WINDOWS 0
#endif

#ifndef CE_PLATFORM_MACOS
	#define CE_PLATFORM_MACOS 0
#endif

#ifndef CE_PLATFORM_LINUX
	#define CE_PLATFORM_LINUX 0
#endif

#if (CE_DEBUG + CE_RELEASE + CE_DIST) != 1
	#error "Exactly one of CE_DEBUG, CE_RELEASE, CE_DIST must be 1 - link CE_Config (or CE_Define) into this target."
#endif

#if (CE_PLATFORM_WINDOWS + CE_PLATFORM_MACOS + CE_PLATFORM_LINUX) != 1
	#error "Exactly one of CE_PLATFORM_WINDOWS, CE_PLATFORM_MACOS, CE_PLATFORM_LINUX must be 1 - CelestialEngine supports only Windows, macOS and Linux."
#endif

#endif //CE_DEFINE_CONFIG_HPP
