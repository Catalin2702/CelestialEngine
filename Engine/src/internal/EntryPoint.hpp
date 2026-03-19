//
// Module: Engine
// File: EntryPoint.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
//

#pragma once

#ifndef CE_ENTRYPOINT_HPP
#define CE_ENTRYPOINT_HPP

#include <stdexcept>

#ifdef CE_API

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
 *			The function is only defined when CE_API is defined (in engine builds).
 *			Client applications should not define their own main function.
 */
int main(const int argc, const char* argv[]) {
	int code = 0;

	Log::Log::Init();
	{
		try {
#ifdef CE_DEBUG
			Time::Chronometer chronometer;
#endif
			const auto windowProps = CMD::GetWindowProps(argc, argv);
			const auto app = Core::CreateApplication();
			app->InitAll(windowProps);
			app->Run();
		} catch ([[maybe_unused]] const std::runtime_error& _err) {
			CE_CORE_ERROR("Runtime error: {0}", _err.what());
			code = 1;
		} catch ([[maybe_unused]] const std::exception& _err) {
			CE_CORE_ERROR("Exception: {0}", _err.what());
			code = 1;
		} catch (...) {
			CE_CORE_ERROR("Unknown exception occurred");
			code = 1;
		}
	}
	Log::Log::Shutdown();

	return code;
}

#endif

#endif //CE_ENTRYPOINT_HPP
