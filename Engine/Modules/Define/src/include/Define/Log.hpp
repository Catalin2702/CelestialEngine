//
// Module: CelestialEngine/Engine/Modules/Define
// File: Log.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_DEFINE_LOG_HPP
#define CE_DEFINE_LOG_HPP

// Core Trace and Info logging macros - disabled in Release and Dist builds
#if defined(CE_RELEASE) || defined(CE_DIST)
	/**
	 * @brief Core engine trace logging macro (disabled in Release/Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Release and Dist builds, this macro is completely removed for performance
	 */
	#define CE_CORE_TRACE(...)

	/**
	 * @brief Core engine info logging macro (disabled in Release/Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Release and Dist builds, this macro is completely removed for performance
	 */
	#define CE_CORE_INFO(...)

	/**
	 * @brief Client application trace logging macro (disabled in Release/Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Release and Dist builds, this macro is completely removed for performance
	 */
	#define CE_TRACE(...)

	/**
	 * @brief Client application info logging macro (disabled in Release/Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Release and Dist builds, this macro is completely removed for performance
	 */
	#define CE_INFO(...)
#else
	/**
	 * @brief Core engine trace logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs trace-level messages from the engine core. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CORE_TRACE(...) assert(CE::Tools::Log::Log::GetCoreLogger() && "CoreLogger is not initialized");\
		CE::Tools::Log::Log::GetCoreLogger()->trace(__VA_ARGS__)

	/**
	 * @brief Core engine info logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs info-level messages from the engine core. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CORE_INFO(...) assert(CE::Tools::Log::Log::GetCoreLogger() && "CoreLogger is not initialized");\
		CE::Tools::Log::Log::GetCoreLogger()->info(__VA_ARGS__)

	/**
	 * @brief Client application trace logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs trace-level messages from the client application. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_TRACE(...) assert(CE::Tools::Log::Log::GetClientLogger() && "ClientLogger is not initialized");\
		CE::Tools::Log::Log::GetClientLogger()->trace(__VA_ARGS__)

	/**
	 * @brief Client application info logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs info-level messages from the client application. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_INFO(...) assert(CE::Tools::Log::Log::GetClientLogger() && "ClientLogger is not initialized");\
		CE::Tools::Log::Log::GetClientLogger()->info(__VA_ARGS__)
#endif

// Warning, Error, and Critical logging macros - disabled only in Dist builds
#ifdef CE_DIST
	/**
	 * @brief Core engine warning logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_CORE_WARN(...)

	/**
	 * @brief Core engine error logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_CORE_ERROR(...)

	/**
	 * @brief Core engine critical logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_CORE_CRITICAL(...)

	/**
	 * @brief Client application warning logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_WARN(...)

	/**
	 * @brief Client application error logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_ERROR(...)

	/**
	 * @brief Client application critical logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_CRITICAL(...)
#else
	/**
	 * @brief Core engine warning logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs warning-level messages from the engine core. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CORE_WARN(...) assert(CE::Tools::Log::Log::GetCoreLogger() && "CoreLogger is not initialized");\
	CE::Tools::Log::Log::GetCoreLogger()->warn(__VA_ARGS__)

	/**
	 * @brief Core engine error logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs error-level messages from the engine core. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CORE_ERROR(...) assert(CE::Tools::Log::Log::GetCoreLogger() && "CoreLogger is not initialized");\
	CE::Tools::Log::Log::GetCoreLogger()->error(__VA_ARGS__)

	/**
	 * @brief Core engine critical logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs critical-level messages from the engine core. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CORE_CRITICAL(...) assert(CE::Tools::Log::Log::GetCoreLogger() && "CoreLogger is not initialized");\
	CE::Tools::Log::Log::GetCoreLogger()->critical(__VA_ARGS__)

	/**
	 * @brief Client application warning logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs warning-level messages from the client application. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_WARN(...) assert(CE::Tools::Log::Log::GetClientLogger() && "ClientLogger is not initialized");\
	CE::Tools::Log::Log::GetClientLogger()->warn(__VA_ARGS__)

	/**
	 * @brief Client application error logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs error-level messages from the client application. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_ERROR(...) assert(CE::Tools::Log::Log::GetClientLogger() && "ClientLogger is not initialized");\
	CE::Tools::Log::Log::GetClientLogger()->error(__VA_ARGS__)

	/**
	 * @brief Client application critical logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs critical-level messages from the client application. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CRITICAL(...) assert(CE::Tools::Log::Log::GetClientLogger() && "ClientLogger is not initialized");\
	CE::Tools::Log::Log::GetClientLogger()->critical(__VA_ARGS__)
#endif

#endif //CE_DEFINE_LOG_HPP
