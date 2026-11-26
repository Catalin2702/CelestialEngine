//
// Module: CelestialEngine/Engine/Source/Runtime/Core/Render/Renderer
// File: RendererFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-26
//

#include "Core/Render/Renderer/Renderer.hpp"
#include "Core/Window/I_Window.hpp"
#include "RHI/Device/I_GraphicDevice.hpp"
#include "RHI/Surface/I_Surface.hpp"
#include "RHI/Swapchain/I_Swapchain.hpp"
#include "Tools/Tools.hpp"

#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_Renderer> I_Renderer::MakeRenderer(I_Window& window, const Types::GraphicsApi api) {
	// Unlike the RHI factories below it, this one does not switch on the API: what varies here is the technique, and
	// every technique runs on every backend. The API only decides what the device and the swapchain are, and those
	// two already know how to answer that themselves - including by throwing for a backend they do not implement yet.
	auto graphicDevice = RHI::I_GraphicDevice::MakeDevice(api);

	// The RHI presents into an RHI::I_Surface and knows nothing of windows; a window that can be presented into
	// implements the surface of its backend, which derives from it. Asked here, on the Core side, because this is the
	// only place that knows both: a window without a surface is a window the renderer cannot draw into.
	auto* const surface = dynamic_cast<RHI::I_Surface*>(&window);
	if (not surface) [[unlikely]] {
		constexpr auto error = "I_Renderer::MakeRenderer: Window does not offer a surface to present into!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	// After the device on purpose: a swapchain is created against one, and on OpenGL the device's creation is what
	// leaves a context current for it to read its drawable from.
	auto swapchain = RHI::I_Swapchain::MakeSwapchain(*surface, *graphicDevice, api);

	return std::make_unique<ForwardRenderer>(std::move(graphicDevice), std::move(swapchain));
}

}
