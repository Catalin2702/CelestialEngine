//
// Module: CelestialEngine/Engine/Modules/Core/Window
// File: PlatformFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/Window/I_Platform.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwPlatform.hpp"
#include "Tools/Log/Log.hpp"

#if CE_PLATFORM_MACOS
	#include "Core/Window/Platforms/Mac/Cocoa/CocoaPlatform.hpp"
#endif

#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_Platform> I_Platform::MakePlatform(const Types::WindowApi windowApi) {
	switch (windowApi) {
		case Types::WindowApi::GLFW:
			return std::make_unique<GlfwPlatform>();

#if CE_PLATFORM_MACOS
		case Types::WindowApi::Cocoa:
			return std::make_unique<CocoaPlatform>();
#endif

		default: {
			constexpr auto error = "I_Platform::MakePlatform: Not yet implemented!";
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}
