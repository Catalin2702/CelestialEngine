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

/**
 * @brief Parses command-line arguments to create window properties
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Types::Window::WindowProps Window configuration extracted from arguments
 * @details Parses command-line arguments for window configuration:
 *          - --title/-t "title" : Sets window title (default: "CelestialEngine")
 *          - --width/-w 1280 : Sets window width (default: 1280)
 *          - --height/-h 720 : Sets window height (default: 720)
 *          - --vsync/-v true/false : Enables/disables VSync (default: true)
 *          Uses case-insensitive matching for argument names. Exits with error
 *          if invalid values are provided for numeric parameters.
 */
Types::Window::WindowProps GetWindowProps(const int argc, char* argv[]){
	std::string title = "CelestialEngine";
	unsigned int width = 1280;
	unsigned int height = 720;
	auto VSync = true;
	auto graphicsApi = Types::Window::GraphicsApi::OpenGL;

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
		else if ((arg == "--graphics-api" or arg == "-g") and i + 1 < argc) {
			// ReSharper disable once CppTooWideScopeInitStatement
			const std::string apiArg = Manipulation::ToLowerCase(argv[++i]);
			if (apiArg == "opengl") {
				graphicsApi = Types::Window::GraphicsApi::OpenGL;
			}
			else if (apiArg == "metal") {
				graphicsApi = Types::Window::GraphicsApi::Metal;
			}
			else if (apiArg == "vulkan") {
				graphicsApi = Types::Window::GraphicsApi::Vulkan;
			}
			else if (apiArg == "directx11" or apiArg == "dx11" or apiArg == "d3d11") {
				graphicsApi = Types::Window::GraphicsApi::DirectX11;
			}
			else if (apiArg == "directx12" or apiArg == "dx12" or apiArg == "d3d12") {
				graphicsApi = Types::Window::GraphicsApi::DirectX12;
			}
			else {
				CE_CORE_WARN("Unsupported graphics API specified: ({0}). Defaulting to OpenGL.", argv[i]);
				graphicsApi = Types::Window::GraphicsApi::OpenGL;
			}
		}
		else {
			CE_CORE_WARN("The parameter: ({0}) is not supported", argv[i]);
		}
	}
	return {title, width, height, VSync, graphicsApi};
}

}
