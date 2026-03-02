//
// Module: Tools/Log
// File: Log.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#include "Tools/Log/Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace CE::Tools::Log {

std::shared_ptr<spdlog::logger> Log::_s_coreLogger;
std::shared_ptr<spdlog::logger> Log::_s_clientLogger;

/**
 * @brief Initializes the logging system
 * @details Creates two separate console loggers with color output:
 *			- Core logger: for engine internal messages (prefix "CELESTIAL_ENGINE")
 *			- Client logger: for application messages (prefix "APP")
 *			Both loggers are configured to show timestamps, logger names, and log levels.
 *			Drops any existing loggers before creating new ones to allow reinitialization.
 */
void Log::Init() {
#ifndef CE_DIST
	// Drop all existing loggers to allow reinitialization
	spdlog::drop_all();

	spdlog::set_pattern("%^[%T] %n: %v%$");
	_s_coreLogger = spdlog::stdout_color_mt("CelestialLogger");
	_s_clientLogger = spdlog::stdout_color_mt("AppLogger");

	if (_s_coreLogger)
		_s_coreLogger->set_level(spdlog::level::trace);
	if (_s_clientLogger)
		_s_clientLogger->set_level(spdlog::level::trace);
#endif
}

/**
 * @brief Terminates the logging system
 * @details Flushes all pending log messages, resets logger pointers,
 *			and drops all loggers from the spdlog registry.
 */
void Log::Shutdown() {
#ifndef CE_DIST
	if (_s_coreLogger)
		_s_coreLogger.reset();
	if (_s_clientLogger)
		_s_clientLogger.reset();

	// Drop all loggers from the registry
	spdlog::drop_all();
#endif
}

}
