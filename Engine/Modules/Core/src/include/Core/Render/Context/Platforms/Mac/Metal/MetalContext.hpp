//
// Module: CelestialEngine/Engine/Modules/Core/Render/Context/Platforms/Mac/Metal
// File: MetalContext.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-19
// Updated by: Catalin Chirosca
// Updated: 2026-08-29
//

#pragma once

#ifndef CE_CORE_RENDER_CONTEXT_METALCONTEXT_HPP
#define CE_CORE_RENDER_CONTEXT_METALCONTEXT_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Render.hpp"

#include "Core/Render/Context/I_Context.hpp"

#include "Apple/MetalCpp/AppKit/NsViewEventDispatcher.hpp"
#include "Apple/MetalCpp/MetalKit/MtkViewDelegate.hpp"
#include "Apple/MetalCpp/QuartzCore/CaMetalDisplayLinkDelegate.hpp"
#include "Core/Render/Shader/Platforms/Mac/Metal/MetalShaderLibrary.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <MetalKit/MetalKit.hpp>

#include <utility>


namespace MTL {
	class CommandQueue;
	class Device;
	class RenderPassDescriptor;
	class RenderPipelineState;
}

namespace CA {
	class MetalDrawable;
	class MetalDisplayLink;
}

namespace CE::Core {

	class CocoaWindow;

struct CE_CORE_API MetalContextProps {
	MTL::PixelFormat pixelFormat = MTL::PixelFormat::PixelFormatBGRA8Unorm;
};

class MetalContextEventDispatcher: public Native::NsViewEventDispatcher {
	struct MetalContextLifeCycleEvents {
		UnicastDispatcher<> onCreatedDispatcher;
		UnicastDispatcher<> onInitializedDispatcher;
		UnicastDispatcher<> onWillShutdownDispatcher;
		UnicastDispatcher<bool> onVSyncChangedDispatcher;
		UnicastDispatcher<f64, f64> onResizeDispatcher;
	};
public:
	/**
	 * @brief Forwards the metal context created callback to the bound listener
	 */
	void DispatchMetalContextCreated() const;
	/**
	 * @brief Forwards the metal context initialized callback to the bound listener
	 */
	void DispatchMetalContextInitialized() const;
	/**
	 * @brief Forwards the metal context will shutdown callback to the bound listener
	 */
	void DispatchMetalContextWillShutdown() const;
	/**
	 * @brief Forwards the v sync changed callback to the bound listener
	 */
	void DispatchVSyncChanged(bool vsync) const;
	/**
	 * @brief Forwards the resize callback to the bound listener
	 */
	void DispatchResizeEvent(f64 width, f64 height) const;

public:
	MetalContextLifeCycleEvents metalContextLifeCycleEvents;
};

/**
 * @class MetalContext
 * @brief macOS-specific graphics context implementation using Metal API
 * @details Provides a graphics context implementation for macOS that uses the Metal API for rendering.
 *			Manages Metal resources such as the device, command queue, and render layer. This class is designed
 *			for applications that want to use Metal on macOS without relying on GLFW, providing a more native experience.
 */
class CE_CORE_API MetalContext final: public I_Context {
public:
	/**
	 * @brief Default constructor
	 */
	MetalContext();

	/**
	 * @brief Destructor
	 * @details Tears down the display link (removed from the run loop and invalidated) before the view and layer are released.
	 */
	~MetalContext() override;

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
	 * @param size New content size as a pair of f32s (width, height)
	 * @details Updates the Metal layer's drawable size to match the new content size. This method should be called whenever the window is resized to ensure that rendering remains correct at different sizes and that the viewport and related settings are updated accordingly.
	 */
	void HandleContentSizeChange(const std::pair<f32, f32>& size) const;

	/**
	 * @brief Changes VSync state
	 * @param enabled
	 * @details Updates the Metal layer's display-sync setting and, in lockstep, creates or tears down the CAMetalDisplayLink:
	 *			it must exist while VSync is on (it paces frames) and must not exist while VSync is off (Core Animation forbids
	 *			CAMetalLayer::nextDrawable(), used by the VSync-off tick loop, once a display link exists for the layer). This
	 *			makes the state safe to swap at runtime. Fires the VSync-changed dispatcher afterwards.
	 */
	void SetVSync(bool enabled) override;

public:
	/**
	 * @brief Checks whether vertical synchronization is currently enabled
	 */
	[[nodiscard]] bool IsVSyncEnabled() const override;

	/**
	 * @brief Gets the content scale (drawable pixels per point, e.g. 2.0 on Retina)
	 */
	[[nodiscard]] std::pair<f32, f32> GetContentScale() const override;

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
	[[nodiscard]] const MetalShaderLibrary& GetShaderLibrary() const { return _shaderLibrary; }

