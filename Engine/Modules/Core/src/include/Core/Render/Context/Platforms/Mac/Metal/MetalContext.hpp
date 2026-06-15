//
// Module: CelestialEngine/Engine/Modules/Render/Context
// File: MetalContext.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-06-15
//

#pragma once

#ifndef CE_RENDER_CONTEXT_METALCONTEXT_HPP
#define CE_RENDER_CONTEXT_METALCONTEXT_HPP

#include "Core/Render/Context/I_Context.hpp"

#include "Define/DynamicLinker.hpp"
#include "Define/Render.hpp"

#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"

#include "Apple/MetalCpp/Foundation/Foundation.hpp"
#include "Apple/MetalCpp/Metal/Metal.hpp"
#include "Apple/MetalCpp/MetalKit/MetalKit.hpp"

#include <memory>
#include <utility>


namespace MTL {
	class CommandQueue;
	class Device;
	class RenderPassDescriptor;
	class RenderPipelineState;
}

namespace CE::Core::Window {
	class CocoaWindow;
}

namespace CE::Core::Render::Context {

struct CE_API MetalContextProps {
	MTL::PixelFormat pixelFormat = MTL::PixelFormat::PixelFormatBGRA8Unorm;
};

/**
 * @class MetalContext
 * @brief macOS-specific graphics context implementation using Metal API
 * @details Provides a graphics context implementation for macOS that uses the Metal API for rendering.
 *			Manages Metal resources such as the device, command queue, and render layer. This class is designed
 *			for applications that want to use Metal on macOS without relying on GLFW, providing a more native experience.
 */
class CE_API MetalContext final: public I_Context {
public:
	/**
	 * @brief Initializes the Metal graphics context
	 * @details Sets up the Metal device, command queue, and render layer based on the provided properties. This method must be called after constructing the MetalContext instance to prepare it for rendering operations.
	 *			Initializes the Core Animation Metal layer and attaches it to the window's content view for rendering.
	 */
	void Init() override;

public:
	/**
	 * @brief Handles changes in content size (e.g., when the window is resized)
	 * @param size New content size as a pair of floats (width, height)
	 * @details Updates the Metal layer's drawable size to match the new content size. This method should be called whenever the window is resized to ensure that rendering remains correct at different sizes and that the viewport and related settings are updated accordingly.
	 */
	void HandleContentSizeChange(const std::pair<float, float>& size) const;

	/**
	 * @brief Handles changes in VSync state
	 * @param enabled True if VSync is enabled, false otherwise
	 * @details Updates the Metal layer's display sync setting based on the new VSync state. This method should be called when the VSync state changes to ensure that rendering is synchronized with the monitor's refresh rate if VSync is enabled.
	 */
	void HandleVSyncChange(bool enabled) const;

public:
	/**
	 * @brief Checks if VSync is currently enabled
	 * @return bool True if VSync is enabled, false otherwise
	 * @details Returns the current state of VSync in the Metal context by checking the display sync setting of the Metal layer. This allows the application to query whether VSync is active and adjust rendering behavior accordingly.
	 */
	[[nodiscard]] bool IsVSyncEnabled() const override;

public:
	/** @brief Gets the Metal command queue
	 * @return MTL::CommandQueue* Pointer to the MTL::CommandQueue used for issuing rendering commands
	 * @details Returns a pointer to the underlying MTL::CommandQueue used for issuing rendering commands. This allows the application to access platform-specific features or perform operations that require direct access to the Metal command queue.
	 */
	[[nodiscard]] MTL::CommandQueue* GetCommandQueue() const { return _commandQueue.get(); }

	/** @brief Gets the Metal device
	 * @return MTL::Device* Pointer to the MTL::Device representing the Metal device (GPU)
	 * @details Returns a pointer to the underlying MTL::Device used for rendering. This allows the application to access platform-specific features or perform operations that require direct access to the Metal device.
	 */
	[[nodiscard]] MTL::Device* GetDevice() const { return _device.get(); }

	/** @brief Gets the shader library
	 * @return const Shader::MetalShaderLibrary& Reference to the MetalShaderLibrary used for managing shaders
	 * @details Returns a reference to the MetalShaderLibrary used for managing shaders in the Metal context. This allows the application to access platform-specific shader management features or perform operations that require direct access to the shader library.
	 */
	[[nodiscard]] Shader::MetalShaderLibrary* GetShaderLibrary() const { return _shaderLibrary.get(); }

	/** @brief Gets the MetalKit render view
	 * @return MTK::RenderView* Pointer to the MTK::RenderView used for rendering
	 * @details Returns a pointer to the underlying MTK::RenderView used for rendering. This allows the application to access platform-specific features or perform operations that require direct access to the MetalKit render view.
	 */
	[[nodiscard]] MTK::RenderView* GetView() const { return _view.get(); }

public:
	void SetView(MTK::RenderView* view);

	RENDER_API_TYPE(Metal)

public:
	MetalContextProps props; ///< Properties for initializing the Metal context

private:
	NS::SharedPtr<MTL::CommandQueue> _commandQueue = nullptr;	///< Metal command queue for issuing rendering commands
	NS::SharedPtr<MTL::Device> _device = nullptr;				///< Metal device (GPU) for resource creation and rendering
	NS::SharedPtr<MTK::RenderView> _view = nullptr;					///< MetalKit view used for rendering

private:
	std::unique_ptr<Shader::MetalShaderLibrary> _shaderLibrary; ///< Shader library for managing Metal shaders
};

}

#endif //CE_RENDER_CONTEXT_METALCONTEXT_HPP
