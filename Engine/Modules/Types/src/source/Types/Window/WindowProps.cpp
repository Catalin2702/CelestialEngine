//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: WindowProps.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-03-16
//

#include "Types/Window/WindowProps.hpp"


namespace CE::Types::Window {

WindowProps::WindowProps(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const GraphicsApi graphicsApi, const WindowApi windowApi): // NOLINT(*-pass-by-value)
	title(title), width(width), height(height), VSync(VSync), graphicsApi(graphicsApi), windowApi(windowApi) {}

bool IsGraphicsApiSupported(const GraphicsApi& api) {
	if (api == GraphicsApi::None)
		return false;

	if (api == GraphicsApi::OpenGL or api == GraphicsApi::Vulkan)
		return true;

#ifdef CE_PLATFORM_MACOS
	if (api == GraphicsApi::Metal)
		return true;
#elifdef CE_PLATFORM_WINDOWS
	if (api == GraphicsApi::DirectX11 or api == GraphicsApi::DirectX12)
		return true;
#else
		#error Unsupported platform for graphics API support check
#endif
	return false;
}

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
#elifdefCE_PLATFORM_LINUX
	if (api == WindowApi::X11)
		return true;
#else
		#error Unsupported platform for window API support check
#endif
	return false;
}

bool IsGraphicsApiCompatibleWithWindowApi(const GraphicsApi& graphicsApi, const WindowApi& windowApi) {
	if (graphicsApi == GraphicsApi::None or windowApi == WindowApi::None)
		return false;

	if (graphicsApi == GraphicsApi::OpenGL and windowApi == WindowApi::GLFW)
		return true;

	if (graphicsApi == GraphicsApi::Vulkan and windowApi == WindowApi::GLFW)
		return true;

#ifdef CE_PLATFORM_MACOS
	if (graphicsApi == GraphicsApi::Metal and windowApi == WindowApi::Cocoa)
		return true;

	if (graphicsApi == GraphicsApi::Metal and windowApi == WindowApi::GLFW)
		return true;
#elifdef CE_PLATFORM_WINDOWS
	if ((graphicsApi == GraphicsApi::DirectX11 or graphicsApi == GraphicsApi::DirectX12) and windowApi == WindowApi::Win32)
		return true;
#elifdef CE_PLATFORM_LINUX
	if (graphicsApi == GraphicsApi::Vulkan and windowApi == WindowApi::X11)
		return true;
#else
		#error Unsupported platform for graphics API and window API compatibility check
#endif
	return false;
}

}
