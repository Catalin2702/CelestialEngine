//
// Module: CelestialEngine/Engine/Modules/Utility/CMD
// File: Window.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-09-18
//

#include "Utility/CMD/Window.hpp"

#include "Tools/Tools.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Utility/Manipulation/String.hpp"

#include <cctype>
#include <stdexcept>

namespace CE::Utility {

namespace {

/// Parses the numeric value of an option. `label` names it in the thrown message ("Invalid width parameter: ...").
unsigned int ParseUnsigned(const std::string& arg, const char* value, const std::string& label) {
	try {
		return static_cast<unsigned int>(std::stoi(value));
	}
	catch (const std::invalid_argument& invalidArgument) {
		CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, value, invalidArgument.what());
		throw std::runtime_error("Invalid " + label + " parameter: " + std::string(value));
	}
	catch (const std::out_of_range& ofRange) {
		CE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, value, ofRange.what());
		// Capitalised because it starts the message: "Width parameter out of range: ...".
		std::string capitalised = label;
		capitalised.front() = static_cast<char>(std::toupper(static_cast<unsigned char>(capitalised.front())));
		throw std::runtime_error(capitalised + " parameter out of range: " + std::string(value));
	}
}

Types::GraphicsApi ParseGraphicsApi(const char* value) {
	const std::string apiArg = ToLowerCase(value);
	if (apiArg == "opengl")
		return Types::GraphicsApi::OpenGL;
	if (apiArg == "metal")
		return Types::GraphicsApi::Metal;
	if (apiArg == "vulkan")
		return Types::GraphicsApi::Vulkan;
	if (apiArg == "directx11" or apiArg == "dx11" or apiArg == "d3d11")
		return Types::GraphicsApi::DirectX11;
	if (apiArg == "directx12" or apiArg == "dx12" or apiArg == "d3d12")
		return Types::GraphicsApi::DirectX12;

	CE_CORE_WARN("Unsupported graphics API specified: ({0}). Defaulting to OpenGL.", value);
	return Types::GraphicsApi::OpenGL;
}

Types::WindowApi ParseWindowApi(const char* value) {
	const std::string apiArg = ToLowerCase(value);
	if (apiArg == "glfw")
		return Types::WindowApi::GLFW;
	if (apiArg == "win32")
		return Types::WindowApi::Win32;
	if (apiArg == "x11")
		return Types::WindowApi::X11;
	if (apiArg == "cocoa")
		return Types::WindowApi::Cocoa;

	CE_CORE_WARN("Unsupported window API specified: ({0}). Defaulting to GLFW.", value);
	return Types::WindowApi::GLFW;
}

}

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
		const std::string arg = ToLowerCase(argv[i]);
		// Every supported option takes a value, so an option in last position is as unsupported as an unknown one.
		const bool hasValue = i + 1 < argc;

		if ((arg == "--title" or arg == "-t") and hasValue) {
			title = argv[++i];
		}
		else if ((arg == "--width" or arg == "-w") and hasValue) {
			width = ParseUnsigned(arg, argv[++i], "width");
		}
		else if ((arg == "--height" or arg == "-h") and hasValue) {
			height = ParseUnsigned(arg, argv[++i], "height");
		}
		else if ((arg == "--vsync" or arg == "-v") and hasValue) {
			const std::string vsyncArg = ToLowerCase(argv[++i]);
			VSync = (vsyncArg == "true" or vsyncArg == "1");
		}
		else if ((arg == "--refresh" or arg == "-r") and hasValue) {
			refreshRate = ParseUnsigned(arg, argv[++i], "refresh");
		}
		else if ((arg == "--graphics-api" or arg == "-g") and hasValue) {
			graphicsApi = ParseGraphicsApi(argv[++i]);
		}
		else if ((arg == "--window-api" or arg == "-wa") and hasValue) {
			windowApi = ParseWindowApi(argv[++i]);
		}
		else {
			CE_CORE_WARN("The parameter: ({0}) is not supported", argv[i]);
		}
	}
	return {title, width, height, VSync, refreshRate, graphicsApi, windowApi};
}

}
