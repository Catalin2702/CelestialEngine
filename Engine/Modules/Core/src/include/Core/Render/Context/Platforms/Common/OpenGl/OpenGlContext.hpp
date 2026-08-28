//
// Module: CelestialEngine/Engine/Modules/Core/Render/Context/Platforms/Common/OpenGl
// File: OpenGlContext.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-08-28
//

#pragma once

#ifndef CE_CORE_RENDER_CONTEXT_OPENGLCONTEXT_HPP
#define CE_CORE_RENDER_CONTEXT_OPENGLCONTEXT_HPP

#include "Core/Render/Context/I_Context.hpp"

#include "Define/DynamicLinker.hpp"
#include "Define/Render.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <functional>
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
class CE_CORE_API OpenGlContextEventDispatcher {
	struct OpenGlContextLifeCycle {
		UnicastDispatcher<> onCreatedDispatcher;
		UnicastDispatcher<> onInitializedDispatcher;
		UnicastDispatcher<> onWillShutdownDispatcher;
		UnicastDispatcher<bool> onVSyncChangedDispatcher;
		UnicastDispatcher<int, int> onResizeDispatcher;

		// Every member is a UnicastDispatcher, which is nothrow-movable, so the moves are just memberwise: defaulting them
		// keeps the bindings alive across the move instead of silently dropping them. Copies stay deleted - two live copies
		// of the same lifecycle channel would deliver each event twice.
		OpenGlContextLifeCycle() = default;
		OpenGlContextLifeCycle(const OpenGlContextLifeCycle&) = delete;
		OpenGlContextLifeCycle(OpenGlContextLifeCycle&&) noexcept = default;
		~OpenGlContextLifeCycle() = default;

		OpenGlContextLifeCycle& operator=(const OpenGlContextLifeCycle&) = delete;
		OpenGlContextLifeCycle& operator=(OpenGlContextLifeCycle&&) noexcept = default;
	};

public:
	OpenGlContextEventDispatcher() = default;
	OpenGlContextEventDispatcher(const OpenGlContextEventDispatcher&) = delete;
	OpenGlContextEventDispatcher(OpenGlContextEventDispatcher&&) noexcept = default;
	~OpenGlContextEventDispatcher() = default;

	OpenGlContextEventDispatcher& operator=(const OpenGlContextEventDispatcher&) = delete;
	OpenGlContextEventDispatcher& operator=(OpenGlContextEventDispatcher&&) noexcept = default;

public:
	/**
	 * @brief Forwards the context created callback to the bound listener
	 */
	void DispatchContextCreated() const;
	/**
	 * @brief Forwards the context initialized callback to the bound listener
	 */
	void DispatchContextInitialized() const;
	/**
	 * @brief Forwards the context will shutdown callback to the bound listener
	 */
	void DispatchContextWillShutdown() const;
	/**
	 * @brief Forwards the v sync changed callback to the bound listener
	 */
	void DispatchVSyncChanged(bool VSync) const;
	/**
	 * @brief Forwards the resize callback to the bound listener
	 */
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
class CE_CORE_API OpenGlContext final: public I_Context {
public:
	/**
	 * @brief Constructor
	 * @param window Pointer to the GLFW window associated with this OpenGL context
	 * @details Creates an OpenGlContext instance associated with the specified GLFW window. The constructor does not perform initialization of the OpenGL context; the Init() method must be called separately to set up the context.
	 */
	explicit OpenGlContext(GLFWwindow* window);

	OpenGlContext(const OpenGlContext& other) = delete;

	OpenGlContext(OpenGlContext&& other) noexcept;

	/**
	 * @brief Destructor
	 * @details Cleans up OpenGL resources and disassociates the context from the GLFW window.
	 *			The destructor does not destroy the GLFW window itself, as window management is handled separately.
	 */
	~OpenGlContext() override;

public:
	OpenGlContext& operator=(const OpenGlContext& other) = delete;

	OpenGlContext& operator=(OpenGlContext&& other) noexcept;

public:
	void Init() override {}

	/**
	 * @brief Swaps the front and back buffers
	 * @details Pure virtual method that handles buffer swapping to present the rendered frame on the screen.
	 *			Must be implemented by derived classes to ensure proper presentation of rendered content.
	 */
	void SwapBuffers() const;

	/**
	 * @brief Sets the swap interval and fires a VSyncEvent through the hub
	 */
	void SetVSync(bool enabled) override;

public:
	/**
	 * @brief Sets the OpenGL viewport rectangle
	 */
	static void SetViewport(int x, int y, int width, int height);

	/**
	 * @brief Clears the selected framebuffer attachments
	 */
	static void ClearBuffers(Types::BufferBit mask);

public:
	/**
	 * @brief Checks whether vertical synchronization is currently enabled
	 */
	[[nodiscard]] bool IsVSyncEnabled() const override;

	/**
	 * @brief Gets the content scale (framebuffer pixels per screen coordinate, e.g. 2.0 on Retina)
	 */
	[[nodiscard]] std::pair<float, float> GetContentScale() const override;

RENDER_API_TYPE(OpenGL)

public:
	OpenGlContextEventDispatcher openGlContextEventDispatcher; ///< Fires the context's resize / lifecycle events

private:
	std::reference_wrapper<GLFWwindow> _window;							///< Pointer to the GLFW window associated with this OpenGL context
};

}

#endif //CE_CORE_RENDER_CONTEXT_OPENGLCONTEXT_HPP
