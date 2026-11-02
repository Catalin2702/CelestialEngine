//
// Module: CelestialEngine/Engine/Modules/Utility/CMD
// File: Window.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Utility/CMD/Window.hpp"

#include "Tools/Tools.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Utility/Manipulation/String.hpp"

#include <array>
#include <cctype>
#include <format>
#include <stdexcept>
#include <string>

namespace CE::Utility {

namespace {

/// Parses the numeric value of an option. `label` names it in the thrown message ("Invalid width parameter: ...").
unsigned int ParseUnsigned(const std::string& arg, const char* value, const std::string& label) {
	try {
		return static_cast<unsigned int>(std::stoi(value));
	}
	catch (const std::invalid_argument& invalidArgument) {
		CE_CORE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, value, invalidArgument.what());
		throw std::runtime_error("Invalid " + label + " parameter: " + std::string(value));
	}
	catch (const std::out_of_range& ofRange) {
		CE_CORE_ERROR("Error retrieving {0} parameter with value '{1}'.\nError: {2}", arg, value, ofRange.what());
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

	const auto fallback = Types::GetDefaultGraphicsApi();
	CE_CORE_WARN("Unsupported graphics API specified: ({0}). Defaulting to {1}.", value, fallback);
	return fallback;
}

/// Parses -wa. `graphicsApi` is what an unknown value falls back to the pairing of.
Types::WindowApi ParseWindowApi(const char* value, const Types::GraphicsApi graphicsApi) {
	const std::string apiArg = ToLowerCase(value);
	if (apiArg == "glfw")
		return Types::WindowApi::GLFW;
	if (apiArg == "win32")
		return Types::WindowApi::Win32;
	if (apiArg == "x11")
		return Types::WindowApi::X11;
	if (apiArg == "cocoa")
		return Types::WindowApi::Cocoa;

	const auto fallback = Types::GetDefaultWindowApi(graphicsApi);
	CE_CORE_WARN("Unsupported window API specified: ({0}). Defaulting to {1}.", value, fallback);
	return fallback;
}

/// Every graphics API this build contains, comma-separated, for error messages.
std::string CompiledGraphicsApis() {
	constexpr std::array all{Types::GraphicsApi::OpenGL, Types::GraphicsApi::Metal, Types::GraphicsApi::Vulkan,
							 Types::GraphicsApi::DirectX11, Types::GraphicsApi::DirectX12};
	std::string names;
	for (const auto api: all) {
		if (not Types::IsGraphicsApiCompiled(api))
			continue;
		if (not names.empty())
			names += ", ";
		names += format_as(api);
	}
	return names;
}

/// Every window API this build contains, comma-separated, for error messages.
std::string CompiledWindowApis() {
	constexpr std::array all{Types::WindowApi::GLFW, Types::WindowApi::Cocoa, Types::WindowApi::Win32,
							 Types::WindowApi::X11, Types::WindowApi::Wayland};
	std::string names;
	for (const auto api: all) {
		if (not Types::IsWindowApiCompiled(api))
			continue;
		if (not names.empty())
			names += ", ";
		names += format_as(api);
	}
	return names;
}

}

Types::WindowProps GetWindowProps(const int argc, const char* argv[]){
	std::string title = "CelestialEngine";
	unsigned int width = 1280;
	unsigned int height = 720;
	auto VSync = true;
	unsigned int refreshRate = 0;
	auto graphicsApi = Types::GetDefaultGraphicsApi();

	// Resolved after the loop: the default window depends on the graphics API, and -g may come after -wa.
	const char* windowApiArg = nullptr;

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
			windowApiArg = argv[++i];
		}
		else {
			CE_CORE_WARN("The parameter: ({0}) is not supported", argv[i]);
		}
	}
	const auto windowApi = windowApiArg ? ParseWindowApi(windowApiArg, graphicsApi) : Types::GetDefaultWindowApi(graphicsApi);

	return {title, width, height, VSync, refreshRate, graphicsApi, windowApi};
}

void ValidateWindowProps(const Types::WindowProps& windowProps) {
	// Thrown without logging: the one caller is main(), which reports what it catches - at critical level, so the
	// message survives even in Dist, where an error-level log here would be compiled out.
	const auto graphicsApi = windowProps.graphicsApi;
	const auto windowApi = windowProps.windowApi;

	if (not Types::IsGraphicsApiCompiled(graphicsApi)) [[unlikely]] {
		const auto error = std::format(
			"Graphics API {} is not compiled into this build (CE_RENDER_BACKENDS). Available: {}.",
			graphicsApi, CompiledGraphicsApis());
		throw std::runtime_error(error);
	}

	if (not Types::IsWindowApiCompiled(windowApi)) [[unlikely]] {
		const auto error = std::format(
			"Window API {} is not compiled into this build (it follows CE_RENDER_BACKENDS). Available: {}.",
			windowApi, CompiledWindowApis());
		throw std::runtime_error(error);
	}

	if (not Types::IsGraphicsApiCompatible(graphicsApi, windowApi)) [[unlikely]] {
		const auto error = std::format(
			"Graphics API {} cannot render into a {} window; {} pairs with {}.",
			graphicsApi, windowApi, graphicsApi, Types::GetDefaultWindowApi(graphicsApi));
		throw std::runtime_error(error);
	}
}

}
