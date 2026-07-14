//
// Module: CelestialEngine/Engine/Modules/Core/Layers
// File: I_ImGuiLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_LAYERS_IMGUI_I_IMGUILAYER_HPP
#define CE_CORE_LAYERS_IMGUI_I_IMGUILAYER_HPP

#include "Core/Layers/I_Layer.hpp"

namespace CE::Events {
	class MouseMovedEvent;
	class MouseDraggedEvent;
	class MouseWheelScrolledEvent;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;

	class KeyPressedEvent;
	class KeyReleasedEvent;
	class KeyTypedEvent;

	class WindowResizeEvent;
}

namespace CE::Core {

/**
 * @class I_ImGuiLayer
 * @brief Abstract base class for ImGui overlay layers
 * @details Extends I_Layer to provide a specialized interface for ImGui-based UI layers.
 *			Defines pure virtual methods for handling various input events that are
 *			relevant for ImGui interactions. Concrete implementations handle platform-specific rendering.
 */
class I_ImGuiLayer: public I_Layer {
public:
	I_ImGuiLayer([[maybe_unused]] const std::string& name = "ImGuiLayer"): I_Layer(name) {}

	void OnAttach() override { _Init(); }
	void OnDetach() override { _Shutdown(); }

public:
	void OnUpdate() override {}

protected:
	/**
	 * @brief Platform-specific initialization logic for ImGui layer
	 * @details Initializes ImGui context, sets up rendering backend, and configures input handling.
	 *			Caches necessary pointers for use in rendering. Should be called when the layer is attached.
	 */
	virtual void _Init() = 0;
	/**
	 * @brief Platform-specific shutdown logic for ImGui layer
	 * @details Shuts down ImGui backend and destroys ImGui context. Should be called when the layer is detached.
	 */
	virtual void _Shutdown() = 0;

protected:
	/**
	 * @brief Handles mouse moved events for ImGui
	 * @param event Mouse moved event
	 * @details Pure virtual method for updating mouse cursor position in ImGui. Subscribed to the event hub's
	 *			mouse-moved multicast dispatcher (see the concrete layer's SubscribeToEventHub).
	 */
	virtual void _OnMouseMoved(Events::MouseMovedEvent& event) const = 0;

	/**
	 * @brief Feeds the cursor position to ImGui while a mouse button is held
	 * @details macOS reports cursor movement as mouseDragged (not mouseMoved) while a button is down, so without this ImGui
	 *			would not see the cursor move during a drag and windows could not be moved. Same coordinate space as
	 *			_OnMouseMoved (top-left points).
	 */
	virtual void _OnMouseDragged(Events::MouseDraggedEvent& event) const = 0;

	/**
	 * @brief Handles mouse scrolled events for ImGui
	 * @param event Mouse scrolled event
	 * @details Pure virtual method for processing mouse wheel scrolling in ImGui
	 */
	virtual void _OnMouseScrolled(Events::MouseWheelScrolledEvent& event) const = 0;

	/**
	 * @brief Handles mouse button pressed events for ImGui
	 * @param event Mouse button pressed event
	 * @details Pure virtual method for processing mouse button presses in ImGui context
	 */
	virtual void _OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const = 0;

	/**
	 * @brief Handles mouse button released events for ImGui
	 * @param event Mouse button released event
	 * @details Pure virtual method for processing mouse button releases in ImGui context
	 */
	virtual void _OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const = 0;

	/**
	 * @brief Handles key pressed events for ImGui
	 * @param event Key pressed event
	 * @details Pure virtual method for processing keyboard key presses in ImGui context
	 */
	virtual void _OnKeyPressed(Events::KeyPressedEvent& event) const = 0;

	/**
	 * @brief Handles key released events for ImGui
	 * @param event Key released event
	 * @details Pure virtual method for processing keyboard key releases in ImGui context
	 */
	virtual void _OnKeyReleased(Events::KeyReleasedEvent& event) const = 0;

	/**
	 * @brief Handles key typed events for ImGui
	 * @param event Key typed event (character input)
	 * @details Pure virtual method for processing character input for ImGui text fields
	 */
	virtual void _OnKeyTyped(Events::KeyTypedEvent& event) const = 0;

	/**
	 * @brief Handles window resized events for ImGui
	 * @param event Window resize event
	 * @details Pure virtual method for updating ImGui window when window is resized
	 */
	virtual void _OnWindowResized(Events::WindowResizeEvent& event) const = 0;

protected:
	bool _initialized = false;						///< Flag to track if ImGui Metal backend was successfully initialized
};

}

#endif //CE_CORE_LAYERS_IMGUI_I_IMGUILAYER_HPP