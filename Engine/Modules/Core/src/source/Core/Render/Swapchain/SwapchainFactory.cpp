//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain
// File: SwapchainFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/Render/Swapchain/I_Swapchain.hpp"
#include "Core/Render/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.hpp"
#include "Core/Render/Surface/I_OpenGlSurface.hpp"
#include "Core/Window/I_Window.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Types.hpp"

#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_Swapchain> I_Swapchain::MakeSwapchain(I_Window& window, const Types::GraphicsApi api) {
	switch (api) {
		case Types::GraphicsApi::OpenGL: {
			auto* const surface = dynamic_cast<I_OpenGlSurface*>(&window);
			if (not surface) [[unlikely]] {
				constexpr auto error = "I_Swapchain::MakeSwapchain: the window does not provide an OpenGL surface!";
				CE_CORE_ERROR(error);
				throw std::runtime_error(error);
			}
			return std::make_unique<OpenGlSwapchain>(*surface);
		}
		case Types::GraphicsApi::Metal:
		default: {
			constexpr auto error = "I_Swapchain::MakeSwapchain: Not yet implemented!";
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}