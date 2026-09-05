//
// Module: CelestialEngine/Engine/Modules/Core/Render/Renderer
// File: RendererFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Core/Render/Device/I_GraphicDevice.hpp"
#include "Core/Render/Renderer/I_Renderer.hpp"
#include "Core/Render/Renderer/ForwardRenderer.hpp"
#include "Core/Render/Swapchain/I_Swapchain.hpp"


namespace CE::Core {

std::unique_ptr<I_Renderer> I_Renderer::MakeRenderer(I_Window& window, const Types::GraphicsApi api) {
	// Unlike the RHI factories below it, this one does not switch on the API: what varies here is the technique, and
	// every technique runs on every backend. The API only decides what the device and the swapchain are, and those
	// two already know how to answer that themselves - including by throwing for a backend they do not implement yet.
	auto graphicDevice = I_GraphicDevice::MakeDevice(api);

	// After the device on purpose: a swapchain is created against one, and on OpenGL the device's creation is what
	// leaves a context current for it to read its drawable from.
	auto swapchain = I_Swapchain::MakeSwapchain(window, *graphicDevice, api);

	return std::make_unique<ForwardRenderer>(std::move(graphicDevice), std::move(swapchain));
}

}
