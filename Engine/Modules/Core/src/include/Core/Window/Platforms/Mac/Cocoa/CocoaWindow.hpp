//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Mac/Cocoa
// File: CocoaWindow.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-09-05
//

#pragma once

#ifndef CE_CORE_WINDOW_MAC_COCOAWINDOW_HPP
#define CE_CORE_WINDOW_MAC_COCOAWINDOW_HPP

#include "Define/DynamicLinker.hpp"

#include "Apple/MetalCpp/AppKit/NsWindowEventDispatcher.hpp"
#include "Core/Render/Surface/Mac/Metal/I_MetalSurface.hpp"
#include "Core/Window/I_Window.hpp"
#include "Types/Window/WindowProps.hpp"

#include <AppKit/AppKit.hpp>
#include <Foundation/Foundation.hpp>

#include <string_view>
#include <utility>


namespace CA {
	class MetalLayer;
}

namespace MTK {
	class View;
}

namespace MTL {
	class CommandQueue;
	class Device;
}

namespace NS {
	class Notification;
	class Screen;
	class Window;
}

namespace CE::Core {

class CocoaWindowEventHandler: public Native::NsWindowEventDispatcher {
	struct CocoaWindowLifecycleEvents {
		UnicastDispatcher<> cocoaWindowCreatedDispatcher;
		UnicastDispatcher<> cocoaWindowInitializedDispatcher;
		UnicastDispatcher<> cocoaWindowWillShutdownDispatcher;
	};

public:
	/**
	 * @brief Forwards the cocoa window created callback to the bound listener
	 */
	void DispatchCocoaWindowCreated() const;
	/**
	 * @brief Forwards the cocoa window initialized callback to the bound listener
	 */
	void DispatchCocoaWindowInitialized() const;
	/**
	 * @brief Forwards the cocoa window will shutdown callback to the bound listener
	 */
	void DispatchCocoaWindowWillShutdown() const;

public:
	CocoaWindowLifecycleEvents cocoaWindowStateEvents;
};

/**
 * @class CocoaWindow
 * @brief macOS-specific window implementation using Metal API and Cocoa
 * @details Provides a window implementation for macOS that uses Metal for rendering and Cocoa for window management.
 *			Manages a native macOS window and Metal resources for graphics.
 *			This class is designed for applications that want to use Metal on macOS without relying on GLFW, providing a more native experience.
 */
class CE_CORE_API CocoaWindow final: public I_WindowBase<Types::WindowApi::Cocoa>, public I_MetalSurface {
public:
	/**
	 * @brief Constructor
	 * @details Creates and initializes a Cocoa window
	 */
	CocoaWindow();

	/**
	 * @brief Destructor
	 * @details Cleans up Metal and Cocoa resources
	 */
	~CocoaWindow() override;

public:
	/**
	 * @brief Creates the native window
	 * @details Not part of I_Window: the Cocoa window is brought up from applicationDidFinishLaunching rather than
	 *			from the constructor, because AppKit will not give a usable window before the run loop is up.
	 */
	void Init();

	/**
	 * @brief Makes the window key and brings it to the front
	 * @details Deferred until the application has finished launching (see NsApplicationDelegate::applicationDidFinishLaunching)
	 *			so the window is revealed once the run loop is active, following the canonical Cocoa lifecycle. Also
	 *			restores the native fullscreen state saved on the previous shutdown.
	 */
	void Show() override;

	/**
	 * @brief Routes the NS::Window lifecycle and focus notifications into the hub
	 */
	void ConnectToEventHub(I_EventHubDispatcher& eventHub) override;

public:
	/**
	 * @brief Minimizes the window to the dock
	 */
	void Miniaturize() override;

	/**
	 * @brief Restores the window from the dock
	 */
	void Deminiaturize() override;

	/**
	 * @brief Toggles the native macOS fullscreen mode
	 */
	void ToggleFullScreen() override;

public:
	/**
	 * @brief Gets the current size of the window
	 * @return std::pair<f32, f32> Pair of width and height in pixels
	 */
	[[nodiscard]] std::pair<u32, u32> GetWindowSize() const override;

	/**
	 * @brief Gets the current size of the frame
	 * @return std::pair<u32, u32> Pair of frame width and height in backing pixels
	 */
	[[nodiscard]] std::pair<u32, u32> GetFrameSize() const override;

	/**
	 * @brief Gets the ratio between backing pixels and screen coordinates
	 */
	[[nodiscard]] f32 GetContentScale() const override;

	/**
	 * @brief Gets the refresh rate of the screen the window is on
	 */
	[[nodiscard]] u32 GetRefreshRate() const override;

	/**
	 * @brief Gets the native macOS window
	 * @return void* Pointer to the native macOS window (NS::Window*)
	 * @details Provides access to the underlying macOS window for platform-specific operations. The returned pointer can be cast to NS::Window* for use with Cocoa APIs.
	 */
	[[nodiscard]] void* GetNativeWindow() const override { return _window.get(); }

	/**
	 * @brief Gets the Core Animation layer the window is presented through
	 * @return CA::MetalLayer* The MetalKit view's layer, or null before the content view has been installed
	 * @details The surface *is* whatever the window is currently showing: the content view is the MTK::View the render
	 *			context created, and a MetalKit view is always backed by a CAMetalLayer. That is what makes this a
	 *			one-line implementation and why nothing has to be stored - the window already knows its content view,
	 *			and asking it every time cannot go stale when the view is replaced.
	 */
	[[nodiscard]] CA::MetalLayer* GetMetalLayer() const override;

public:
	/**
	 * @brief Sets the window size
	 * @param width New width in pixels
	 * @param height New height in pixels
	 * @details Convenience method to set both width and height at once. Updates the window's dimensions and resizes the Metal layer accordingly to ensure proper rendering.
	 */
	void SetWindowSize(u32 width, u32 height) override;

	/**
	 * @brief Sets the text shown in the window's title bar
	 */
	void SetTitle(std::string_view title) override;

public:
	/**
	 * @brief Gets the native macOS window
	 * @return NS::Window* Pointer to the AppKit window
	 * @details Provides access to the underlying macOS window for platform-specific operations. This allows for direct interaction with the Cocoa window when necessary.
	 */
	[[nodiscard]] NS::Window* GetWindow() const { return _window.get(); }

public:
	/**
	 * @brief Installs the MetalKit view as the window's content view
	 * @param view MetalKit view (owned by the render context) to display and route input through
	 * @details Attaches @p view as the window's content view, makes it the first responder so it receives keyboard
	 *			events, and enables mouse-moved delivery so the view's dispatcher gets continuous cursor updates.
	 */
	void SetContentView(const MTK::View* view) const;

private:
	/**
	 * @brief Initializes the Cocoa window and Metal layer
	 * @details Creates the native macOS window using Cocoa APIs and sets up the Core Animation Metal layer for rendering.
	 *			This includes configuring the layer's properties and attaching it to the window's content view.
	 */
	void _InitWindow();

	/**
	 * @brief Shuts down the window and cleans up resources
	 * @details This method is called by the destructor to clean up Metal and Cocoa resources, including closing the window and releasing any allocated resources.
	 */
	void _Shutdown();

public:
	CocoaWindowEventHandler cocoaWindowEventDispatcher; ///< Dispatch the NS::Window and CocoaWindow events

private:
	NS::SharedPtr<NS::Window> _window; ///< Native macOS window
};

}

#endif //CE_CORE_WINDOW_MAC_COCOAWINDOW_HPP
