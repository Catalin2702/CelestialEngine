//
// Module: Tools/Log
// File: Log.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_LOG_LOG_HPP
#define CE_LOG_LOG_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Log.hpp"

#include <memory>
#include <spdlog/spdlog.h>
// ReSharper disable once CppUnusedIncludeDirective
#include <spdlog/fmt/ostr.h>

/**
 * @namespace CE::Tools::Log
 * @brief Logging system for the engine and client applications
 * @details Provides static loggers for both the engine core and client applications.
 *			Built on top of spdlog for high-performance logging with formatting support.
 *			Must be initialized with Init() before use and terminated with Terminate() on shutdown.
 */
namespace CE::Tools::Log {

/**
 * @class Log
 * @brief Static logging system for the engine and client applications
 * @details Provides two separate loggers: one for the engine core and one for client applications.
 *			Built on top of spdlog library for high-performance logging with formatting support.
 *			Must be initialized with Init() before use and terminated with Terminate() on shutdown.
 */
class CE_API Log {
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
	static void Terminate();

	/**
	 * @brief Gets the core logger instance
	 * @return std::shared_ptr<spdlog::logger>& Shared pointer to the core logger
	 * @details The core logger is used by the engine internals. Access it via
	 *			the CE_CORE_* logging macros (CE_CORE_INFO, CE_CORE_ERROR, etc.).
	 */
	static std::shared_ptr<spdlog::logger>& GetCoreLogger() {return _s_coreLogger; }

	/**
	 * @brief Gets the client logger instance
	 * @return std::shared_ptr<spdlog::logger>& Shared pointer to the client logger
	 * @details The client logger is used by client applications. Access it via
	 *			the CE_* logging macros (CE_INFO, CE_ERROR, etc.).
	 */
	static std::shared_ptr<spdlog::logger>& GetClientLogger() {return _s_clientLogger; }

private:
	static std::shared_ptr<spdlog::logger> _s_coreLogger;	///< Logger for engine core messages
	static std::shared_ptr<spdlog::logger> _s_clientLogger;	///< Logger for client application messages
};

}


#endif //CE_LOG_LOG_HPP
