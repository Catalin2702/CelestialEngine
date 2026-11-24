//
// Module: CelestialEngine/Engine/Modules/Utility/CMD
// File: CommandLine.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#include "Utility/CMD/CommandLine.hpp"

#include "Utility/CMD/Window.hpp"
#include "Utility/Config/Config.hpp"

#include <utility>


namespace CE::Utility {

void InitFromCommandLine(const int argc, const char* argv[]) {
	// Validated before it is stored: Config never holds a window this build cannot open.
	auto windowProps = ParseWindowArgs(argc, argv);
	ValidateWindowProps(windowProps);
	Config::SetWindowProps(std::move(windowProps));
}

}
