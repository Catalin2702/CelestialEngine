//
// Module: CelestialEngine/Engine/Modules/Layers/ImGui/Platforms/Mac/Metal
// File: ImGuiMetalLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_LAYERS_IMGUIMETALLAYER_HPP
#define CE_CORE_LAYERS_IMGUIMETALLAYER_HPP

#include "Core/Layers/ImGui/I_ImGuiLayer.hpp"

#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"

#include <array>
#include <functional>
#include <optional>
#include <semaphore>


namespace CA {
	class MetalDrawable;
	class MetalLayer;
}

namespace MTL {
	class CommandBuffer;
	class RenderCommandEncoder;
}

namespace CE::Events {
	class MouseDraggedEvent;
}

namespace CE::Core {

class CocoaEventHubDispatcher;
class MetalGraphicDevice;
class MetalSwapchain;

class CE_CORE_API ImGuiMetalLayer final: public I_ImGuiLayer {
	enum EventHubSubscription: std::size_t {
		MouseMoved = 0,
		MouseDragged,
		MouseWheelScrolled,
		MouseButtonPressed,
		MouseButtonRelease,
		KeyboardKeyPressed,
		KeyboardKeyReleased,
		KeyboardCharTyped,
		ViewResize,
		_Count
	};

	struct MetalFrameContext {
		CA::MetalDrawable* drawable = nullptr;			///< Pointer to the Metal drawable
		MTL::CommandBuffer* commandBuffer = nullptr;		//< Pointer to the Metal command buffer
		MTL::RenderCommandEncoder* renderCommandEncoder = nullptr;		///< Pointer to the Metal render command encoder
	};

public:
	/**
	 * @brief Default constructor
	 */
	ImGuiMetalLayer();

	~ImGuiMetalLayer() override;

public:
	/**
	 * @brief Renders the layer's ImGui widgets (called between Begin and End)
	 */
	void OnRender() const override;

	/**
	 * @brief Starts a new ImGui frame on the Metal backend
	 * @param deltaTime Time elapsed since the previous frame, fed to the ImGui IO
	 */
	void Begin(f32 deltaTime) override;

	/**
	 * @brief Finalizes the ImGui frame and encodes its draw data into the current Metal render pass
	 */
	void End() override;

public:
	/**
	 * @brief Subscribes this layer's input handlers to the Cocoa event hub
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
	 * @brief Initializes the ImGui context and its Metal/OSX backends
	 */
	void _Init() override;

	/**
	 * @brief Shuts down the ImGui backends and destroys the context
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
	/// The two halves of what used to be MetalContext: the device the ImGui backend allocates its font atlas and its
	/// buffers on, and the swapchain that owns this frame's drawable. Borrowed - the renderer owns both, and it
	/// outlives every layer.
	MetalGraphicDevice* _graphicDevice = nullptr;
	MetalSwapchain* _swapchain = nullptr;

	std::optional<std::reference_wrapper<CocoaWindow>> _window;	///< Cached Cocoa window for the display size
	std::optional<std::reference_wrapper<CocoaEventHubDispatcher>> _eventHub;	///< Hub this layer is subscribed to (non-owning); null when not subscribed
	MetalFrameContext _frameContext;				///< Cached frame context for the current frame

	std::counting_semaphore<3> _renderSemaphore{3};		///< Semaphore to synchronize frame rendering with Metal

	std::array<u32, _Count> _eventHubHandles{};		///< Subscription handles, ordered to match Subscribe/UnsubscribeFromEventHub
};

}

#endif //CE_CORE_LAYERS_IMGUIMETALLAYER_HPP
