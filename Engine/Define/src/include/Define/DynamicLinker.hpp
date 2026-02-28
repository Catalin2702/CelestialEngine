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
