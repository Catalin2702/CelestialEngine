//
// Module: CelestialEngine/Engine/Modules/Define
// File: Log.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#pragma once

#ifndef CE_DEFINE_LOG_HPP
#define CE_DEFINE_LOG_HPP

// Every macro below wraps its body in do { ... } while (0). Without it they expand to two statements
// (the assert and the log call), so a braceless `if (cond) CE_CORE_ERROR(msg);` would run the assert
// under the condition and log unconditionally - the log line appearing even when the branch is not
// taken. The disabled variants expand to the same empty do/while so that a trailing semicolon stays
// one statement in every configuration.

// Core Trace and Info logging macros - disabled in Release and Dist builds
#if CE_RELEASE || CE_DIST
	/**
	 * @brief Core engine trace logging macro (disabled in Release/Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Release and Dist builds, this macro is completely removed for performance
	 */
	#define CE_CORE_TRACE(...) do { } while (0)

	/**
	 * @brief Core engine info logging macro (disabled in Release/Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Release and Dist builds, this macro is completely removed for performance
	 */
	#define CE_CORE_INFO(...) do { } while (0)

	/**
	 * @brief Client application trace logging macro (disabled in Release/Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Release and Dist builds, this macro is completely removed for performance
	 */
	#define CE_TRACE(...) do { } while (0)

	/**
	 * @brief Client application info logging macro (disabled in Release/Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Release and Dist builds, this macro is completely removed for performance
	 */
	#define CE_INFO(...) do { } while (0)
#else
	/**
	 * @brief Core engine trace logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs trace-level messages from the engine core. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CORE_TRACE(...) do {										\
		assert(CE::Tools::Log::GetCoreLogger() && "CoreLogger is not initialized");	\
		CE::Tools::Log::GetCoreLogger()->trace(__VA_ARGS__);							\
	} while (0)

	/**
	 * @brief Core engine info logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs info-level messages from the engine core. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CORE_INFO(...) do {										\
		assert(CE::Tools::Log::GetCoreLogger() && "CoreLogger is not initialized");	\
		CE::Tools::Log::GetCoreLogger()->info(__VA_ARGS__);							\
	} while (0)

	/**
	 * @brief Client application trace logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs trace-level messages from the client application. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_TRACE(...) do {										\
		assert(CE::Tools::Log::GetClientLogger() && "ClientLogger is not initialized");	\
		CE::Tools::Log::GetClientLogger()->trace(__VA_ARGS__);							\
	} while (0)

	/**
	 * @brief Client application info logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs info-level messages from the client application. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_INFO(...) do {										\
		assert(CE::Tools::Log::GetClientLogger() && "ClientLogger is not initialized");	\
		CE::Tools::Log::GetClientLogger()->info(__VA_ARGS__);							\
	} while (0)
#endif

// Warning, Error, and Critical logging macros - disabled only in Dist builds
#if CE_DIST
	/**
	 * @brief Core engine warning logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_CORE_WARN(...) do { } while (0)

	/**
	 * @brief Core engine error logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_CORE_ERROR(...) do { } while (0)

	/**
	 * @brief Core engine critical logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_CORE_CRITICAL(...) do { } while (0)

	/**
	 * @brief Client application warning logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_WARN(...) do { } while (0)

	/**
	 * @brief Client application error logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_ERROR(...) do { } while (0)

	/**
	 * @brief Client application critical logging macro (disabled in Dist)
	 * @param ... Format string and arguments (printf-style)
	 * @details In Dist builds, this macro is completely removed
	 */
	#define CE_CRITICAL(...) do { } while (0)
#else
	/**
	 * @brief Core engine warning logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs warning-level messages from the engine core. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CORE_WARN(...) do {										\
		assert(CE::Tools::Log::GetCoreLogger() && "CoreLogger is not initialized");	\
		CE::Tools::Log::GetCoreLogger()->warn(__VA_ARGS__);							\
	} while (0)

	/**
	 * @brief Core engine error logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs error-level messages from the engine core. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CORE_ERROR(...) do {										\
		assert(CE::Tools::Log::GetCoreLogger() && "CoreLogger is not initialized");	\
		CE::Tools::Log::GetCoreLogger()->error(__VA_ARGS__);							\
	} while (0)

	/**
	 * @brief Core engine critical logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs critical-level messages from the engine core. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CORE_CRITICAL(...) do {										\
		assert(CE::Tools::Log::GetCoreLogger() && "CoreLogger is not initialized");	\
		CE::Tools::Log::GetCoreLogger()->critical(__VA_ARGS__);							\
	} while (0)

	/**
	 * @brief Client application warning logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs warning-level messages from the client application. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_WARN(...) do {										\
		assert(CE::Tools::Log::GetClientLogger() && "ClientLogger is not initialized");	\
		CE::Tools::Log::GetClientLogger()->warn(__VA_ARGS__);							\
	} while (0)

	/**
	 * @brief Client application error logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs error-level messages from the client application. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_ERROR(...) do {										\
		assert(CE::Tools::Log::GetClientLogger() && "ClientLogger is not initialized");	\
		CE::Tools::Log::GetClientLogger()->error(__VA_ARGS__);							\
	} while (0)

	/**
	 * @brief Client application critical logging macro
	 * @param ... Format string and arguments (printf-style)
	 * @details Logs critical-level messages from the client application. Includes assertion check
	 *			to ensure logger is initialized.
	 */
	#define CE_CRITICAL(...) do {										\
		assert(CE::Tools::Log::GetClientLogger() && "ClientLogger is not initialized");	\
		CE::Tools::Log::GetClientLogger()->critical(__VA_ARGS__);							\
	} while (0)
#endif

#endif //CE_DEFINE_LOG_HPP
