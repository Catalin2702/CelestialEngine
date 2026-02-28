//
// Module: Engine
// File: EntryPoint.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-15
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_ENTRYPOINT_HPP
#define CE_ENTRYPOINT_HPP

#ifdef CE_API

/**
 * @brief Main entry point for Celestial Engine applications
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return int Exit code (0 for success)
 * @details This is the engine's main function that:
 *          1. Initializes the logging system
 *          2. Parses command-line arguments for window properties
 *          3. Creates the application using the factory function
 *          4. Runs the main application loop
 *          5. Cleans up and terminates logging
 *          The function is only defined when CE_API is defined (in engine builds).
 *          Client applications should not define their own main function.
 */
int main(const int argc, char* argv[]) {
	Log::Log::Init();
	{
		[[maybe_unused]] Time::Chronometer timer;
		const auto app = Core::CreateApplication(CMD::GetWindowProps(argc, argv));
		app->Run();
		delete app;
	}
	Log::Log::Terminate();
}

#endif

#endif //CE_ENTRYPOINT_HPP
