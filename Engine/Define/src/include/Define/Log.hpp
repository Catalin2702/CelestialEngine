//
// Created by Catalin Chirosca on 2026-02-17.
//
#pragma once

#ifndef CE_DEFINE_LOG_HPP
#define CE_DEFINE_LOG_HPP

#if defined(CE_RELEASE) || defined(CE_DIST)
	#define CE_CORE_TRACE(...)
	#define CE_CORE_INFO(...)

	#define CE_TRACE(...)
	#define CE_INFO(...)
#else
	#define CE_CORE_TRACE(...) assert(CE::Tools::Log::Log::GetCoreLogger() && "CelestialLogger is not initialized");\
		CE::Tools::Log::Log::GetCoreLogger()->trace(__VA_ARGS__)
	#define CE_CORE_INFO(...) assert(CE::Tools::Log::Log::GetCoreLogger() && "CelestialLogger is not initialized");\
		CE::Tools::Log::Log::GetCoreLogger()->info(__VA_ARGS__)

	#define CE_TRACE(...) assert(CE::Tools::Log::Log::GetClientLogger() && "AppLogger is not initialized");\
		CE::Tools::Log::Log::GetClientLogger()->trace(__VA_ARGS__)
	#define CE_INFO(...) assert(CE::Tools::Log::Log::GetClientLogger() && "AppLogger is not initialized");\
		CE::Tools::Log::Log::GetClientLogger()->info(__VA_ARGS__)
#endif

#ifdef CE_DIST
	#define CE_CORE_WARN(...)
	#define CE_CORE_ERROR(...)
	#define CE_CORE_CRITICAL(...)
	#define CE_WARN(...)
	#define CE_ERROR(...)
	#define CE_CRITICAL(...)
#else
	#define CE_CORE_WARN(...) assert(CE::Tools::Log::Log::GetCoreLogger() && "CelestialLogger is not initialized");\
	CE::Tools::Log::Log::GetCoreLogger()->warn(__VA_ARGS__)
	#define CE_CORE_ERROR(...) assert(CE::Tools::Log::Log::GetCoreLogger() && "CelestialLogger is not initialized");\
	CE::Tools::Log::Log::GetCoreLogger()->error(__VA_ARGS__)
	#define CE_CORE_CRITICAL(...) assert(CE::Tools::Log::Log::GetCoreLogger() && "CelestialLogger is not initialized");\
	CE::Tools::Log::Log::GetCoreLogger()->critical(__VA_ARGS__)


	#define CE_WARN(...) assert(CE::Tools::Log::Log::GetClientLogger() && "AppLogger is not initialized");\
	CE::Tools::Log::Log::GetClientLogger()->warn(__VA_ARGS__)
	#define CE_ERROR(...) assert(CE::Tools::Log::Log::GetClientLogger() && "AppLogger is not initialized");\
	CE::Tools::Log::Log::GetClientLogger()->error(__VA_ARGS__)
	#define CE_CRITICAL(...) assert(CE::Tools::Log::Log::GetClientLogger() && "AppLogger is not initialized");\
	CE::Tools::Log::Log::GetClientLogger()->critical(__VA_ARGS__)
#endif

#endif //CE_DEFINE_LOG_HPP
