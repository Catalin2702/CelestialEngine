//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: OpenGlContext.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-21
//

#pragma once

#ifndef CE_RENDER_CONTEXT_OPENGLCONTEXT_HPP
#define CE_RENDER_CONTEXT_OPENGLCONTEXT_HPP

#include "Render/Context/I_Context.hpp"

#include "Define/Render.hpp"

#include <utility>

struct GLFWwindow;

namespace CE::Render::Context {

class OpenGlContext final: public I_Context {
public:
	OpenGlContext(GLFWwindow* window);

	~OpenGlContext() override = default;

public:
	void HandleContentScaleChange(const std::pair<float, float>&) override {}

public:
	void Init() override;
	void SwapBuffers() override;

RENDER_API_TYPE(OpenGL)

private:
	GLFWwindow* _window;							///< Pointer to the GLFW window associated with this OpenGL context
};

}

#endif //CE_RENDER_CONTEXT_OPENGLCONTEXT_HPP
