//
// Module: CelestialEngine/Engine/Modules/Tools/Log
// File: Log.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_TOOLS_LOG_LOG_HPP
#define CE_TOOLS_LOG_LOG_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Log.hpp"

#include <memory>
#include <spdlog/spdlog.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <spdlog/fmt/ostr.h>


namespace CE::Tools {

/**
 * @class Log
 * @brief Static logging system for the engine and client applications
 * @details Provides two separate loggers: one for the engine core and one for client applications.
 *			Built on top of spdlog library for high-performance logging with formatting support.
 *			Must be initialized with Init() before use and terminated with Terminate() on shutdown.
 */
class CE_TOOLS_API Log {
public:
	/**
	 * @brief Initializes the logging system
	 * @details Creates and configures both core and client loggers with appropriate
	 *			formatting patterns and output sinks. Must be called before using any
	 *			logging macros (CE_CORE_*, CE_*).
	 */
	static void Init();

	/**
	 * @brief Terminates the logging system
	 * @details Flushes all pending log messages and releases logger resources.
	 *			Should be called during application shutdown.
	 */
	static void Shutdown();

	/**
	 * @brief Gets the core logger instance
	 * @return std::shared_ptr<spdlog::logger>& Shared pointer to the core logger
	 * @details The core logger is used by the engine internals. Access it via
	 *			the CE_CORE_* logging macros (CE_CORE_INFO, CE_CORE_ERROR, etc.).
	 */
	static std::shared_ptr<spdlog::logger>& GetCoreLogger();

	/**
	 * @brief Gets the client logger instance
	 * @return std::shared_ptr<spdlog::logger>& Shared pointer to the client logger
	 * @details The client logger is used by client applications. Access it via
	 *			the CE_* logging macros (CE_INFO, CE_ERROR, etc.).
	 */
	static std::shared_ptr<spdlog::logger>& GetClientLogger();

	/**
	 * @brief Logs an already-formatted message at error level on the core logger
	 * @param message The message to log, formatted by the caller
	 * @details Deliberately out-of-line (defined in Log.cpp) instead of the usual
	 *			CE_CORE_* macros: those expand to a call into spdlog's templated,
	 *			header-only formatting API, which gets compiled straight into whichever
	 *			translation unit uses them - including EntryPoint.hpp's main(), which is
	 *			compiled directly into the client executable. That pulls a direct (non
	 *			delay-loadable) import of spdlog/fmt's own DLL into the executable itself.
	 *			Routing through this plain, compiled function keeps that call inside
	 *			CE_Tools instead, which the executable already reaches through the
	 *			regular (delay-loaded) module DLL machinery.
	 */
	static void LogCoreError(const std::string& message);

private:
	static std::shared_ptr<spdlog::logger> _s_coreLogger;	///< Logger for engine core messages
	static std::shared_ptr<spdlog::logger> _s_clientLogger;	///< Logger for client application messages
};

}

#endif //CE_TOOLS_LOG_LOG_HPP
