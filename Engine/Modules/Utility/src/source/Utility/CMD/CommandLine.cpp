//
// Module: CelestialEngine/Engine/Modules/Utility/CMD
// File: CommandLine.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-03
//

#include "Utility/CMD/CommandLine.hpp"

#include "Utility/CMD/Window.hpp"
#include "Utility/Config/Config.hpp"
#include "Utility/FileSystem/FileSystem.hpp"

#include <utility>


namespace CE::Utility {

void InitFromCommandLine(const int argc, const char* argv[]) {
	// Validated before it is stored: Config never holds a window this build cannot open.
	auto windowProps = ParseWindowArgs(argc, argv);
	ValidateWindowProps(windowProps);
	Config::SetWindowProps(std::move(windowProps));

	// argv[0] may be missing (argc == 0 is allowed by the standard): the root then stays where it was.
	if (argc > 0 and argv[0])
		FileSystem::SetRootDirectory(fs::path(argv[0]).parent_path());
}

}
