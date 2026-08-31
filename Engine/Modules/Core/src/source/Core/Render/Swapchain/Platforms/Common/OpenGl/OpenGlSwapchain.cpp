//
// Module: CelestialEngine/Engine/Modules/Core/Render/Swapchain/Platforms/Common/OpenGl
// File: OpenGlSwapchain.cpp
// Created by: Catalin Chirosca
// Created: 2026-08-31
// Updated by: Catalin Chirosca
// Updated: 2026-08-31
//

#include "Core/Render/Swapchain/Platforms/Common/OpenGl/OpenGlSwapchain.hpp"
#include "Tools/Log/Log.hpp"

#include <GLFW/glfw3.h>

#include <stdexcept>


namespace CE::Core {

OpenGlSwapchain::OpenGlSwapchain(GLFWwindow* window): _window(window) {
	if (not _window) [[unlikely]] {
		constexpr auto error = "OpenGlSwapchain::OpenGlSwapchain: The OpenGL swapchain needs a valid GLFWwindow!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	std::ignore = OpenGlSwapchain::AcquireNextTarget();
}

bool OpenGlSwapchain::AcquireNextTarget() {
	i32 width = 0;
	i32 height = 0;
	glfwGetFramebufferSize(_window, &width, &height);

	_width = static_cast<u32>(width);
	_height = static_cast<u32>(height);

	return _width != 0 and height != 0;
}

void OpenGlSwapchain::Present() {
	glfwSwapBuffers(_window);
}

void OpenGlSwapchain::Resize(const u32 width, const u32 height) {
	_width = width;
	_height = height;
}

void OpenGlSwapchain::SetVSync(const bool enabled) {
	glfwSwapInterval(enabled ? 1 : 0);
	_vsync = enabled;
}

}
