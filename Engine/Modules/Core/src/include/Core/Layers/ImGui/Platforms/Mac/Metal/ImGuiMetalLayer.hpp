//
// Module: CelestialEngine/Engine/Modules/Layers/ImGui/Platforms/Mac/Metal
// File: ImGuiMetalLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-17
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_LAYERS_IMGUIMETALLAYER_HPP
#define CE_CORE_LAYERS_IMGUIMETALLAYER_HPP

#include "Core/Layers/ImGui/I_ImGuiLayer.hpp"

#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Define/DynamicLinker.hpp"

#include <array>
#include <cstdint>
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

namespace CE::Core {

class CocoaEventHubDispatcher;

class CE_API ImGuiMetalLayer final: public I_ImGuiLayer {

	struct MetalFrameContext {
		CA::MetalDrawable* drawable = nullptr;			///< Pointer to the Metal drawable
		MTL::CommandBuffer* commandBuffer = nullptr;		//< Pointer to the Metal command buffer
		MTL::RenderCommandEncoder* renderCommandEncoder = nullptr;		///< Pointer to the Metal render command encoder
	};

public:
	ImGuiMetalLayer();

	~ImGuiMetalLayer() override;

public:
	void OnRender() const override;

	void OnEvent(Events::I_Event& event) override;

	void Begin(float deltaTime) override;

	void End() override;

public:
	/**
	 * @brief Subscribes this layer's input handlers to the Cocoa event hub
	 * @param hub The application's event hub whose multicast dispatchers drive ImGui input
	 * @details Input is delivered directly from the hub (no layer-stack traversal): each ImGui handler becomes a hub
	 *			subscriber. Call UnsubscribeFromEventHub before the layer is destroyed to avoid dangling delegates.
	 */
	void SubscribeToEventHub(CocoaEventHubDispatcher& hub);

	/**
	 * @brief Removes this layer's input handlers from the event hub it was subscribed to
	 */
	void UnsubscribeFromEventHub();

protected:
	void _Init() override;

	void _Shutdown() override;

	void _OnMouseMoved(Events::MouseMovedEvent& event) const override;

	void _OnMouseScrolled(Events::MouseWheelScrolledEvent& event) const override;

	void _OnMouseButtonPressed(Events::MouseButtonPressedEvent& event) const override;

	void _OnMouseButtonReleased(Events::MouseButtonReleasedEvent& event) const override;

	void _OnKeyPressed(Events::KeyPressedEvent& event) const override;

	void _OnKeyReleased(Events::KeyReleasedEvent& event) const override;

	void _OnKeyTyped(Events::KeyTypedEvent& event) const override;

	void _OnWindowResized(Events::WindowResizeEvent& event) const override;

private:
	std::optional<std::reference_wrapper<MetalContext>> _context;	///< Cached Metal context for rendering
	std::optional<std::reference_wrapper<CocoaWindow>> _window;	///< Cached Cocoa window for event handling and context access
	MetalFrameContext _frameContext;				///< Cached frame context for the current frame

	std::counting_semaphore<3> _renderSemaphore{3};		///< Semaphore to synchronize frame rendering with Metal

	CocoaEventHubDispatcher* _eventHub = nullptr;	///< Hub this layer is subscribed to (non-owning); null when not subscribed
	std::array<uint32_t, 8> _eventHubHandles{};		///< Subscription handles, ordered to match Subscribe/UnsubscribeFromEventHub
};

}

#endif //CE_CORE_LAYERS_IMGUIMETALLAYER_HPP
