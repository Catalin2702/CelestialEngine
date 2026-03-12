//
// Module: CelestialEngine/Engine/Modules/Layers
// File: ImGuiMetalLayer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-24
// Updated by: Catalin Chirosca
// Updated: 2026-03-12
//

#pragma once

#ifndef CE_LAYERS_IMGUIMETALLAYER_HPP
#define CE_LAYERS_IMGUIMETALLAYER_HPP

#include "Layers/ImGui/I_ImGuiLayer.hpp"

#include <Foundation/Foundation.hpp>

struct GLFWwindow;

namespace CA {
class MetalDrawable;
class MetalLayer;
}
namespace MTL {
class CommandBuffer;
class CommandQueue;
class Device;
class RenderCommandEncoder;
class RenderPassDescriptor;
}
namespace NS {
class AutoreleasePool;
}
namespace CE::Window {
class MetalWindow;
}


/**
 * @namespace CE::Layers
 * @brief Layer system for organizing application logic into stackable components
 * @details Provides the ImGuiMetalLayer class, a concrete implementation of I_ImGuiLayer
 *			that integrates ImGui with Apple's Metal API for rendering on macOS. This layer
 *			handles initialization, rendering, and input events specific to ImGui interactions
 *			in a Metal context. It caches pointers to the Metal window, device, command queue,
 *			and layer to optimize performance by avoiding repeated lookups every frame.
 */
namespace CE::Layers {

/**
 * @class ImGuiMetalLayer
 * @brief ImGui layer implementation for Metal rendering on macOS
 * @details Concrete implementation of I_ImGuiLayer that uses Apple's Metal API
 *			for rendering ImGui interfaces. This layer integrates ImGui with the
 *			Metal window, handling initialization, rendering, and input events.
 *			It caches window resources to avoid repeated lookups every frame.
 */
class ImGuiMetalLayer final: public I_ImGuiLayer {
public:
	/**
	 * @brief Constructor
	 * @details Creates an ImGui Metal layer with default initialization
	 */
	ImGuiMetalLayer();

	~ImGuiMetalLayer() override;

public:
	/**
	 * @brief Called every frame to render ImGui
	 * @details Sets up new frame, renders ImGui demo window (if enabled),
	 *			and submits rendering commands to Metal command queue
	 */
	void OnRender() const override;

	void OnEvent(Events::I_Event& event) override;

	/**
	 * @brief Called at the beginning of the frame to set up ImGui state for Metal rendering
	 * @details Prepares ImGui for a new frame, updates display size and delta time, and sets up
	 *			ImGui for rendering with Metal. Caches necessary pointers for use in End().
	 *			Should be called at the beginning of each frame before rendering ImGui interfaces.
	 */
	void Begin() override;

	/**
	 * @brief Handles the end of an ImGui frame for Metal rendering
	 * @details Updates ImGui display size and delta time, finalizes ImGui rendering, and presents the frame
	 *			to the screen using Metal command queue
	 */
	void End() override;

protected:
	/**
	 * @brief Platform-specific initialization logic for ImGui Metal layer
	 * @details Initializes ImGui context, sets up Metal rendering backend,
	 *			configures ImGui for GLFW input, and caches window pointers
	 */
	void _Init() override;
	/**
	 * @brief Platform-specific shutdown logic for ImGui Metal layer
	 * @details Shuts down ImGui Metal backend, GLFW backend, and destroys ImGui context
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
	// Cached pointers to avoid repeated lookups every frame
	Window::MetalWindow* _window = nullptr;			///< Cached Metal window pointer
	GLFWwindow* _glfwWindow = nullptr;				///< Cached GLFW window pointer
	MTL::Device* _metalDevice = nullptr;			///< Cached Metal device pointer
	MTL::CommandQueue* _commandQueue = nullptr;		///< Cached Metal command queue pointer
	CA::MetalLayer* _metalLayer = nullptr;			///< Cached Metal layer pointer

	// Cached pointers to pass from Begin() to End() without needing to look them up again
	CA::MetalDrawable* _currentDrawable = nullptr;	///< Cached Metal drawable pointer
	MTL::CommandBuffer* _currentCommandBuffer = nullptr;	///< Cached Metal command buffer pointer
	MTL::RenderCommandEncoder* _currentRenderCommandEncoder = nullptr; ///< Cached Metal render command encoder pointer
	MTL::RenderPassDescriptor* _currentRenderPassDescriptor = nullptr; ///< Cached Metal render pass descriptor pointer
	NS::SharedPtr<NS::AutoreleasePool> _currentAutoreleasePool = nullptr; ///< Cached autorelease pool for current frame
};

}

#endif //CE_LAYERS_IMGUIMETALLAYER_HPP
