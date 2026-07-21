//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: WindowProps.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#include "Types/Window/WindowProps.hpp"

namespace CE::Types {

WindowProps::WindowProps(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const unsigned int refreshRate, const GraphicsApi graphicsApi, const WindowApi windowApi): // NOLINT(*-pass-by-value)
	title(title), width(width), height(height), VSync(VSync), refreshRate(refreshRate), graphicsApi(graphicsApi), windowApi(windowApi) {}

bool IsWindowApiSupported(const WindowApi& api) {
	if (api == WindowApi::None)
		return false;

	if (api == WindowApi::GLFW)
		return true;

#ifdef CE_PLATFORM_MACOS
	if (api == WindowApi::Cocoa)
		return true;
#elifdef CE_PLATFORM_WINDOWS
	if (api == WindowApi::Win32)
		return true;
#elifdef CE_PLATFORM_LINUX
	if (api == WindowApi::X11)
		return true;
#else
		#error Unsupported platform for window API support check
#endif
	return false;
}

bool IsGraphicsApiCompatibleWithWindowApi(const GraphicsApi& graphicsApi, const WindowApi& windowApi) {
	if (windowApi == WindowApi::None or graphicsApi == GraphicsApi::None)
		return false;

	if (windowApi == WindowApi::GLFW and graphicsApi == GraphicsApi::OpenGL)
		return true;

#ifdef CE_PLATFORM_MACOS
	if (windowApi == WindowApi::Cocoa and (graphicsApi == GraphicsApi::Metal or graphicsApi == GraphicsApi::Vulkan))
		return true;
#elifdef CE_PLATFORM_WINDOWS
	if (windowApi == WindowApi::Win32 and (graphicsApi == GraphicsApi::DirectX11 or graphicsApi == GraphicsApi::DirectX12 or graphicsApi == GraphicsApi::Vulkan))
		return true;
#elifdef CE_PLATFORM_LINUX
	if (windowApi == WindowApi::X11 and graphicsApi == GraphicsApi::Vulkan)
		return true;
#endif

	return false;
}

}
