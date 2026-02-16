//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Tools/Log/Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace CE::Tools::Log {

std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init() {
#ifndef CE_BUILD
	spdlog::set_pattern("%^[%T] %n: %v%$");
	s_CoreLogger = spdlog::stdout_color_mt("CelestialLogger");
	s_ClientLogger = spdlog::stdout_color_mt("AppLogger");

	if (s_CoreLogger)
		s_CoreLogger->set_level(spdlog::level::trace);
	if (s_ClientLogger)
		s_ClientLogger->set_level(spdlog::level::trace);
#endif
}

void Log::Terminate() {
#ifndef CE_BUILD
	if (s_CoreLogger)
		s_CoreLogger.reset();
	if (s_ClientLogger)
		s_ClientLogger.reset();
#endif
}

}
