//
// Module: CelestialEngine/Engine/Modules/Core/Render/Context/Platforms/Common/OpenGl
// File: OpenGlContext.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-07-21
//

#pragma once

#ifndef CE_CORE_RENDER_CONTEXT_OPENGLCONTEXT_HPP
#define CE_CORE_RENDER_CONTEXT_OPENGLCONTEXT_HPP

#include "Core/Render/Context/I_Context.hpp"

#include "Define/DynamicLinker.hpp"
#include "Define/Render.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <utility>

struct GLFWwindow;

namespace CE::Types {
	enum class BufferBit: uint32_t;
}

namespace CE::Core {

/**
 * @class OpenGlContextEventDispatcher
 * @brief Owns the unicast dispatchers the OpenGL context fires for the events it originates
 * @details The framebuffer-resize event (in backing pixels) is bound by the application to the event hub's
 *			ReceiveWindowResizeEvent; the lifecycle signals mark the context's creation, initialization and shutdown.
 */
class CE_API OpenGlContextEventDispatcher {
	struct OpenGlContextLifeCycle {
		UnicastDispatcher<> onCreatedDispatcher;
		UnicastDispatcher<> onInitializedDispatcher;
		UnicastDispatcher<> onWillShutdownDispatcher;
		UnicastDispatcher<bool> onVSyncChangedDispatcher;
		UnicastDispatcher<int, int> onResizeDispatcher;
	};
public:
	void DispatchContextCreated() const;
	void DispatchContextInitialized() const;
	void DispatchContextWillShutdown() const;
	void DispatchVSyncChanged(bool VSync) const;
	void DispatchResizeEvent(int width, int height) const;

public:
	OpenGlContextLifeCycle openGlContextLifeCycle;
};

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
	void SwapBuffers() const;

	void SetVSync(bool enabled) override;

public:
	static void SetViewport(int x, int y, int width, int height);

	static void ClearBuffers(Types::BufferBit mask);

public:
	[[nodiscard]] bool IsVSyncEnabled() const override;

	[[nodiscard]] std::pair<float, float> GetContentScale() const override;

RENDER_API_TYPE(OpenGL)

public:
	OpenGlContextEventDispatcher openGlContextEventDispatcher; ///< Fires the context's resize / lifecycle events

private:
	GLFWwindow* _window;							///< Pointer to the GLFW window associated with this OpenGL context
};

}

#endif //CE_CORE_RENDER_CONTEXT_OPENGLCONTEXT_HPP
