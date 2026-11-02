//
// Module: CelestialEngine/Engine/Modules/Tools/Log
// File: Log.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Tools/Log/Log.hpp"

#if CE_DIST
	#include <spdlog/sinks/basic_file_sink.h>
	#include <spdlog/sinks/stdout_sinks.h>

	#include <cstdlib>
	#include <filesystem>
	#include <optional>
	#include <vector>
#else
	#include <spdlog/sinks/stdout_color_sinks.h>
#endif

#include <cassert>

// The Dist log's folder and file name: the top-level project's name, or CE_LOG_NAME when set (Tools/CMakeLists.txt).
#ifndef CE_LOG_NAME
	#error "CE_LOG_NAME is not defined - it is set on CE_Tools by Engine/Modules/Tools/CMakeLists.txt."
#endif


namespace CE::Tools {

std::shared_ptr<spdlog::logger> Log::_s_coreLogger;
std::shared_ptr<spdlog::logger> Log::_s_clientLogger;

#if CE_DIST
namespace {

/// Reads an environment variable as a path, or nothing when it is unset or empty.
std::optional<std::filesystem::path> EnvPath(const char* name) {
	// std::getenv is only flagged for thread safety, and this runs once, in Init, before any thread exists.
	const char* value = std::getenv(name); // NOLINT(concurrency-mt-unsafe)
	if (not value or *value == '\0')
		return std::nullopt;
	return std::filesystem::path(value);
}

/// Where a Dist build keeps its log: the per-user log folder each OS expects. Nothing when no home can be found.
std::optional<std::filesystem::path> DistLogDirectory() {
#if CE_PLATFORM_MACOS
	if (const auto home = EnvPath("HOME"))
		return *home / "Library" / "Logs" / CE_LOG_NAME;
#elif CE_PLATFORM_WINDOWS
	if (const auto localAppData = EnvPath("LOCALAPPDATA"))
		return *localAppData / CE_LOG_NAME / "Logs";
#else
	if (const auto state = EnvPath("XDG_STATE_HOME"))
		return *state / CE_LOG_NAME;
	if (const auto home = EnvPath("HOME"))
		return *home / ".local" / "state" / CE_LOG_NAME;
#endif
	return std::nullopt;
}

/// The sinks both Dist loggers share: stderr always, the log file whenever it can be opened.
std::vector<spdlog::sink_ptr> MakeDistSinks() {
	std::vector<spdlog::sink_ptr> sinks;
	sinks.push_back(std::make_shared<spdlog::sinks::stderr_sink_mt>());

	// Init runs before main's try block and must not throw: a log file that cannot be created is a lost convenience,
	// not a reason to stop the application, so the failure only costs this one sink.
	try {
		if (const auto directory = DistLogDirectory()) {
			std::filesystem::create_directories(*directory);
			sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(
				(*directory / (CE_LOG_NAME ".log")).string(), /*truncate*/ false));
		}
	}
	catch (...) { // NOLINT(bugprone-empty-catch) - stderr is still there, and there is nowhere else to report to
	}

	return sinks;
}

std::shared_ptr<spdlog::logger> MakeDistLogger(const char* name, const std::vector<spdlog::sink_ptr>& sinks) {
	auto logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());

	// Only critical reaches the logger in Dist (Define/Log.hpp compiles the rest out); the level says the same thing
	// at runtime, so nothing below it slips through a path that does not use the macros.
	logger->set_level(spdlog::level::critical);
	logger->flush_on(spdlog::level::critical);

	// The date is part of the pattern because the file is appended to across runs.
	logger->set_pattern("[%Y-%m-%d %T.%e] [%l] %n: %v");

	spdlog::register_logger(logger);
	return logger;
}

}
#endif

void Log::Init() {
#if CE_DIST
	if (_s_coreLogger and _s_clientLogger) [[unlikely]]
		return;

	const auto sinks = MakeDistSinks();

	if (not _s_coreLogger) [[likely]]
		_s_coreLogger = MakeDistLogger("CoreLogger", sinks);

	if (not _s_clientLogger) [[likely]]
		_s_clientLogger = MakeDistLogger("ClientLogger", sinks);
#else
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
	Flush();

	spdlog::drop("CoreLogger");
	spdlog::drop("ClientLogger");

	_s_coreLogger.reset();
	_s_clientLogger.reset();
}

std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() {
	return _s_coreLogger;
}

std::shared_ptr<spdlog::logger>& Log::GetClientLogger() {
	return _s_clientLogger;
}

void Log::LogCoreCritical(const std::string& message) {
	// Asserted because calling this before Init is a caller's bug, but still checked: the assert is compiled out under
	// NDEBUG, so without the guard a Release or Dist build would dereference a null shared_ptr here instead of doing
	// nothing.
	assert(GetCoreLogger() and "CoreLogger is not initialized");
	if (not GetCoreLogger())
		return;

	GetCoreLogger()->critical(message);
}

void Log::Flush() {
	if (_s_coreLogger)
		_s_coreLogger->flush();

	if (_s_clientLogger)
		_s_clientLogger->flush();
}

}
