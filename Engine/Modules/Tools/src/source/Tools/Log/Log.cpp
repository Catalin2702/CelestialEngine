//
// Module: Tools/Log
// File: Log.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#include "Tools/Log/Log.hpp"

#ifndef CE_DIST
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

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
	spdlog::set_pattern("%^[%T] %n: %v%$");
	if (not _s_coreLogger) {
		_s_coreLogger = spdlog::stdout_color_mt("CelestialLogger");
		if (_s_coreLogger)
			_s_coreLogger->set_level(spdlog::level::trace);
	}

	if (not _s_clientLogger) {
		_s_clientLogger = spdlog::stdout_color_mt("AppLogger");
		 if (_s_clientLogger)
			 _s_clientLogger->set_level(spdlog::level::trace);
	}
#endif
}

/**
 * @brief Terminates the logging system
 * @details Flushes all pending log messages, resets logger pointers,
 *			and drops all loggers from the spdlog registry.
 */
void Log::Shutdown() {
#ifndef CE_DIST
	spdlog::drop("CelestialLogger");
	spdlog::drop("AppLogger");
#endif
}

}
