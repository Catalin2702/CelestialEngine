//
// Module: CelestialEngine/Engine/Modules/Layers/ImGui/Platforms/Common/OpenGl
// File: ImGuiOpenGlLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#pragma once

#ifndef CE_LAYERS_IMGUIOPENGLLAYER_HPP
#define CE_LAYERS_IMGUIOPENGLLAYER_HPP

#include "Core/Layers/ImGui/I_ImGuiLayer.hpp"

#include "Define/DynamicLinker.hpp"

#include <functional>
#include <optional>

namespace CE::Render::Context {
class OpenGlContext;
}
namespace CE::Window {
class GlfwWindow;
}


/**
 * @namespace CE::Core::Layers
 * @brief Layer system for organizing application logic into stackable components
 * @details Provides the ImGuiOpenGlLayer class, a concrete implementation of I_ImGuiLayer
 *			that integrates ImGui with OpenGL for rendering on Windows and Linux. This layer
 *			handles initialization, rendering, and input events specific to ImGui interactions
 *			in an OpenGL context. It caches pointers to the OpenGL window and GLFW window
 *			to optimize performance by avoiding repeated lookups every frame.
 */
namespace CE::Core::Layers {

/**
 * @class ImGuiOpenGlLayer
 * @brief ImGui layer implementation for OpenGL rendering
 * @details Concrete implementation of I_ImGuiLayer that uses OpenGL for rendering
 *			ImGui interfaces. This is the cross-platform implementation that works
 *			on Windows, Linux, and macOS. It integrates ImGui with OpenGL windows
 *			and handles all input events through GLFW.
 */
class CE_API ImGuiOpenGlLayer final: public I_ImGuiLayer {
public:
	/**
	 * @brief Constructor
	 * @details Creates an ImGui OpenGL layer with default initialization
	 */
	ImGuiOpenGlLayer();
	/**
	 * @brief Destructor
	 * @details Shuts down ImGui OpenGL backend and releases resources if not already done in OnDetach
	 */
	~ImGuiOpenGlLayer() override;

	// Disable copy and move to prevent issues with cached window pointers
	ImGuiOpenGlLayer(const ImGuiOpenGlLayer&) = delete;
	ImGuiOpenGlLayer& operator=(const ImGuiOpenGlLayer&) = delete;
	ImGuiOpenGlLayer(ImGuiOpenGlLayer&&) = delete;
	ImGuiOpenGlLayer& operator=(ImGuiOpenGlLayer&&) = delete;

public:
	/**
	 * @brief Called every frame to render ImGui
	 * @details Sets up new frame, renders ImGui demo window (if enabled),
	 *			and submits rendering commands to OpenGL
	 */
	void OnRender() const override;

	/**
	 * @brief Called when an event is received
	 * @param event Reference to the event to be processed
	 * @details Dispatches input events to ImGui for processing and updates ImGui's internal state accordingly. This allows ImGui to respond to user interactions such as mouse movement, clicks, keyboard input, and window resizing.
	 */
	void OnEvent(Events::I_Event& event) override;

	/**
	 * @brief Called at the beginning of the frame to set up ImGui state for OpenGL rendering
	 * @details Prepares ImGui for a new frame by setting up the OpenGL rendering state and starting a new ImGui frame.
	 * 			This is called before any ImGui rendering commands are issued each frame.
	 */
	void Begin(float deltaTime) override;

	/**
	 * @brief Called at the end of the frame to finalize ImGui rendering
	 * @details Finalizes ImGui rendering by submitting draw data to OpenGL and resetting any modified state.
	 * 			This is called after all ImGui rendering commands have been issued each frame.
	 */
	void End() override;

protected:
	/**
	 * @brief Platform-specific initialization logic for ImGui layer
	 * @details Initializes ImGui for OpenGL rendering, sets up necessary context and resources,
	 *			and caches pointers to the GLFW window and OpenGL window for use in rendering.
	 */
	void _Init() override;

	/**
	 * @brief Platform-specific shutdown logic for ImGui layer
	 * @details Shuts down ImGui for OpenGL rendering and releases any resources allocated during initialization.
	 */
	void _Shutdown() override;

	bool _OnMouseMoved(Events::MouseMovedEvent& event) const override;
	bool _OnMouseScrolled(Events::MouseScrolledEvent& event) const override;
	bool _OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const override;
	bool _OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const override;

	bool _OnKeyPressed(Events::KeyPressedEvent& event) const override;
	bool _OnKeyReleased(Events::KeyReleasedEvent& event) const override;
	bool _OnKeyTyped(Events::KeyTypedEvent& event) const override;

	bool _OnWindowResized(Events::WindowResizeEvent& event) const override;

private:
	std::optional<std::reference_wrapper<Render::Context::OpenGlContext>> _context; ///< Cached OpenGL context pointer
	std::optional<std::reference_wrapper<Window::GlfwWindow>> _window; ///< Cached OpenGL window pointer
};

}

#endif //CE_LAYERS_IMGUIOPENGLLAYER_HPP
