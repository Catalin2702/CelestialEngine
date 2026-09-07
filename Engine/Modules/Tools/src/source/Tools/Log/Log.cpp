//
// Module: CelestialEngine/Engine/Modules/Tools/Log
// File: Log.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#include "Tools/Log/Log.hpp"

#if not CE_DIST
#include <spdlog/sinks/stdout_color_sinks.h>
#endif

#include <cassert>


namespace CE::Tools {

std::shared_ptr<spdlog::logger> Log::_s_coreLogger;
std::shared_ptr<spdlog::logger> Log::_s_clientLogger;

void Log::Init() {
#if not CE_DIST
	spdlog::set_pattern("%^[%T] %n: %v%$");
	if (not _s_coreLogger) [[likely]] {
		_s_coreLogger = spdlog::stdout_color_mt("CoreLogger");
		if (_s_coreLogger) [[likely]]
			_s_coreLogger->set_level(spdlog::level::trace);
	}

	if (not _s_clientLogger) [[likely]] {
		_s_clientLogger = spdlog::stdout_color_mt("ClientLogger");
		 if (_s_clientLogger) [[likely]]
			 _s_clientLogger->set_level(spdlog::level::trace);
	}
#endif
}

void Log::Shutdown() {
#if not CE_DIST
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

void Log::LogCoreError(const std::string& message) {
	// Asserted because calling this before Init is a caller's bug, but still checked: the assert is compiled out
	// under NDEBUG, and in Dist there is legitimately no logger at all - so without the guard every Release and Dist
	// build would dereference a null shared_ptr here instead of doing nothing.
	assert(GetCoreLogger() && "CoreLogger is not initialized");
	if (not GetCoreLogger())
		return;

	GetCoreLogger()->error(message);
}

void Log::Flush() {
	if (_s_coreLogger)
		_s_coreLogger->flush();

	if (_s_clientLogger)
		_s_clientLogger->flush();
}

}
