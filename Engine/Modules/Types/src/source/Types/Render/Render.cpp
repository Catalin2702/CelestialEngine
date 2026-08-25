//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: Render.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include "Types/Render/Render.hpp"

namespace CE::Types {

bool HasAnyFlags(const GraphicsApi x, const GraphicsApi y) {
	return (x & y) != GraphicsApi::None;
}

bool IsGraphicsApiSupported(const GraphicsApi& api) {
	if (api == GraphicsApi::None)
		return false;

	if (api == GraphicsApi::OpenGL)
		return true;

	if constexpr (CE_PLATFORM_MACOS)
		if (HasAnyFlags(api, GraphicsApi::Metal | GraphicsApi::Vulkan))
			return true;
	if constexpr (CE_PLATFORM_WINDOWS)
		if (HasAnyFlags(api, GraphicsApi::DirectX11 | GraphicsApi::DirectX12 | GraphicsApi::Vulkan))
			return true;
	if constexpr (CE_PLATFORM_LINUX)
		if (api == GraphicsApi::Vulkan)
			return true;
	return false;
}

}
