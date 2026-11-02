//
// Module: CelestialEngine/Engine/Modules/Core/Window
// File: PlatformFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Core/Window/Window.hpp"
#include "Tools/Tools.hpp"

#if CE_WINDOW_COCOA
	#include "Core/Window/Platforms/Mac/Cocoa/CocoaPlatform.hpp"
#endif

#include <format>
#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_Platform> I_Platform::MakePlatform(const Types::WindowApi windowApi) {
	switch (windowApi) {
#if CE_WINDOW_GLFW
		case Types::WindowApi::GLFW:
			return std::make_unique<GlfwPlatform>();
#endif

#if CE_WINDOW_COCOA
		case Types::WindowApi::Cocoa:
			return std::make_unique<CocoaPlatform>();
#endif

		default: {
			// Either a backend that does not exist yet, or one this build was configured without (CE_RENDER_BACKENDS).
			const auto error = std::format("I_Platform::MakePlatform: {} is not compiled into this build!", windowApi);
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}
