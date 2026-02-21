//
// Created by Catalin Chirosca on 2026-02-21.
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

	for (int i = 0; i < argc; ++i) {
		std::string arg = Manipulation::ToLowerCase(argv[i]);

		if ((arg == "--title" or arg == "-t") and i + 1 < argc) {
			title = argv[++i];
		}
		else if ((arg == "--width" or arg == "-w") and i + 1 < argc) {
			width = std::stoi(argv[++i]);
		}
		else if ((arg == "--height" or arg == "-h") and i + 1 < argc) {
			height = std::stoi(argv[++i]);
		}
		else if ((arg == "--vsync" or arg == "-v") and i + 1 < argc) {
			VSync = argv[++i];
		}
		else
			CE_CORE_WARN("The parameter: ({0}) is not supported", argv[i]);
	}
	return {title, width, height, VSync};
}

}
