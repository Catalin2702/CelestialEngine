//
// Module: CelestialEngine/Engine/Modules/Types/Render
// File: GraphicsApi.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-06
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Types/Render/GraphicsApi.hpp"


namespace CE::Types {

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

bool IsGraphicsApiCompiled(const GraphicsApi api) {
	// A chain of comparisons rather than a switch: the CE_RENDER_* values are 0 or 1, so the branches of a switch would
	// read as identical to the compiler. A combination of flags, or None, matches none of them.
	return (CE_RENDER_OPENGL and api == GraphicsApi::OpenGL)
		or (CE_RENDER_METAL and api == GraphicsApi::Metal)
		or (CE_RENDER_VULKAN and api == GraphicsApi::Vulkan)
		or (CE_RENDER_DIRECTX11 and api == GraphicsApi::DirectX11)
		or (CE_RENDER_DIRECTX12 and api == GraphicsApi::DirectX12);
}

GraphicsApi GetDefaultGraphicsApi() {
	constexpr auto api = GraphicsApi::CE_RENDER_DEFAULT_API;
	static_assert(api != GraphicsApi::None, "CE_RENDER_DEFAULT_API must name a graphics backend.");

	return api;
}

}
