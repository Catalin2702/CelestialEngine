//
// Module: Tools/CMD
// File: Window.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-21
// Updated: 2026-02-28
//

#include "Tools/CMD/Window.hpp"
#include "Tools/Log/Log.hpp"
#include "Tools/Manipulation/String.hpp"
#include "Types/Window/WindowProps.hpp"


namespace CE::Tools::CMD {

Types::Window::WindowProps GetWindowProps(const int argc, char* argv[]){
	std::string title = "CelestialEngine";
	unsigned int width = 1280;
	unsigned int height = 720;
	bool VSync = true;

	// Start from 1 to skip executable path
	for (int i = 1; i < argc; ++i) {
		// ReSharper disable once CppTooWideScopeInitStatement
		const std::string arg = Manipulation::ToLowerCase(argv[i]);

		if ((arg == "--title" or arg == "-t") and i + 1 < argc) {
			title = argv[++i];
		}
		else if ((arg == "--width" or arg == "-w") and i + 1 < argc) {
			try {
				width = static_cast<unsigned int>(std::stoi(argv[++i]));
			}
			catch ([[maybe_unused]] const std::invalid_argument& invalidArgument) {
				CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, argv[i], invalidArgument.what());
				exit(EXIT_FAILURE);
			}
			catch ([[maybe_unused]] const std::out_of_range& ofRange) {
				CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, argv[i], ofRange.what());
				exit(EXIT_FAILURE);
			}
		}
		else if ((arg == "--height" or arg == "-h") and i + 1 < argc) {
			try {
				height = static_cast<unsigned int>(std::stoi(argv[++i]));
			}
			catch ([[maybe_unused]] const std::invalid_argument& invalidArgument) {
				CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, argv[i], invalidArgument.what());
				exit(EXIT_FAILURE);
			}
			catch ([[maybe_unused]] const std::out_of_range& ofRange) {
				CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, argv[i], ofRange.what());
				exit(EXIT_FAILURE);
			}
		}
		else if ((arg == "--vsync" or arg == "-v") and i + 1 < argc) {
			const std::string vsyncArg = Manipulation::ToLowerCase(argv[++i]);
			VSync = (vsyncArg == "true" || vsyncArg == "1");
		}
		else {
			CE_CORE_WARN("The parameter: ({0}) is not supported", argv[i]);
		}
	}
	return {title, width, height, VSync};
}

}
