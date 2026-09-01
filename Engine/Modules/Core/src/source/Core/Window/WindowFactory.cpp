//
// Module: CelestialEngine/Engine/Modules/Core/Window
// File: WindowFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/Window/I_Window.hpp"
#include "Core/Window/I_Platform.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwPlatform.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwWindow.hpp"
#include "Tools/Log/Log.hpp"

#if CE_PLATFORM_MACOS
	#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#endif

#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_Window> I_Window::MakeWindow(I_Platform& platform, const Types::WindowApi windowApi) {
	switch (windowApi) {
		case Types::WindowApi::GLFW: {
			// GLFW configures the next window through global, sticky hints, which is an idiom no other backend has -
			// so applying them stays on GlfwPlatform rather than being forced onto I_Platform, and reaching it costs
			// one downcast here. The check is what turns "-wa cocoa with a GLFW window" into a message instead of
			// undefined behaviour.
			auto* const glfwPlatform = dynamic_cast<GlfwPlatform*>(&platform);
			if (not glfwPlatform) [[unlikely]] {
				constexpr auto error = "I_Window::MakeWindow: a GLFW window needs a GLFW platform!";
				CE_CORE_ERROR(error);
				throw std::runtime_error(error);
			}

			return std::make_unique<GlfwWindow>(*glfwPlatform);
		}

#if CE_PLATFORM_MACOS
		// The Cocoa window needs nothing from the platform: AppKit has no library to configure before creating one.
		case Types::WindowApi::Cocoa:
			return std::make_unique<CocoaWindow>();
#endif

		default: {
			constexpr auto error = "I_Window::MakeWindow: Not yet implemented!";
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}
