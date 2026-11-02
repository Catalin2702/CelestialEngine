//
// Module: CelestialEngine/Engine/Modules/Core/Window
// File: WindowFactory.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-11-02
//

#include "Core/Window/Window.hpp"
#include "Tools/Tools.hpp"

#if CE_WINDOW_COCOA
	#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#endif

#include <format>
#include <stdexcept>


namespace CE::Core {

std::unique_ptr<I_Window> I_Window::MakeWindow(I_Platform& platform) {
	switch (platform.GetWindowApi()) {
#if CE_WINDOW_GLFW
		case Types::WindowApi::GLFW: {
			// GLFW configures the next window through global, sticky hints, which is an idiom no other backend has -
			// so applying them stays on GlfwPlatform rather than being forced onto I_Platform, and reaching it costs
			// one downcast here. The check is what turns "-wa cocoa with a GLFW window" into a message instead of
			// undefined behaviour.
			const auto* const glfwPlatform = dynamic_cast<GlfwPlatform*>(&platform);
			if (not glfwPlatform) [[unlikely]] {
				constexpr auto error = "I_Window::MakeWindow: a GLFW window needs a GLFW platform!";
				CE_CORE_ERROR(error);
				throw std::runtime_error(error);
			}

			return std::make_unique<GlfwWindow>(*glfwPlatform);
		}
#endif

#if CE_WINDOW_COCOA
		// The Cocoa window needs nothing from the platform: AppKit has no library to configure before creating one.
		case Types::WindowApi::Cocoa:
			return std::make_unique<CocoaWindow>();
#endif

		default: {
			// Either a backend that does not exist yet, or one this build was configured without (CE_RENDER_BACKENDS).
			const auto error = std::format("I_Window::MakeWindow: {} is not compiled into this build!", platform.GetWindowApi());
			CE_CORE_ERROR(error);
			throw std::logic_error(error);
		}
	}
}

}
