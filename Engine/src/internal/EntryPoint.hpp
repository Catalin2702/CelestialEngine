//
// Module: Engine
// File: EntryPoint.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_ENTRYPOINT_HPP
#define CE_ENTRYPOINT_HPP

#include <format>
#include <stdexcept>


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
	{
		try {
#ifdef CE_DEBUG
			Utility::Chronometer chronometer;
#endif
			Utility::FileSystem::SetRootDirectory(fs::path(argv[0]).parent_path());
			const auto app = Core::CreateApplication(argc, argv);
			app->Start();
		} catch ([[maybe_unused]] const std::runtime_error& _err) {
			// Formatted with std::format and logged via Log::LogCoreError (not the CE_CORE_ERROR
			// macro) so this file - compiled directly into the client executable - never touches
			// spdlog/fmt's own template machinery; see LogCoreError's doc comment for why.
			Tools::Log::LogCoreError(std::format("Runtime error: {0}", _err.what()));
			code = 1;
		} catch ([[maybe_unused]] const std::exception& _err) {
			Tools::Log::LogCoreError(std::format("Exception: {0}", _err.what()));
			code = 1;
		} catch (...) {
			Tools::Log::LogCoreError("Unknown exception occurred");
			code = 1;
		}
	}
	Tools::Log::Shutdown();

	return code;
}

#endif //CE_ENTRYPOINT_HPP
