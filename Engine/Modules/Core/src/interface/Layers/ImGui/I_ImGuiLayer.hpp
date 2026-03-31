//
// Module: CelestialEngine/Engine/Modules/Layers
// File: I_ImGuiLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-04-01
//

#pragma once

#ifndef CE_LAYERS_IMGUI_I_IMGUILAYER_HPP
#define CE_LAYERS_IMGUI_I_IMGUILAYER_HPP

#include "Layers/Render/I_RenderLayer.hpp"

namespace CE::Events {

class MouseMovedEvent;
class MouseScrolledEvent;
class MouseButtonPressedEvent;
class MouseButtonReleasedEvent;

class KeyPressedEvent;
class KeyReleasedEvent;
class KeyTypedEvent;

class WindowResizeEvent;

}

namespace CE::Layers {

/**
 * @class I_ImGuiLayer
 * @brief Abstract base class for ImGui overlay layers
 * @details Extends I_Layer to provide a specialized interface for ImGui-based UI layers.
 *			Defines pure virtual methods for handling various input events that are
 *			relevant for ImGui interactions. Concrete implementations handle platform-specific rendering.
 */
class I_ImGuiLayer: public I_RenderLayer {
public:
	I_ImGuiLayer([[maybe_unused]] const std::string& name = "ImGuiLayer"): I_RenderLayer(name) {}

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
	 * @return bool True if the event was handled
	 * @details Pure virtual method for updating mouse cursor position in ImGui
	 * @brief Pure virtual method for handling creation frame logic
	 */
	virtual bool _OnMouseMoved(Events::MouseMovedEvent& event) const = 0;

	/**
	 * @brief Handles mouse scrolled events for ImGui
	 * @param event Mouse scrolled event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing mouse wheel scrolling in ImGui
	 * @brief Pure virtual method for handling rendering frame logic
	 */
	virtual bool _OnMouseScrolled(Events::MouseScrolledEvent& event) const = 0;

	/**
	 * @brief Handles mouse button pressed events for ImGui
	 * @param event Mouse button pressed event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing mouse button presses in ImGui context
	 */
	virtual bool _OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const = 0;

	/**
	 * @brief Handles mouse button released events for ImGui
	 * @param event Mouse button released event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing mouse button releases in ImGui context
	 */
	virtual bool _OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const = 0;

	/**
	 * @brief Handles key pressed events for ImGui
	 * @param event Key pressed event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing keyboard key presses in ImGui context
	 */
	virtual bool _OnKeyPressed(Events::KeyPressedEvent& event) const = 0;

	/**
	 * @brief Handles key released events for ImGui
	 * @param event Key released event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing keyboard key releases in ImGui context
	 */
	virtual bool _OnKeyReleased(Events::KeyReleasedEvent& event) const = 0;

	/**
	 * @brief Handles key typed events for ImGui
	 * @param event Key typed event (character input)
	 * @return bool True if the event was handled
	 * @details Pure virtual method for processing character input for ImGui text fields
	 */
	virtual bool _OnKeyTyped(Events::KeyTypedEvent& event) const = 0;

	/**
	 * @brief Handles window resized events for ImGui
	 * @param event Window resize event
	 * @return bool True if the event was handled
	 * @details Pure virtual method for updating ImGui window when window is resized
	 */
	virtual bool _OnWindowResized(Events::WindowResizeEvent& event) const = 0;

protected:
	bool _initialized = false;						///< Flag to track if ImGui Metal backend was successfully initialized
};

}

#endif //CE_LAYERS_IMGUI_I_IMGUILAYER_HPP