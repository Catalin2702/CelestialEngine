//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: OpenGlContext.cpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-23
//

#include "Render/Context/Platforms/Common/OpenGlContext.hpp"

#include "Tools/Log/Log.hpp"

#include <cassert>
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace CE::Render::Context {

OpenGlContext::OpenGlContext(GLFWwindow* window): _window(window) {}

OpenGlContext::~OpenGlContext() {
	_window = nullptr;
}

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

void OpenGlContext::SetViewport(const int x, const int y, const int width, const int height) {
	glViewport(x, y, width, height);
}

void OpenGlContext::ClearBuffers(const unsigned int mask) {
	glClear(mask);
}

}
