//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: OpenGlContext.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-19
//

#include "Render/Context/Platforms/Common/OpenGlContext.hpp"

#include "Tools/Log/Log.hpp"

#include <cassert>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace CE::Render::Context {

OpenGlContext::OpenGlContext(GLFWwindow* window): _window(window) {}

void OpenGlContext::Init() {
	assert(_window && "OpenGlContext requires a valid GLFW window pointer");
	glfwMakeContextCurrent(_window);

	if (const int gladStatus = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)); not gladStatus) {
		CE_CORE_ERROR("OpenGlContext::Init: Could not load GLAD");
		throw std::runtime_error("OpenGlContext::Init: Could not load GLAD");
	}
}

void OpenGlContext::SwapBuffers() {
	glfwSwapBuffers(_window);
}

}
