//
// Module: CelestialEngine/Engine/Modules/Utility/CMD
// File: Window.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#include "Utility/CMD/Window.hpp"

#include "Tools/Tools.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Utility/Manipulation/String.hpp"

#include <stdexcept>

namespace CE::Utility {

Types::WindowProps GetWindowProps(const int argc, const char* argv[]){
	std::string title = "CelestialEngine";
	unsigned int width = 1280;
	unsigned int height = 720;
	auto VSync = true;
	unsigned int refreshRate = 0;
	auto graphicsApi = Types::GraphicsApi::OpenGL;
	auto windowApi = Types::WindowApi::GLFW;

	// Start from 1 to skip executable path
	for (int i = 1; i < argc; ++i) {
		// ReSharper disable once CppTooWideScopeInitStatement
		const std::string arg = ToLowerCase(argv[i]);

		if ((arg == "--title" or arg == "-t") and i + 1 < argc) {
			title = argv[++i];
		}
		else if ((arg == "--width" or arg == "-w") and i + 1 < argc) {
			try {
				width = static_cast<unsigned int>(std::stoi(argv[++i]));
			}
			catch (const std::invalid_argument& invalidArgument) {
				CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, argv[i], invalidArgument.what());
				throw std::runtime_error("Invalid width parameter: " + std::string(argv[i]));
			}
			catch (const std::out_of_range& ofRange) {
				CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, argv[i], ofRange.what());
				throw std::runtime_error("Width parameter out of range: " + std::string(argv[i]));
			}
		}
		else if ((arg == "--height" or arg == "-h") and i + 1 < argc) {
			try {
				height = static_cast<unsigned int>(std::stoi(argv[++i]));
			}
			catch (const std::invalid_argument& invalidArgument) {
				CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, argv[i], invalidArgument.what());
				throw std::runtime_error("Invalid height parameter: " + std::string(argv[i]));
			}
			catch (const std::out_of_range& ofRange) {
				CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, argv[i], ofRange.what());
				throw std::runtime_error("Height parameter out of range: " + std::string(argv[i]));
			}
		}
		else if ((arg == "--vsync" or arg == "-v") and i + 1 < argc) {
			const std::string vsyncArg = ToLowerCase(argv[++i]);
			VSync = (vsyncArg == "true" || vsyncArg == "1");
		}
		else if ((arg == "--refresh" or arg == "-r") and i + 1 < argc) {
			try {
				refreshRate = static_cast<unsigned int>(std::stoi(argv[++i]));
			}
			catch (const std::invalid_argument& invalidArgument) {
				CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, argv[i], invalidArgument.what());
				throw std::runtime_error("Invalid refresh parameter: " + std::string(argv[i]));
			}
			catch (const std::out_of_range& ofRange) {
				CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, argv[i], ofRange.what());
				throw std::runtime_error("Refresh parameter out of range: " + std::string(argv[i]));
			}
		}
		else if ((arg == "--graphics-api" or arg == "-g") and i + 1 < argc) {
			// ReSharper disable once CppTooWideScopeInitStatement
			const std::string apiArg = ToLowerCase(argv[++i]);
			if (apiArg == "opengl") {
				graphicsApi = Types::GraphicsApi::OpenGL;
			}
			else if (apiArg == "metal") {
				graphicsApi = Types::GraphicsApi::Metal;
			}
			else if (apiArg == "vulkan") {
				graphicsApi = Types::GraphicsApi::Vulkan;
			}
			else if (apiArg == "directx11" or apiArg == "dx11" or apiArg == "d3d11") {
				graphicsApi = Types::GraphicsApi::DirectX11;
			}
			else if (apiArg == "directx12" or apiArg == "dx12" or apiArg == "d3d12") {
				graphicsApi = Types::GraphicsApi::DirectX12;
			}
			else {
				CE_CORE_WARN("Unsupported graphics API specified: ({0}). Defaulting to OpenGL.", argv[i]);
				graphicsApi = Types::GraphicsApi::OpenGL;
			}
		}
		else if ((arg == "--window-api" or arg == "-wa") and i + 1 < argc) {
			// ReSharper disable once CppTooWideScopeInitStatement
			const std::string apiArg = ToLowerCase(argv[++i]);
			if (apiArg == "glfw") {
				windowApi = Types::WindowApi::GLFW;
			}
			else if (apiArg == "win32") {
				windowApi = Types::WindowApi::Win32;
			}
			else if (apiArg == "x11") {
				windowApi = Types::WindowApi::X11;
			}
			else if (apiArg == "cocoa") {
				windowApi = Types::WindowApi::Cocoa;
			}
			else {
				CE_CORE_WARN("Unsupported window API specified: ({0}). Defaulting to GLFW.", argv[i]);
				windowApi = Types::WindowApi::GLFW;
			}
		}
		else {
			CE_CORE_WARN("The parameter: ({0}) is not supported", argv[i]);
		}
	}
	return {title, width, height, VSync, refreshRate, graphicsApi, windowApi};
}

}
