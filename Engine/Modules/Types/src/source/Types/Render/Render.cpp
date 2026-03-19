//
// Module: CelestialEngine/Engine/Modules/Types/Window
// File: Render.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
//

#include "Types/Render/Render.hpp"


namespace CE::Types::Render {

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

}
