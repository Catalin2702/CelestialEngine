//
// Module: CelestialEngine/Engine/Modules/Core/Layers/ImGui/Platforms/Common/OpenGl
// File: ImGuiOpenGlLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_CORE_LAYERS_IMGUIOPENGLLAYER_HPP
#define CE_CORE_LAYERS_IMGUIOPENGLLAYER_HPP

#include "Core/Layers/ImGui/I_ImGuiLayer.hpp"

#include "Define/DynamicLinker.hpp"

#include <array>
#include <functional>
#include <optional>


namespace CE::Core {
	class OpenGlContext;
	class GlfwWindow;
	class GlfwEventHubDispatcher;
}

namespace CE::Core {

/**
 * @class ImGuiOpenGlLayer
 * @brief ImGui layer implementation for OpenGL rendering
 * @details Concrete implementation of I_ImGuiLayer that uses OpenGL for rendering
 *			ImGui interfaces. This is the cross-platform implementation that works
 *			on Windows, Linux, and macOS. It integrates ImGui with OpenGL windows
 *			and handles all input events through GLFW.
 */
class CE_CORE_API ImGuiOpenGlLayer final: public I_ImGuiLayer {
	enum EventHubSubscription: std::size_t {
		MouseMoved = 0,
		MouseDragged,
		MouseWheelScrolled,
		MouseButtonPressed,
		MouseButtonReleased,
		KeyboardKeyPressed,
		KeyboardKeyReleased,
		KeyboardCharTyped,
		ViewResize,
		_Count
	};

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

public:
	/**
	 * @brief Subscribes this layer's input handlers to the GLFW event hub
	 * @details Input is delivered directly from the hub (no layer-stack traversal): each ImGui handler becomes a hub
	 *			subscriber. Call UnsubscribeFromEventHub before the layer is destroyed to avoid dangling delegates.
	 */
	void SubscribeToEventHub() override;

	/**
	 * @brief Removes this layer's input handlers from the event hub it was subscribed to
	 */
	void UnsubscribeFromEventHub() override;

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

	/**
	 * @brief Feeds the cursor position to ImGui
	 */
	void _OnMouseMoved(Events::MouseMovedEvent& event) const override;
	/**
	 * @brief Feeds the cursor position to ImGui during a drag
	 */
	void _OnMouseDragged(Events::MouseDraggedEvent& event) const override;
	/**
	 * @brief Feeds the scroll wheel offsets to ImGui
	 */
	void _OnMouseScrolled(Events::MouseWheelScrolledEvent& event) const override;
	/**
	 * @brief Feeds the button press to ImGui and consumes the event when ImGui wants the mouse
	 */
	void _OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const override;
	/**
	 * @brief Feeds the button release to ImGui
	 */
	void _OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const override;

	/**
	 * @brief Feeds the key press to ImGui and consumes the event when ImGui wants the keyboard
	 */
	void _OnKeyPressed(Events::KeyPressedEvent& event) const override;
	/**
	 * @brief Feeds the key release to ImGui
	 */
	void _OnKeyReleased(Events::KeyReleasedEvent& event) const override;
	/**
	 * @brief Feeds the typed character to ImGui text input
	 */
	void _OnKeyTyped(Events::KeyTypedEvent& event) const override;

	/**
	 * @brief Updates ImGui's display size on view resize
	 */
	void _OnViewResized(Events::WindowResizeEvent& event) const override;

private:
	std::optional<std::reference_wrapper<OpenGlContext>> _context; ///< Cached OpenGL context pointer
	std::optional<std::reference_wrapper<GlfwWindow>> _window; ///< Cached OpenGL window pointer
	std::optional<std::reference_wrapper<GlfwEventHubDispatcher>> _eventHub; ///< Hub this layer is subscribed to (non-owning); null when not subscribed

	std::array<uint32_t, _Count> _eventHubHandles{};		///< Subscription handles, ordered to match Subscribe/UnsubscribeFromEventHub
};

}

#endif //CE_CORE_LAYERS_IMGUIOPENGLLAYER_HPP
