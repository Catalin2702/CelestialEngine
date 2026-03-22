//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: MetalContext.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
//

#pragma once

#ifndef CE_RENDER_CONTEXT_METALCONTEXT_HPP
#define CE_RENDER_CONTEXT_METALCONTEXT_HPP

#include "Render/Context/I_Context.hpp"

#include "Define/Render.hpp"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#include <utility>

namespace CA {
class MetalLayer;
}
namespace MTL {
class CommandQueue;
class Device;
class RenderPassDescriptor;
}
namespace NS {
class Window;
}

namespace CE::Window {
class CocoaWindow;
}

namespace CE::Render::Context {

struct MetalContextProps {
	Window::CocoaWindow* window = nullptr;
	MTL::PixelFormat pixelFormat = MTL::PixelFormat::PixelFormatBGRA8Unorm;
};

/**
 * @class MetalContext
 * @brief macOS-specific graphics context implementation using Metal API
 * @details Provides a graphics context implementation for macOS that uses the Metal API for rendering.
 *			Manages Metal resources such as the device, command queue, and render layer. This class is designed
 *			for applications that want to use Metal on macOS without relying on GLFW, providing a more native experience.
 */
class MetalContext final: public I_Context {
public:
	/**
	 * @brief Constructor
	 * @param props Properties for initializing the Metal context, including window and pixel format
	 * @details Creates a MetalContext instance with the specified properties. The constructor does not perform initialization of Metal resources; the Init() method must be called separately to set up the context.
	 */
	MetalContext(const MetalContextProps& props);

	/**
	 * @brief Destructor
	 * @details Cleans up Metal resources. The destructor does not destroy the associated window, as window management is handled separately.
	 */
	~MetalContext() override;

public:
	/**
	 * @brief Initializes the Metal graphics context
	 * @details Sets up the Metal device, command queue, and render layer based on the provided properties. This method must be called after constructing the MetalContext instance to prepare it for rendering operations.
	 *			Initializes the Core Animation Metal layer and attaches it to the window's content view for rendering.
	 */
	void Init() override;
	/**
	 * @brief Swaps the front and back buffers to present the rendered frame
	 * @details Handles buffer swapping by presenting the current drawable from the Metal layer. This method should be called after rendering commands have been issued to present the rendered content on the screen.
	 */
	void SwapBuffers() override;

public:
	/**
	 * @brief Handles changes in content size (e.g., when the window is resized)
	 * @param size New content size as a pair of floats (width, height)
	 * @details Updates the Metal layer's drawable size to match the new content size. This method should be called whenever the window is resized to ensure that rendering remains correct at different sizes and that the viewport and related settings are updated accordingly.
	 */
	void HandleContentSizeChange(const std::pair<float, float>& size) override;
	/**
	 * @brief Handles changes in VSync state
	 * @param enabled True if VSync is enabled, false otherwise
	 * @details Updates the Metal layer's display sync setting based on the new VSync state. This method should be called when the VSync state changes to ensure that rendering is synchronized with the monitor's refresh rate if VSync is enabled.
	 */
	void HandleVSyncChange(bool enabled) override;

public:
	/**
	 * @brief Gets the native Metal device
	 * @return void* Pointer to the MTL::Device representing the Metal device (GPU)
	 * @details Returns a pointer to the underlying MTL::Device used for rendering. This allows the application to access platform-specific features or perform operations that require direct access to the Metal device.
	 */
	[[nodiscard]] void* GetNativeDevice() const override { return _device.get(); }

	/**
	 * @brief Gets the native Core Animation Metal layer
	 * @return void* Pointer to the CA::MetalLayer used for rendering
	 * @details Returns a pointer to the underlying CA::MetalLayer that serves as the rendering target. This allows the application to access platform-specific features or perform operations that require direct access to the Metal layer.
	 */
	[[nodiscard]] void* GetNativeLayer() const override { return _layer.get(); }

	/**
	 * @brief Gets the native Metal command queue
	 * @return void* Pointer to the MTL::CommandQueue used for issuing rendering commands
	 * @details Returns a pointer to the underlying MTL::CommandQueue used for issuing rendering commands. This allows the application to access platform-specific features or perform operations that require direct access to the Metal command queue.
	 */
	[[nodiscard]] void* GetNativeCommandQueue() const override { return _commandQueue.get(); }

	/**
	 * @brief Gets the native Metal render pass descriptor
	 * @return void* Pointer to the MTL::RenderPassDescriptor used for configuring render passes
	 * @details Returns a pointer to the underlying MTL::RenderPassDescriptor used for configuring render passes. This allows the application to access platform-specific features or perform operations that require direct access to the Metal render pass descriptor.
	 */
	[[nodiscard]] void* GetNativeRenderPassDescriptor() const override { return _renderPassDescriptor.get(); }

	/** @brief Gets the Metal device
	 * @return MTL::Device* Pointer to the MTL::Device representing the Metal device (GPU)
	 * @details Returns a pointer to the underlying MTL::Device used for rendering. This allows the application to access platform-specific features or perform operations that require direct access to the Metal device.
	 */
	[[nodiscard]] MTL::Device* GetMetalDevice() const { return _device.get(); }

	/** @brief Gets the Core Animation Metal layer
	 * @return CA::MetalLayer* Pointer to the CA::MetalLayer used for rendering
	 * @details Returns a pointer to the underlying CA::MetalLayer that serves as the rendering target. This allows the application to access platform-specific features or perform operations that require direct access to the Metal layer.
	 */
	[[nodiscard]] CA::MetalLayer* GetMetalLayer() const { return _layer.get(); }

	/** @brief Gets the Metal command queue
	 * @return MTL::CommandQueue* Pointer to the MTL::CommandQueue used for issuing rendering commands
	 * @details Returns a pointer to the underlying MTL::CommandQueue used for issuing rendering commands. This allows the application to access platform-specific features or perform operations that require direct access to the Metal command queue.
	 */
	[[nodiscard]] MTL::CommandQueue* GetMetalCommandQueue() const { return _commandQueue.get(); }

	/** @brief Gets the Metal render pass descriptor
	 * @return MTL::RenderPassDescriptor* Pointer to the MTL::RenderPassDescriptor used for configuring render passes
	 * @details Returns a pointer to the underlying MTL::RenderPassDescriptor used for configuring render passes. This allows the application to access platform-specific features or perform operations that require direct access to the Metal render pass descriptor.
	 */
	[[nodiscard]] MTL::RenderPassDescriptor* GetMetalRenderPassDescriptor() const { return _renderPassDescriptor.get(); }

RENDER_API_TYPE(Metal)

private:
	NS::SharedPtr<MTL::CommandQueue> _commandQueue = nullptr;	///< Metal command queue for issuing rendering commands
	NS::SharedPtr<MTL::Device> _device = nullptr;				///< Metal device (GPU) for resource creation and rendering
	NS::SharedPtr<CA::MetalLayer> _layer = nullptr;				///< Core Animation Metal layer for rendering
	NS::SharedPtr<MTL::RenderPassDescriptor> _renderPassDescriptor = nullptr;	///< Metal render pass descriptor for configuring render passes

	MetalContextProps _props;							///< Properties for initializing the Metal context, including window and pixel format
};

}

#endif //CE_RENDER_CONTEXT_METALCONTEXT_HPP
