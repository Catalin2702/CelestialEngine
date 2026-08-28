//
// Module: CelestialEngine/Engine/Modules/Core/Render/Context/Platforms/Common/OpenGl
// File: OpenGlContext.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-08-28
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

static bool _st_VSync = false;

void OpenGlContextEventDispatcher::DispatchContextCreated() const {
	openGlContextLifeCycle.onCreatedDispatcher.Dispatch();
}

void OpenGlContextEventDispatcher::DispatchContextInitialized() const {
	openGlContextLifeCycle.onInitializedDispatcher.Dispatch();
}

void OpenGlContextEventDispatcher::DispatchContextWillShutdown() const {
	openGlContextLifeCycle.onWillShutdownDispatcher.Dispatch();
}

void OpenGlContextEventDispatcher::DispatchVSyncChanged(const bool VSync) const {
	openGlContextLifeCycle.onVSyncChangedDispatcher.Dispatch(VSync);
}

void OpenGlContextEventDispatcher::DispatchResizeEvent(const int width, const int height) const {
	openGlContextLifeCycle.onResizeDispatcher.Dispatch(width, height);
}

// _window is a reference_wrapper, so the pointer is dereferenced in the initializer list: the null check has to happen
// there too, before the dereference, rather than in the constructor body after the fact.
static GLFWwindow& RequireWindow(GLFWwindow* window) {
	assert(window && "OpenGlContext requires a valid GLFW window pointer");
	return *window;
}

OpenGlContext::OpenGlContext(GLFWwindow* window): _window(RequireWindow(window)) {
	glfwMakeContextCurrent(&_window.get());
	openGlContextEventDispatcher.DispatchContextCreated();

	if (const int gladStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)); not gladStatus) [[unlikely]] {
		CE_CORE_ERROR("OpenGlContext::Init: Could not load GLAD");
		throw std::runtime_error("OpenGlContext::Init: Could not load GLAD");
	}

	// The context owns the framebuffer-resize signal (backing pixels), fired to the event hub as a WindowResize. The window
	// keeps the separate window-size callback (points) for its own cached size. Resolve the context through the running
	// application, mirroring how GlfwWindow reaches the app from its own C callbacks.
	glfwSetFramebufferSizeCallback(&_window.get(), [](GLFWwindow*, const int width, const int height) {
		const auto& context = dynamic_cast<GlfwApplication&>(I_Application::StGet()).GetOpenGlContext();
		context.openGlContextEventDispatcher.DispatchResizeEvent(width, height);
	});

	openGlContextEventDispatcher.DispatchContextInitialized();
}

// _window is a std::reference_wrapper, which has no default constructor: it has to be initialized in the member
// initializer list, it cannot be assigned inside the body like a raw pointer.
// The moved-from context is still destroyed, and ~OpenGlContext fires DispatchContextWillShutdown: its dispatchers are
// therefore cleared, so the listener is not told the context shut down when it merely changed address.
OpenGlContext::OpenGlContext(OpenGlContext&& other) noexcept:
	openGlContextEventDispatcher(std::move(other.openGlContextEventDispatcher)),
	_window(other._window) {
	other.openGlContextEventDispatcher = {};
}

OpenGlContext& OpenGlContext::operator=(OpenGlContext&& other) noexcept {
	if (this == &other) [[unlikely]]
		return *this;

	openGlContextEventDispatcher = std::move(other.openGlContextEventDispatcher);
	_window = other._window;
	other.openGlContextEventDispatcher = {};

	return *this;
}

OpenGlContext::~OpenGlContext() {
	openGlContextEventDispatcher.DispatchContextWillShutdown();
}

void OpenGlContext::SwapBuffers() const {
	glfwSwapBuffers(&_window.get());
}

void OpenGlContext::SetVSync(const bool enabled) {
	glfwSwapInterval(enabled ? 1 : 0);
	_st_VSync = enabled;
	openGlContextEventDispatcher.DispatchVSyncChanged(enabled);
}

void OpenGlContext::SetViewport(const int x, const int y, const int width, const int height) {
	glViewport(x, y, width, height);
}

void OpenGlContext::ClearBuffers(const Types::BufferBit mask) {
	glClear(Types::ToInt(mask));
}

bool OpenGlContext::IsVSyncEnabled() const {
	return _st_VSync;
}

std::pair<float, float> OpenGlContext::GetContentScale() const {
	float xScale = 1.0f, yScale = 1.0f;
	glfwGetWindowContentScale(&_window.get(), &xScale, &yScale);
	return {xScale, yScale};
}

}
