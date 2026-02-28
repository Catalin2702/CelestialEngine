//
// Module: Layers
// File: ImGuiMetalLayer.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-24
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_LAYERS_IMGUIMETALLAYER_HPP
#define CE_LAYERS_IMGUIMETALLAYER_HPP

#include "Layers/I_ImGuiLayer.hpp"

struct GLFWwindow;

namespace CA {
class MetalLayer;
}
namespace MTL {
class CommandQueue;
class Device;
}
namespace CE::Window {
class MetalViewport;
}


/**
 * @namespace CE::Layers
 * @brief Layer system for organizing application logic into stackable components
 * @details Provides the ImGuiMetalLayer class, a concrete implementation of I_ImGuiLayer
 *          that integrates ImGui with Apple's Metal API for rendering on macOS. This layer
 *          handles initialization, rendering, and input events specific to ImGui interactions
 *          in a Metal context. It caches pointers to the Metal viewport, device, command queue,
 *          and layer to optimize performance by avoiding repeated lookups every frame.
 */
namespace CE::Layers {

/**
 * @class ImGuiMetalLayer
 * @brief ImGui layer implementation for Metal rendering on macOS
 * @details Concrete implementation of I_ImGuiLayer that uses Apple's Metal API
 *          for rendering ImGui interfaces. This layer integrates ImGui with the
 *          Metal viewport, handling initialization, rendering, and input events.
 *          It caches viewport resources to avoid repeated lookups every frame.
 */
class ImGuiMetalLayer final: public I_ImGuiLayer {
public:
	/**
	 * @brief Constructor
	 * @details Creates an ImGui Metal layer with default initialization
	 */
	ImGuiMetalLayer();

public:
	/**
	 * @brief Called when the layer is attached to the layer stack
	 * @details Initializes ImGui context, sets up Metal rendering backend,
	 *          configures ImGui for GLFW input, and caches viewport pointers
	 */
	void OnAttach() override;

	/**
	 * @brief Called when the layer is detached from the layer stack
	 * @details Shuts down ImGui Metal backend, GLFW backend, and destroys ImGui context
	 */
	void OnDetach() override;

	/**
	 * @brief Called every frame to render ImGui
	 * @details Sets up new frame, renders ImGui demo window (if enabled),
	 *          and submits rendering commands to Metal command queue
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
	/**
	 * @brief Handles key pressed events
	 * @param event Key pressed event
	 * @return bool Always returns false to allow event propagation
	 */
	bool OnKeyPressed(Events::KeyPressedEvent& event) override;

	/**
	 * @brief Handles key released events
	 * @param event Key released event
	 * @return bool Always returns false to allow event propagation
	 */
	bool OnKeyReleased(Events::KeyReleasedEvent& event) override;

	/**
	 * @brief Handles key typed events (character input)
	 * @param event Key typed event
	 * @return bool Always returns false to allow event propagation
	 */
	bool OnKeyTyped(Events::KeyTypedEvent& event) override;

	/**
	 * @brief Handles mouse button pressed events
	 * @param event Mouse button pressed event
	 * @return bool Always returns false to allow event propagation
	 */
	bool OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) override;

	/**
	 * @brief Handles mouse button released events
	 * @param event Mouse button released event
	 * @return bool Always returns false to allow event propagation
	 */
	bool OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) override;

	/**
	 * @brief Handles mouse moved events
	 * @param event Mouse moved event
	 * @return bool Always returns false to allow event propagation
	 */
	bool OnMouseMoved(Events::MouseMovedEvent& event) override;

	/**
	 * @brief Handles mouse scrolled events
	 * @param event Mouse scrolled event
	 * @return bool Always returns false to allow event propagation
	 */
	bool OnMouseScrolled(Events::MouseScrolledEvent& event) override;

	/**
	 * @brief Handles window resized events
	 * @param event Window resize event
	 * @return bool Always returns false to allow event propagation
	 */
	bool OnWindowResized(Events::WindowResizeEvent& event) override;

private:
	float _time = 0.0f;								///< Time accumulator for frame timing

	// Cached pointers to avoid repeated lookups every frame
	Window::MetalViewport* _viewport = nullptr;		///< Cached Metal viewport pointer
	GLFWwindow* _glfwWindow = nullptr;				///< Cached GLFW window pointer
	MTL::Device* _metalDevice = nullptr;			///< Cached Metal device pointer
	MTL::CommandQueue* _commandQueue = nullptr;		///< Cached Metal command queue pointer
	CA::MetalLayer* _metalLayer = nullptr;			///< Cached Metal layer pointer

};

}


#endif //CE_LAYERS_IMGUIMETALLAYER_HPP
