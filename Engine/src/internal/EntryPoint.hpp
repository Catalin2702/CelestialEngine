//
// Module: Engine
// File: EntryPoint.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-09-07
//

#pragma once

#ifndef CE_ENTRYPOINT_HPP
#define CE_ENTRYPOINT_HPP

#include <exception>

#include <cstdio>
#include <cstdlib>


static void Report(const char* message) noexcept {
	if (Tools::Log::GetCoreLogger()) {
		Tools::Log::LogCoreError(message);

		// Nobody will shut the logger down in an orderly way after a terminate: what is queued is written now or lost.
		Tools::Log::Flush();
	}
	else {
		std::fputs(message, stderr);
		std::fputc('\n', stderr);
	}
}

[[noreturn]] static void _OnTerminate() {
	if (const auto inFlight = std::current_exception()) {
		try {
			std::rethrow_exception(inFlight);
		}
		catch (const std::exception& exception) {
			Report(exception.what());
		}
		catch (...) {
			Report("Terminate on an object that does not derive from std::exception.");
		}
	}
	else {
		// std::terminate reached without an exception: an explicit call, or the destructor of a std::thread that
		// was still joinable. Silent here would mean dying on SIGABRT with nothing written anywhere.
		Report("Terminate with no exception in flight.");
	}

	std::abort();
}

/**
 * @brief Main entry point for Celestial Engine applications
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return int Exit code (0 for success, 1 for failure)
 * @details This is the engine's main function that:
 *			1. Initializes the logging system
 *			2. Parses command-line arguments for window properties
 *			3. Creates the application using the factory function
 *			4. Runs the main application loop
 *			5. Cleans up and terminates logging
 *			Catches and handles exceptions thrown during initialization or runtime.
 *			Client applications should not define their own main function.
 */
int main(const int argc, const char* argv[]) {
	int code = 0;

	Tools::Log::Init();
	std::set_terminate(&_OnTerminate);

	try {
#if CE_DEBUG
		Utility::Chronometer chronometer;
#endif
		Utility::Config::SetWindowProps(Utility::GetWindowProps(argc, argv));
		Utility::FileSystem::SetRootDirectory(fs::path(argv[0]).parent_path());
		Core::Application app{};
		app.InitImguiLayer();
		app.Start();
	}
	catch (const std::exception& _err) {
		Report(_err.what());
		code = 1;
	}
	catch (...) {
		Report("Unknown exception: the thrown object does not derive from std::exception.");
		code = 1;
	}

	Tools::Log::Shutdown();

	return code;
}

#endif //CE_ENTRYPOINT_HPP
