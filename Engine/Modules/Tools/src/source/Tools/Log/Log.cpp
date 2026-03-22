//
// Module: CelestialEngine/Engine/Modules/Tools/Log
// File: Log.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
//

#include "Tools/Log/Log.hpp"

#ifndef CE_DIST
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

namespace CE::Tools::Log {

std::shared_ptr<spdlog::logger> Log::_s_coreLogger;
std::shared_ptr<spdlog::logger> Log::_s_clientLogger;

void Log::Init() {
#ifndef CE_DIST
	spdlog::set_pattern("%^[%T] %n: %v%$");
	if (not _s_coreLogger) {
		_s_coreLogger = spdlog::stdout_color_mt("CoreLogger");
		if (_s_coreLogger)
			_s_coreLogger->set_level(spdlog::level::trace);
	}

	if (not _s_clientLogger) {
		_s_clientLogger = spdlog::stdout_color_mt("ClientLogger");
		 if (_s_clientLogger)
			 _s_clientLogger->set_level(spdlog::level::trace);
	}
#endif
}

void Log::Shutdown() {
#ifndef CE_DIST
	spdlog::drop("CoreLogger");
	spdlog::drop("ClientLogger");
#endif
	_s_coreLogger.reset();
	_s_clientLogger.reset();
}

std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() {
	return _s_coreLogger;
}

std::shared_ptr<spdlog::logger>& Log::GetClientLogger() {
	return _s_clientLogger;
}

}
