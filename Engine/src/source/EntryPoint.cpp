//
// Module: CelestialEngine/Engine/src/source
// File: EntryPoint.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

// The main() of every engine application. Not part of any engine library: ce_configure_application adds this file to
// the executable's own sources, because the entry point has to be in the executable itself (macOS looks for it in the
// main binary only, never in a dylib). Compiled once per application, so it sees that application's
// CE_APPLICATION_NAME. The application provides CE::CreateApplicationClient and nothing else: it neither includes this
// file nor defines a main() of its own.

#include "CelestialEngine.hpp"

// Defined by ce_configure_application on the executable this file is compiled into: the folder its user data goes in.
// Missing means the file reached a target some other way, and the application's settings would have nowhere to go.
#ifndef CE_APPLICATION_NAME
	#error "CE_APPLICATION_NAME is not defined: EntryPoint.cpp is added by ce_configure_application(), not by hand"
#endif

#include <exception>

#include <cstdio>
// ReSharper disable once CppUnusedIncludeDirective
#include <cstdlib>


namespace {

void Report(const char* message) noexcept {
	if (CE::Tools::Log::GetCoreLogger()) {
		CE::Tools::Log::LogCoreCritical(message);

		// Nobody will shut the logger down in an orderly way after a terminate: what is queued is written now or lost.
		CE::Tools::Log::Flush();
	}
	else {
		std::fputs(message, stderr);
		std::fputc('\n', stderr);
	}
}

[[noreturn]] void _OnTerminate() {
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

}

/**
 * @brief Main entry point for Celestial Engine applications
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return int Exit code (0 for success, 1 for failure)
 * @details This is the engine's main function that:
 *			1. Initializes the logging system
 *			2. Parses command-line arguments for window properties, and checks the build can honour them
 *			3. Mounts the standard file system locations (engine://, resources://, config://, cache://) for this application
 *			4. Creates the application using the factory function
 *			5. Runs the main application loop
 *			6. Cleans up and terminates logging
 *			Catches and handles exceptions thrown during initialization or runtime.
 *			Client applications should not define their own main function.
 */
int main(const int argc, const char* argv[]) {
	int code = 0;

	CE::Tools::Log::Init();
	std::set_terminate(&_OnTerminate);

	try {
#if CE_DEBUG
		const CE::Utility::Chronometer chronometer;
#endif
		// Window options into Config - validated first, so a backend this build lacks (CE_RENDER_BACKENDS) is reported
		// here with the list of what is available - and the FileSystem root from argv[0].
		CE::Core::InitFromCommandLine(argc, argv);

		// The standard mounts, located from argv[0]: before the Application, whose renderer loads its shaders through
		// them, and before GLFW, which changes the working directory a relative argv[0] is resolved against. The name
		// is this executable's, compiled into this translation unit by ce_configure_application: it keeps the user data
		// of two applications built on the engine in two folders.
		CE::IO::FileSystem::Initialize(argc > 0 and argv[0] ? argv[0] : "", CE_APPLICATION_NAME);

		CE::Core::Application app{CE::CreateApplicationClient()};
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

	CE::Tools::Log::Shutdown();

	return code;
}
