//
// Module: CelestialEngine/Engine/Modules/RHI/Swapchain
// File: SwapchainFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-24
//

#include "RHI/Device/I_GraphicDevice.hpp"
#include "RHI/Surface/I_Surface.hpp"
#include "RHI/Swapchain/Swapchain.hpp"
#include "Tools/Tools.hpp"
#include "Types/Types.hpp"

#if CE_RENDER_OPENGL
#include "RHI/Surface/Common/OpenGl/I_OpenGlSurface.hpp"
#endif

#if CE_RENDER_METAL
#include "RHI/Device/Platforms/Mac/Metal/MetalGraphicDevice.hpp"
#include "RHI/Surface/Mac/Metal/I_MetalSurface.hpp"
#include "RHI/Swapchain/Platforms/Mac/Metal/MetalSwapchain.hpp"
#endif

#include <format>
#include <stdexcept>


namespace CE::RHI {

std::unique_ptr<I_Swapchain> I_Swapchain::MakeSwapchain(I_Surface& surface, I_GraphicDevice& graphicDevice, const Types::GraphicsApi api) {
	// Checked here rather than asserted: it is the one mistake that produces no error at all further down - the
	// swapchain would present buffers the device never rendered into - and it costs one comparison, once per window.
	if (graphicDevice.GetGraphicApi() != api) [[unlikely]] {
		const auto error = std::format("I_Swapchain::MakeSwapchain: The graphic device speaks {} but a {} swapchain was asked for!", graphicDevice.GetGraphicApi(), api);
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
	switch (api) {
#if CE_RENDER_OPENGL
		case Types::GraphicsApi::OpenGL: {
			auto* const openGlSurface = dynamic_cast<I_OpenGlSurface*>(&surface);
			if (not openGlSurface) [[unlikely]] {
				constexpr auto error = "I_Swapchain::MakeSwapchain: The surface does not provide an OpenGL surface!";
				CE_CORE_ERROR(error);
				throw std::runtime_error(error);
			}
			return std::make_unique<OpenGlSwapchain>(*openGlSurface);
		}
#endif
#if CE_RENDER_METAL
		case Types::GraphicsApi::Metal: {
			auto* const metalSurface = dynamic_cast<I_MetalSurface*>(&surface);
			if (not metalSurface) [[unlikely]] {
				constexpr auto error = "I_Swapchain::MakeSwapchain: The surface does not provide a Metal surface!";
				CE_CORE_ERROR(error);
				throw std::runtime_error(error);
			}
			return std::make_unique<MetalSwapchain>(*metalSurface, static_cast<MetalGraphicDevice&>(graphicDevice));
		}
#endif
		default: {
			// Either a backend that does not exist yet, or one this build was configured without (CE_RENDER_BACKENDS).
			const auto error = std::format("I_Swapchain::MakeSwapchain: {} is not compiled into this build!", api);
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}
