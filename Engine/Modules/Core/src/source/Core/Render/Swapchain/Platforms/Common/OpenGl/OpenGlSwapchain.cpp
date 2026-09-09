//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain/Platforms/Common/OpenGl
// File: OpenGlSwapchain.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-31
// Updated by: Catalin Chirosca
// Updated: 2026-09-09
//

#include "Core/Render/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.hpp"

#include <tuple>


namespace CE::Core {

OpenGlSwapchain::OpenGlSwapchain(I_OpenGlSurface& surface): _surface(&surface) {
	std::ignore = OpenGlSwapchain::PrepareFrame();
}

bool OpenGlSwapchain::PrepareFrame() {
	const auto [width, height] = _surface->GetDrawableSize();

	_width = width;
	_height = height;

	return _width != 0 and _height != 0;
}

bool OpenGlSwapchain::AcquireNextTarget() {
	// Nothing to take. The chain here is the window's default framebuffer, which is always there and is never handed
	// out one buffer at a time - so the whole reason the interface splits this call off the geometry one, that a held
	// buffer is a buffer the display system cannot recycle, simply does not arise. The size check is all that is left
	// of the contract, and it is the same one PrepareFrame already answered.
	return _width != 0 and _height != 0;
}

void OpenGlSwapchain::Present() {
	_surface->SwapBuffers();
}

void OpenGlSwapchain::Resize(const u32 width, const u32 height) {
	_width = width;
	_height = height;
}

void OpenGlSwapchain::SetVSync(const bool enabled) {
	_surface->SetSwapInterval(enabled ? 1 : 0);
	_vsync = enabled;
}

}
