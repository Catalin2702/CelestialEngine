//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: OpenGlContext.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-30
//

#pragma once

#ifndef CE_RENDER_CONTEXT_OPENGLCONTEXT_HPP
#define CE_RENDER_CONTEXT_OPENGLCONTEXT_HPP

#include "Render/Context/I_Context.hpp"

#include "Define/DynamicLinker.hpp"
#include "Define/Render.hpp"

#include <utility>

struct GLFWwindow;

namespace CE::Types::Render {
enum class BufferBit: uint32_t;
}

namespace CE::Render::Context {

/**
 * @class OpenGlContext
 * @brief Cross-platform graphics context implementation using OpenGL API
 * @details Provides a graphics context implementation that uses the OpenGL API for rendering.
 *			Manages an OpenGL context associated with a GLFW window, allowing for cross-platform rendering using OpenGL.
 *			This class is designed for applications that want to use OpenGL for rendering across multiple platforms using GLFW for window management.
 */
class CE_API OpenGlContext final: public I_Context {
public:
	/**
	 * @brief Constructor
	 * @param window Pointer to the GLFW window associated with this OpenGL context
	 * @details Creates an OpenGlContext instance associated with the specified GLFW window. The constructor does not perform initialization of the OpenGL context; the Init() method must be called separately to set up the context.
	 */
	explicit OpenGlContext(GLFWwindow* window);

	/**
	 * @brief Constructor
	 * @param window Generic pointer to the GLFW window associated with this OpenGL context
	 * @details Creates an OpenGlContext instance associated with the specified GLFW window. The constructor does not perform initialization of the OpenGL context; the Init() method must be called separately to set up the context. The window parameter is expected to be a pointer to a GLFWwindow, and it will be cast accordingly.
	 */
	explicit OpenGlContext(void* window);

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

	static void SetViewport(int x, int y, int width, int height);

	static void ClearBuffers(Types::Render::BufferBit mask);

public:
	/**
	 * @brief Checks if VSync is currently enabled
	 * @return bool True if VSync is enabled, false otherwise
	 * @details Returns the current state of VSync in the OpenGL context. This method can be implemented to query the current VSync state based on the context's settings or platform-specific APIs.
	 */
	[[nodiscard]] bool IsVSyncEnabled() const override;

public:
	[[nodiscard]] void* GetNativeDevice() const override { return nullptr; }
	[[nodiscard]] void* GetNativeLayer() const override { return nullptr; }
	[[nodiscard]] void* GetNativeCommandQueue() const override { return nullptr; }

RENDER_API_TYPE(OpenGL)

private:
	GLFWwindow* _window;							///< Pointer to the GLFW window associated with this OpenGL context
};

}

#endif //CE_RENDER_CONTEXT_OPENGLCONTEXT_HPP
