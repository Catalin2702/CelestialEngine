//
// Module: CelestialEngine/Engine/Modules/Core/CMD
// File: CommandLine.cpp
// Created by: Catalin Chirosca
// Created: 2026-11-03
// Updated by: Catalin Chirosca
// Updated: 2026-11-25
//

#include "Core/CMD/CommandLine.hpp"

#include "Core/CMD/Window.hpp"
#include "Core/Config/Config.hpp"

#include <utility>


namespace CE::Core {

void InitFromCommandLine(const int argc, const char* argv[]) {
	// Validated before it is stored: Config never holds a window this build cannot open.
	auto windowProps = ParseWindowArgs(argc, argv);
	ValidateWindowProps(windowProps);
	Config::SetWindowProps(std::move(windowProps));
}

}