	/** @brief Gets the MetalKit render view
	 * @return MTK::RenderView* Pointer to the MTK::RenderView used for rendering
	 * @details Returns a pointer to the underlying MTK::RenderView used for rendering. This allows the application to access platform-specific features or perform operations that require direct access to the MetalKit render view.
	 */
	[[nodiscard]] MTK::View* GetView() const { return _view.get(); }

	RENDER_API_TYPE(Metal)

public:
	/**
	 * @brief Sets the per-frame delegate driven by the CAMetalDisplayLink
	 * @param delegate Invoked once per display-link update (on the main run loop), while the display-link-vended drawable
	 *		  is available through AcquireDrawable(). This is the render pace when the display link is running.
	 */
	void SetDrawDelegate(const EventDelegate<MTK::View*>& delegate);

public:
	/**
	 * @brief Acquires the drawable to render the current frame into
	 * @return CA::MetalDrawable* The drawable vended by the CAMetalDisplayLink for the current update, or a freshly
	 *		   dequeued drawable from the layer when no display-link update is in flight (e.g. the VSync-off tick loop).
	 * @details Must be called from within a frame (between the display link update start and its end, or from the tick
	 *			loop). The returned drawable is owned by the display-link update / autorelease pool; do not retain it.
	 */
	[[nodiscard]] CA::MetalDrawable* AcquireDrawable() const;

	/**
	 * @brief Starts or pauses the display link that drives frames
	 * @param paused True to pause (no frames delivered), false to resume delivering per-frame updates
	 */
	void SetDisplayLinkPaused(bool paused) const;

private:
	/**
	 * @brief Display link callback: renders one frame while VSync pacing is active
	 */
	void _OnMetalLinkNeedsUpdate(CA::MetalDisplayLink*, CA::MetalDisplayLinkUpdate* update);

	/**
	 * @brief Handles the MetalKit view's drawable-size change
	 * @details Bound to the view delegate in _CreateView. Keeps the drawable in lockstep with the view (the view is paused
	 *			and driven by the display link, so MetalKit does not auto-resize it) and fires the context's resize dispatcher
	 *			(backing pixels), which the application routes to the event hub.
	 */
	void _OnDrawableResize(MTK::View*, CGSize size) const;

private:
	/**
	 * @brief Creates and configures the MetalKit view used for rendering and input
	 * @details Allocates the MTK::View with the Metal device, sets the pixel format/clear color, attaches the view
	 *			delegate and the input event dispatcher, and starts it paused so the application controls the frame pace.
	 */
	void _CreateView();

	/**
	 * @brief Creates the CAMetalDisplayLink that paces rendering and wires its delegate
	 * @details Builds the display link from the view's CAMetalLayer, installs the delegate whose per-frame callback
	 *			publishes the vended drawable and invokes the draw callback, then adds it to the main run loop paused.
	 */
	void _CreateDisplayLink();

	/**
	 * @brief Pauses, removes from the run loop, invalidates and releases the CAMetalDisplayLink (and its delegate)
	 * @details No-op when the display link does not exist. After this the layer is display-link-free, so
	 *			CAMetalLayer::nextDrawable() (the VSync-off frame path) is allowed again.
	 */
	void _DestroyDisplayLink();

public:
	MetalContextProps props; ///< Properties for initializing the Metal context

	MetalContextEventDispatcher metalContextEventDispatcher; ///< Dispatches the MTK::View and MetalContext events

private:
	MetalShaderLibrary _shaderLibrary; ///< Shader library for managing Metal shaders

	NS::SharedPtr<MTL::CommandQueue> _commandQueue = nullptr;	///< Metal command queue for issuing rendering commands
	NS::SharedPtr<MTL::Device> _device = nullptr;				///< Metal device (GPU) for resource creation and rendering
	NS::SharedPtr<MTK::View> _view = nullptr;					///< MetalKit view used for rendering
	Native::MtkViewDelegate _viewDelegate; ///< Drives the MTK::View (draw / drawable-size-change)

	NS::SharedPtr<CA::MetalDisplayLink> _displayLink = nullptr; ///< Paces frames in step with the display refresh
	Native::CaMetalDisplayLinkDelegate _displayLinkDelegate; ///< Receives the display link's per-frame updates

	CA::MetalDrawable* _displayLinkDrawable = nullptr; ///< Drawable vended by the current display-link update; valid only for the duration of the frame callback
	CallbackDispatcher<void, MTK::View*> _drawDispatcher; ///< Invoked once per display-link update to render a frame
};

}

#endif //CE_CORE_RENDER_CONTEXT_METALCONTEXT_HPP
