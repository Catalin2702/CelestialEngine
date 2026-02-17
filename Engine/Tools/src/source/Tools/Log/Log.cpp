//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Tools/Log/Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace CE::Tools::Log {

std::shared_ptr<spdlog::logger> Log::_s_coreLogger;
std::shared_ptr<spdlog::logger> Log::_s_clientLogger;

void Log::Init() {
#ifndef CE_DIST
	spdlog::set_pattern("%^[%T] %n: %v%$");
	_s_coreLogger = spdlog::stdout_color_mt("CelestialLogger");
	_s_clientLogger = spdlog::stdout_color_mt("AppLogger");

	if (_s_coreLogger)
		_s_coreLogger->set_level(spdlog::level::trace);
	if (_s_clientLogger)
		_s_clientLogger->set_level(spdlog::level::trace);
#endif
}

void Log::Terminate() {
#ifndef CE_DIST
	if (_s_coreLogger)
		_s_coreLogger.reset();
	if (_s_clientLogger)
		_s_clientLogger.reset();
#endif
}

}
