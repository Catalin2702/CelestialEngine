//
// Module: Layers
// File: I_ImGuiLayer.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-28
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_I_IMGUILAYER_HPP
#define CE_I_IMGUILAYER_HPP

#include "I_Layer.hpp"

namespace CE::Events {

class MouseMovedEvent;
class MouseScrolledEvent;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;

class KeyPressedEvent;
class KeyReleasedEvent;
class KeyTypedEvent;

class WindowResizeEvent;
class WindowCloseEvent;
class AppTickEvent;
class AppUpdateEvent;
class AppRenderEvent;

}

/**
 * @namespace CE::Layers
 * @brief Layer system for organizing application logic into stackable components
 * @details Provides the I_ImGuiLayer interface for defining ImGui overlay layers that can handle
 *          input events specific to ImGui interactions. This allows for a clean separation of
 *          concerns between general application layers and UI-specific layers. The I_ImGuiLayer
 *          interface defines methods for handling keyboard and mouse events, as well as window
 *          resize events, which are essential for responsive UI behavior.
 */
namespace CE::Layers {

/**
 * @class I_ImGuiLayer
 * @brief Abstract base class for ImGui overlay layers
 * @details Extends I_Layer to provide a specialized interface for ImGui-based UI layers.
 *          Defines pure virtual methods for handling various input events that are
 *          relevant for ImGui interactions. Concrete implementations (ImGuiOpenGlLayer,
 *          ImGuiMetalLayer) handle platform-specific rendering.
 */
class I_ImGuiLayer: public I_Layer {
public:
	/**
	 * @brief Constructor
	 * @param name Debug name for the ImGui layer (default: "ImGuiLayer")
	 * @details Creates an ImGui layer with an optional debug name
	 */
	I_ImGuiLayer([[maybe_unused]] const std::string& name = "ImGuiLayer"): I_Layer(name) {}

	/**
	 * @brief Virtual destructor
	 * @details Ensures proper cleanup of derived ImGui layer classes
	 */
	~I_ImGuiLayer() override = default;

protected:
	/**
	 * @brief Handles key pressed events for ImGui
	 * @param event Key pressed event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing keyboard key presses in ImGui context
	 */
	virtual bool OnKeyPressed(Events::KeyPressedEvent& event) = 0;

	/**
	 * @brief Handles key released events for ImGui
	 * @param event Key released event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing keyboard key releases in ImGui context
	 */
	virtual bool OnKeyReleased(Events::KeyReleasedEvent& event) = 0;

	/**
	 * @brief Handles key typed events for ImGui
	 * @param event Key typed event (character input)
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing character input for ImGui text fields
	 */
	virtual bool OnKeyTyped(Events::KeyTypedEvent& event) = 0;

	/**
	 * @brief Handles mouse button pressed events for ImGui
	 * @param event Mouse button pressed event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing mouse button presses in ImGui context
	 */
	virtual bool OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) = 0;

	/**
	 * @brief Handles mouse button released events for ImGui
	 * @param event Mouse button released event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing mouse button releases in ImGui context
	 */
	virtual bool OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) = 0;

	/**
	 * @brief Handles mouse moved events for ImGui
	 * @param event Mouse moved event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for updating mouse cursor position in ImGui
	 */
	virtual bool OnMouseMoved(Events::MouseMovedEvent& event) = 0;

	/**
	 * @brief Handles mouse scrolled events for ImGui
	 * @param event Mouse scrolled event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing mouse wheel scrolling in ImGui
	 */
	virtual bool OnMouseScrolled(Events::MouseScrolledEvent& event) = 0;

	/**
	 * @brief Handles window resized events for ImGui
	 * @param event Window resize event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for updating ImGui window when window is resized
	 */
	virtual bool OnWindowResized(Events::WindowResizeEvent& event) = 0;
};

}

#endif //CE_I_IMGUILAYER_HPP