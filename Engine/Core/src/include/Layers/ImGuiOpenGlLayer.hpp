//
// Module: Layers
// File: ImGuiOpenGlLayer.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-24
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_LAYERS_IMGUIOPENGLLAYER_HPP
#define CE_LAYERS_IMGUIOPENGLLAYER_HPP

#include "Layers/I_ImGuiLayer.hpp"

struct GLFWwindow;

namespace CE::Window {
class OpenGlWindow;
}


/**
 * @namespace CE::Layers
 * @brief Layer system for organizing application logic into stackable components
 * @details Provides the ImGuiOpenGlLayer class, a concrete implementation of I_ImGuiLayer
 *          that integrates ImGui with OpenGL for rendering on Windows and Linux. This layer
 *          handles initialization, rendering, and input events specific to ImGui interactions
 *          in an OpenGL context. It caches pointers to the OpenGL window and GLFW window
 *          to optimize performance by avoiding repeated lookups every frame.
 */
namespace CE::Layers {

/**
 * @class ImGuiOpenGlLayer
 * @brief ImGui layer implementation for OpenGL rendering
 * @details Concrete implementation of I_ImGuiLayer that uses OpenGL for rendering
 *          ImGui interfaces. This is the cross-platform implementation that works
 *          on Windows, Linux, and macOS. It integrates ImGui with OpenGL windows
 *          and handles all input events through GLFW.
 */
class ImGuiOpenGlLayer final: public I_ImGuiLayer {
public:
	/**
	 * @brief Constructor
	 * @details Creates an ImGui OpenGL layer with default initialization
	 */
	ImGuiOpenGlLayer();

public:
	/**
	 * @brief Called when the layer is attached to the layer stack
	 * @details Initializes ImGui context, sets up OpenGL rendering backend,
	 *          configures ImGui for GLFW input, and caches window pointers
	 */
	void OnAttach() override;

	/**
	 * @brief Called when the layer is detached from the layer stack
	 * @details Shuts down ImGui OpenGL backend, GLFW backend, and destroys ImGui context
	 */
	void OnDetach() override;

	/**
	 * @brief Called every frame to render ImGui
	 * @details Sets up new frame, renders ImGui demo window (if enabled),
	 *          and submits rendering commands to OpenGL
	 */
	void OnUpdate() override;

	/**
	 * @brief Handles and dispatches events to appropriate handlers
	 * @param event Reference to the event to process
	 * @return bool True if the event was handled
	 * @details Uses EventDispatcher to route events to specific handler methods
	 */
	bool OnEvent(Events::I_Event& event) override;

protected:
	bool OnKeyPressed(Events::KeyPressedEvent& event) override;
	bool OnKeyReleased(Events::KeyReleasedEvent& event) override;
	bool OnKeyTyped(Events::KeyTypedEvent& event) override;

	bool OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) override;
	bool OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) override;
	bool OnMouseMoved(Events::MouseMovedEvent& event) override;
	bool OnMouseScrolled(Events::MouseScrolledEvent& event) override;

	bool OnWindowResized(Events::WindowResizeEvent& event) override;

private:
	float _time = 0.0f;								///< Time accumulator for frame timing
	GLFWwindow* _glfwWindow = nullptr;				///< Cached GLFW window pointer
	Window::OpenGlWindow* _openGlWindow = nullptr;	///< Cached OpenGL window pointer
};

}

#endif //CE_LAYERS_IMGUIOPENGLLAYER_HPP
