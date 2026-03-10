//
// Module: CelestialEngine/Engine/Modules/Layers
// File: I_ImGuiLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-28
// Updated by: Catalin Chirosca
// Updated: 2026-03-10
//

#pragma once

#ifndef CE_I_IMGUILAYER_HPP
#define CE_I_IMGUILAYER_HPP

#include "Layers/I_Layer.hpp"

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
 *			input events specific to ImGui interactions. This allows for a clean separation of
 *			concerns between general application layers and UI-specific layers. The I_ImGuiLayer
 *			interface defines methods for handling keyboard and mouse events, as well as window
 *			resize events, which are essential for responsive UI behavior.
 */
namespace CE::Layers {

/**
 * @class I_ImGuiLayer
 * @brief Abstract base class for ImGui overlay layers
 * @details Extends I_Layer to provide a specialized interface for ImGui-based UI layers.
 *			Defines pure virtual methods for handling various input events that are
 *			relevant for ImGui interactions. Concrete implementations (ImGuiOpenGlLayer,
 *			ImGuiMetalLayer) handle platform-specific rendering.
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

public:
	/**
	 * @brief Pure virtual method for handling creation frame logic
	 */
	virtual void Begin() = 0;
	/**
	 * @brief Pure virtual method for handling rendering frame logic
	 */
	virtual void End() = 0;

protected:
	mutable float _time = 0.0f;								///< Time accumulator for frame timing
	bool _currentFrameStarted = false;						///< Flag to track if the current frame has started
};

}

#endif //CE_I_IMGUILAYER_HPP