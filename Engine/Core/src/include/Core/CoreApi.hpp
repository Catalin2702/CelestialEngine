//
// Created by Catalin Chirosca on 2026-02-15.
//

#ifndef CELESTIALENGINE_COREAPI_HPP
#define CELESTIALENGINE_COREAPI_HPP

#if defined(_WIN32)
	#ifdef CORE_EXPORTS
		#define CORE_API __declspec(dllexport)
	#else
		#define CORE_API __declspec(dllimport)
	#endif
#else
	#define CORE_API __attribute__((visibility("default")))
#endif

#endif //CELESTIALENGINE_COREAPI_HPP