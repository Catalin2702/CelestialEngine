//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain
// File: SwapchainFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#include "Core/Render/Device/I_GraphicDevice.hpp"
#include "Core/Render/Swapchain/I_Swapchain.hpp"
#include "Core/Render/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.hpp"
#include "Core/Render/Surface/Common/OpenGl/I_OpenGlSurface.hpp"
#include "Core/Window/I_Window.hpp"
#include "Tools/Tools.hpp"
#include "Types/Types.hpp"

#if CE_PLATFORM_MACOS
#include "Core/Render/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#include "Core/Render/Surface/Mac/Metal/I_MetalSurface.hpp"
#include "Core/Render/Swapchain/Platforms/Mac/Metal/MetalSwapchain.hpp"
#endif

#include <format>
#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_Swapchain> I_Swapchain::MakeSwapchain(I_Window& window, I_GraphicDevice& graphicDevice, const Types::GraphicsApi api) {
	// Checked here rather than asserted: it is the one mistake that produces no error at all further down - the
	// swapchain would present buffers the device never rendered into - and it costs one comparison, once per window.
	if (graphicDevice.GetGraphicApi() != api) [[unlikely]] {
		const auto error = std::format("I_Swapchain::MakeSwapchain: The graphic device speaks {} but a {} swapchain was asked for!", graphicDevice.GetGraphicApi(), api);
	}
	switch (api) {
		case Types::GraphicsApi::OpenGL: {
			auto* const surface = dynamic_cast<I_OpenGlSurface*>(&window);
			if (not surface) [[unlikely]] {
				constexpr auto error = "I_Swapchain::MakeSwapchain: The window does not provide an OpenGL surface!";
				CE_CORE_ERROR(error);
				throw std::runtime_error(error);
			}
			return std::make_unique<OpenGlSwapchain>(*surface);
		}
#if CE_PLATFORM_MACOS
		case Types::GraphicsApi::Metal: {
			auto* const surface = dynamic_cast<I_MetalSurface*>(&window);
			if (not surface) [[unlikely]] {
				constexpr auto error = "I_Swapchain::MakeSwapchain: The window does not provide a Metal surface!";
				CE_CORE_ERROR(error);
				throw std::runtime_error(error);
			}
			return std::make_unique<MetalSwapchain>(*surface, static_cast<MetalGraphicDevice&>(graphicDevice));
		}
#endif
		default: {
			constexpr auto error = "I_Swapchain::MakeSwapchain: Not yet implemented!";
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}
