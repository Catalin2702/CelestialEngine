//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: WindowProps.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-21
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include "Types/Window/WindowProps.hpp"


namespace CE::Types {

bool HasAnyFlags(const WindowApi x, const WindowApi y) {
	return (x & y) != WindowApi::None;
}

WindowProps::WindowProps(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const unsigned int refreshRate, const GraphicsApi graphicsApi, const WindowApi windowApi): // NOLINT(*-pass-by-value)
	title(title), width(width), height(height), VSync(VSync), refreshRate(refreshRate), graphicsApi(graphicsApi), windowApi(windowApi) {}

bool IsWindowApiSupported(const WindowApi& api) {
	if (api == WindowApi::None) [[unlikely]]
		return false;

	if (api == WindowApi::GLFW)
		return true;

	if constexpr (CE_PLATFORM_MACOS)
		if (api == WindowApi::Cocoa)
			return true;
	if constexpr (CE_PLATFORM_WINDOWS)
		if (api == WindowApi::Win32)
			return true;
	if constexpr (CE_PLATFORM_LINUX)
		if (HasAnyFlags(api, WindowApi::X11 | WindowApi::Wayland))
			return true;

	return false;
}

bool IsGraphicsApiCompatibleWithWindowApi(const GraphicsApi& graphicsApi, const WindowApi& windowApi) {
	if (windowApi == WindowApi::None or graphicsApi == GraphicsApi::None) [[unlikely]]
		return false;

	if (windowApi == WindowApi::GLFW and graphicsApi == GraphicsApi::OpenGL)
		return true;

	if constexpr (CE_PLATFORM_MACOS)
		if (windowApi == WindowApi::Cocoa and HasAnyFlags(graphicsApi, GraphicsApi::Metal | GraphicsApi::Vulkan))
			return true;
	if constexpr (CE_PLATFORM_WINDOWS)
		if (windowApi == WindowApi::Win32 and HasAnyFlags(graphicsApi, GraphicsApi::DirectX11 | GraphicsApi::DirectX12 | GraphicsApi::Vulkan))
			return true;
	if constexpr (CE_PLATFORM_LINUX)
		if (HasAnyFlags(windowApi, WindowApi::X11 | WindowApi::Wayland) and graphicsApi == GraphicsApi::Vulkan)
			return true;

	return false;
}

}
