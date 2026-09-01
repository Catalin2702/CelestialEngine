//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Common/Glfw
// File: GlfwPlatform.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-02
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#include "Core/Window/Platforms/Common/Glfw/GlfwPlatform.hpp"

#include "Tools/Log/Log.hpp"

#include <GLFW/glfw3.h>

#include <cassert>
#include <stdexcept>


namespace CE::Core {

namespace {

// The GLFW error callback is a plain C function pointer with no user data slot, so the platform holding the dispatcher
// has to be reachable from file scope. Set before glfwInit and cleared in the destructor, so it always names a live
// object for as long as GLFW can call back - unlike the application singleton, which is not published yet while the
// first window is being built.
GlfwPlatform* g_st_Platform = nullptr;

}

GlfwPlatform::GlfwPlatform() {
	assert(g_st_Platform == nullptr && "GlfwPlatform::GlfwPlatform: GLFW is already initialized!");
	g_st_Platform = this;

	glfwSetErrorCallback([](const int errorCode, const char* description) {
		if (g_st_Platform) [[likely]]
			g_st_Platform->onErrorDispatcher.Dispatch(errorCode, description);
	});

	if (not glfwInit()) [[unlikely]] {
		glfwSetErrorCallback(nullptr);
		g_st_Platform = nullptr;

		constexpr auto error = "GlfwPlatform::GlfwPlatform: Could not initialize GLFW!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}
}

GlfwPlatform::~GlfwPlatform() {
	glfwTerminate();
	glfwSetErrorCallback(nullptr);

	g_st_Platform = nullptr;
}

void GlfwPlatform::PollEvents() const {
	glfwPollEvents();
}

void GlfwPlatform::ApplyWindowHints(const Types::GraphicsApi graphicsApi) const {
	// Hints are global and sticky: without this every hint set for a previous window would still apply to the next one.
	glfwDefaultWindowHints();

	switch (graphicsApi) {
		case Types::GraphicsApi::OpenGL:
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
			return;

		// Metal, Vulkan and DirectX all create their own surface from the native handle, so GLFW must not make a
		// context: asking it for one would fail outright on a machine with no OpenGL driver.
		case Types::GraphicsApi::Metal:
		case Types::GraphicsApi::Vulkan:
		case Types::GraphicsApi::DirectX11:
		case Types::GraphicsApi::DirectX12:
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			return;

		case Types::GraphicsApi::None:
			break;
	}

	CE_CORE_WARN("GlfwPlatform::ApplyWindowHints: no hints defined for graphics API {}, creating the window without a client API.", static_cast<u32>(graphicsApi));
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
}

}
