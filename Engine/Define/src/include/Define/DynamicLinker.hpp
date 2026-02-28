//
// Created by Catalin Chirosca on 2026-02-17.
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
