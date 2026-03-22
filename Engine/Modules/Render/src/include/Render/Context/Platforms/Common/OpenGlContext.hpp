//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: OpenGlContext.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
//

#pragma once

#ifndef CE_RENDER_CONTEXT_OPENGLCONTEXT_HPP
#define CE_RENDER_CONTEXT_OPENGLCONTEXT_HPP

#include "Render/Context/I_Context.hpp"

#include "Define/Render.hpp"

#include <utility>

struct GLFWwindow;

namespace CE::Render::Context {

/**
 * @class OpenGlContext
 * @brief Cross-platform graphics context implementation using OpenGL API
 * @details Provides a graphics context implementation that uses the OpenGL API for rendering.
 *			Manages an OpenGL context associated with a GLFW window, allowing for cross-platform rendering using OpenGL.
 *			This class is designed for applications that want to use OpenGL for rendering across multiple platforms using GLFW for window management.
 */
class OpenGlContext final: public I_Context {
public:
	/**
	 * @brief Constructor
	 * @param window Pointer to the GLFW window associated with this OpenGL context
	 * @details Creates an OpenGlContext instance associated with the specified GLFW window. The constructor does not perform initialization of the OpenGL context; the Init() method must be called separately to set up the context.
	 */
	OpenGlContext(GLFWwindow* window);

	/**
	 * @brief Destructor
	 * @details Cleans up OpenGL resources and disassociates the context from the GLFW window.
	 *			The destructor does not destroy the GLFW window itself, as window management is handled separately.
	 */
	~OpenGlContext() override;

public:
	void HandleContentSizeChange(const std::pair<float, float>&) override {}
	void HandleVSyncChange(const bool) override {}

public:
	/**
	 * @brief Initializes the OpenGL context
	 * @details Pure virtual method that initializes the OpenGL context, sets up necessary resources,
	 *			and prepares it for rendering. Must be implemented by derived classes.
	 */
	void Init() override;
	/**
	 * @brief Swaps the front and back buffers
	 * @details Pure virtual method that handles buffer swapping to present the rendered frame on the screen.
	 *			Must be implemented by derived classes to ensure proper presentation of rendered content.
	 */
	void SwapBuffers() override;

public:
	void* GetNativeDevice() const override { return nullptr; }
	void* GetNativeLayer() const override { return nullptr; }
	void* GetNativeCommandQueue() const override { return nullptr; }
	void* GetRenderPassDescriptor() const override { return nullptr; }

RENDER_API_TYPE(OpenGL)

private:
	GLFWwindow* _window;							///< Pointer to the GLFW window associated with this OpenGL context
};

}

#endif //CE_RENDER_CONTEXT_OPENGLCONTEXT_HPP
