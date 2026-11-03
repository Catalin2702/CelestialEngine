//
// Module: CelestialEngine/Engine/Modules/Utility/CMD
// File: CommandLine.hpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-03
//

#pragma once

#ifndef CE_UTILITY_CMD_COMMANDLINE_HPP
#define CE_UTILITY_CMD_COMMANDLINE_HPP

#include "Define/DynamicLinker.hpp"


namespace CE::Utility {

/**
 * @brief Sets up the process-wide state that comes from the command line, before anything else is built
 * @param argc Number of command-line arguments, as received by main()
 * @param argv Command-line arguments, as received by main(); argv[0] is the executable's path
 * @throws std::runtime_error When an option has an invalid value (ParseWindowArgs), or when the build cannot honour
 *		   the graphics/window APIs asked for (ValidateWindowProps). Nothing is stored in that case.
 * @details In order:
 *			1. parses the window options (ParseWindowArgs) and checks this build can open that window
 *			   (ValidateWindowProps) - so a backend missing from CE_RENDER_BACKENDS is reported here, with the list of
 *			   what is available, rather than by a factory later on;
 *			2. stores them in Config, where the rest of the engine reads them (Config::GetWindowProps);
 *			3. sets the FileSystem root to the executable's directory, from argv[0].
 *			Called once by main() (EntryPoint.hpp), after the logger is up and before the Application is constructed.
 */
CE_UTILITY_API void InitFromCommandLine(int argc, const char* argv[]);

}

#endif //CE_UTILITY_CMD_COMMANDLINE_HPP
