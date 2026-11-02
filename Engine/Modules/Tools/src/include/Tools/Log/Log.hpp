//
// Module: CelestialEngine/Engine/Modules/Tools/Log
// File: Log.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#pragma once

#ifndef CE_TOOLS_LOG_LOG_HPP
#define CE_TOOLS_LOG_LOG_HPP

#include "Define/Define.hpp"

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
	 *
	 *			Debug and Release log every level to the console, in colour. Dist keeps only CRITICAL (the macros below
	 *			it are compiled out, see Define/Log.hpp) and writes it twice: to stderr, for a launch from a terminal,
	 *			and appended to a file, for a launch from Finder or Explorer where stderr goes nowhere:
	 *			- macOS:   ~/Library/Logs/<name>/<name>.log
	 *			- Windows: %LOCALAPPDATA%/<name>/Logs/<name>.log
	 *			- Linux:   $XDG_STATE_HOME/<name>/<name>.log (default ~/.local/state)
	 *			<name> is fixed at build time: the top-level CMake project's name (CelestialEngine in this repository,
	 *			the game's once it includes the engine), or CE_LOG_NAME when set - see Tools/CMakeLists.txt.
	 *			Every critical message is flushed at once - it is usually the last thing the process says. A file that
	 *			cannot be opened costs only the file sink: stderr still works and Init never throws.
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
	 * @brief Logs an already-formatted message at critical level on the core logger
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
	 *
	 *			Critical rather than error because its caller is the terminate handler: the process is about to abort,
	 *			and critical is the one level Dist still records.
	 */
	static void LogCoreCritical(const std::string& message);

	/**
	 * @brief Writes out whatever the loggers still hold queued
	 * @details For the paths that end the process without ever reaching Shutdown - a terminate handler above all,
	 *			where the next call is abort() and anything still buffered is simply lost. Out-of-line for the same
	 *			reason as LogCoreCritical: spdlog stays inside CE_Tools.
	 *
	 *			A no-op before Init, which has no logger to flush.
	 */
	static void Flush();

private:
	static std::shared_ptr<spdlog::logger> _s_coreLogger;	///< Logger for engine core messages
	static std::shared_ptr<spdlog::logger> _s_clientLogger;	///< Logger for client application messages
};

}

#endif //CE_TOOLS_LOG_LOG_HPP
