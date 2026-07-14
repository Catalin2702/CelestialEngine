//
// Module: CelestialEngine/Engine/Modules/Core/Render/Context/Platforms/Common/OpenGl
// File: OpenGlContext.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#include "Core/Render/Context/Platforms/Common/OpenGl/OpenGlContext.hpp"

#include "Core/Application/I_Application.hpp"
#include "Core/Application/Platforms/Common/Glfw/GlfwApplication.hpp"
#include "Tools/Log/Log.hpp"
#include "Types/Render/Platforms/Common/OpenGl/OpenGl.hpp"

#include <cassert>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CE::Core {

void OpenGlContextEventDispatcher::DispatchResizeEvent(const int width, const int height) const {
	onResizeDispatcher.Dispatch(width, height);
}

void OpenGlContextEventDispatcher::DispatchContextCreated() const {
	onCreatedDispatcher.Dispatch();
}

void OpenGlContextEventDispatcher::DispatchContextInitialized() const {
	onInitializedDispatcher.Dispatch();
}

void OpenGlContextEventDispatcher::DispatchContextWillShutdown() const {
	onWillShutdownDispatcher.Dispatch();
}

OpenGlContext::OpenGlContext(GLFWwindow* window): _window(window) {
	openGlContextEventDispatcher.DispatchContextCreated();
}

OpenGlContext::OpenGlContext(void* window): _window(static_cast<GLFWwindow*>(window)) {
	openGlContextEventDispatcher.DispatchContextCreated();
}

OpenGlContext::~OpenGlContext() {
	openGlContextEventDispatcher.DispatchContextWillShutdown();
	_window = nullptr;
}

void OpenGlContext::Init() {
	assert(_window && "OpenGlContext requires a valid GLFW window pointer");
	glfwMakeContextCurrent(_window);

	if (const int gladStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)); not gladStatus) {
		CE_CORE_ERROR("OpenGlContext::Init: Could not load GLAD");
		throw std::runtime_error("OpenGlContext::Init: Could not load GLAD");
	}

	// The context owns the framebuffer-resize signal (backing pixels), fired to the event hub as a WindowResize. The window
	// keeps the separate window-size callback (points) for its own cached size. Resolve the context through the running
	// application, mirroring how GlfwWindow reaches the app from its own C callbacks.
	glfwSetFramebufferSizeCallback(_window, [](GLFWwindow*, const int width, const int height) {
		auto& context = dynamic_cast<GlfwApplication&>(I_Application::StGet()).GetOpenGlContext();
		context.openGlContextEventDispatcher.DispatchResizeEvent(width, height);
	});

	openGlContextEventDispatcher.DispatchContextInitialized();
}

void OpenGlContext::SwapBuffers() const {
	glfwSwapBuffers(_window);
}

void OpenGlContext::SetViewport(const int x, const int y, const int width, const int height) {
	glViewport(x, y, width, height);
}

void OpenGlContext::ClearBuffers(const Types::BufferBit mask) {
	glClear(Types::ToInt(mask));
}

bool OpenGlContext::IsVSyncEnabled() const {
	return false;
}

std::pair<float, float> OpenGlContext::GetContentScale() const {
	if (not _window) {
		CE_CORE_WARN("OpenGlContext::GetContentScale: Could not get content scale because the window is not set.");
		return {1.0f, 1.0f};
	}

	float xScale = 1.0f, yScale = 1.0f;
	glfwGetWindowContentScale(_window, &xScale, &yScale);
	return {xScale, yScale};
}

}
