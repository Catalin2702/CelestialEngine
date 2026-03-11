//
// Module: CelestialEngine/Engine/Modules/Layers
// File: ImGuiOpenGlLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-03-11
//

#pragma once

#ifndef CE_LAYERS_IMGUIOPENGLLAYER_HPP
#define CE_LAYERS_IMGUIOPENGLLAYER_HPP

#include "Layers/ImGui/I_ImGuiLayer.hpp"

struct GLFWwindow;

namespace CE::Window {
class OpenGlWindow;
}


/**
 * @namespace CE::Layers
 * @brief Layer system for organizing application logic into stackable components
 * @details Provides the ImGuiOpenGlLayer class, a concrete implementation of I_ImGuiLayer
 *			that integrates ImGui with OpenGL for rendering on Windows and Linux. This layer
 *			handles initialization, rendering, and input events specific to ImGui interactions
 *			in an OpenGL context. It caches pointers to the OpenGL window and GLFW window
 *			to optimize performance by avoiding repeated lookups every frame.
 */
namespace CE::Layers {

/**
 * @class ImGuiOpenGlLayer
 * @brief ImGui layer implementation for OpenGL rendering
 * @details Concrete implementation of I_ImGuiLayer that uses OpenGL for rendering
 *			ImGui interfaces. This is the cross-platform implementation that works
 *			on Windows, Linux, and macOS. It integrates ImGui with OpenGL windows
 *			and handles all input events through GLFW.
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
	 *			configures ImGui for GLFW input, and caches window pointers
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
	 *			and submits rendering commands to OpenGL
	 */
	void OnRender() const override;

	/**
	 * @brief Called at the beginning of the frame to set up ImGui state for OpenGL rendering
	 * @details Prepares ImGui for a new frame by setting up the OpenGL rendering state and starting a new ImGui frame.
	 * 			This is called before any ImGui rendering commands are issued each frame.
	 */
	void Begin() override;

	/**
	 * @brief Called at the end of the frame to finalize ImGui rendering
	 * @details Finalizes ImGui rendering by submitting draw data to OpenGL and resetting any modified state.
	 * 			This is called after all ImGui rendering commands have been issued each frame.
	 */
	void End() override;

private:
	GLFWwindow* _glfwWindow = nullptr;				///< Cached GLFW window pointer
	Window::OpenGlWindow* _window = nullptr;		///< Cached OpenGL window pointer
};

}

#endif //CE_LAYERS_IMGUIOPENGLLAYER_HPP
