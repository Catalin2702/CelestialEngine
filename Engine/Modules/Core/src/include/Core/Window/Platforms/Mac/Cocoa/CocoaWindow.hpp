//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac/Cocoa
// File: CocoaWindow.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-05-28
//

#pragma once

#ifndef CE_WINDOW_MAC_COCOAWINDOW_HPP
#define CE_WINDOW_MAC_COCOAWINDOW_HPP

#include "Core/Window/I_Window.hpp"
#include "Define/DynamicLinker.hpp"

#include "Apple/MetalCpp/AppKit/AppKit.hpp"
#include "Apple/MetalCpp/Foundation/Foundation.hpp"
#include "Apple/Types/Types.hpp"
#include "Define/Window.hpp"
#include "Types/Window/WindowProps.hpp"

#include <memory>
#include <utility>

namespace CA {
class MetalLayer;
}

namespace MTL {
class CommandQueue;
class Device;
}

namespace NS {
class RenderViewController;
class Window;
class WindowDelegate;
}

/**
 * @namespace CE::Core::Window
 * @brief Window management and window interfaces
 * @details Contains the I_Window interface and related type definitions for window management.
 *			This namespace defines the common interface for all window implementations,
 *			allowing for platform-specific implementations while maintaining a consistent API.
 *			The CocoaWindow class provides a macOS-specific implementation of the I_Window interface using Metal for rendering and Cocoa for window management,
 *			offering a native experience for applications targeting macOS with Metal.
 */
namespace CE::Core::Window {

struct EventHandlers {
	Apple::Types::ViewEventHandler ViewEventHandler{};
	Apple::Types::WindowDelegateEventHandler WindowDelegateEventHandler{};
	Apple::Types::ViewControllerEventHandler ViewControllerEventHandler{};
};

/**
 * @class CocoaWindow
 * @brief macOS-specific window implementation using Metal API and Cocoa
 * @details Provides a window implementation for macOS that uses Metal for rendering and Cocoa for window management.
 *			Manages a native macOS window and Metal resources for graphics.
 *			This class is designed for applications that want to use Metal on macOS without relying on GLFW, providing a more native experience.
 */
class CE_API CocoaWindow final: public I_Window {
public:
	/**
	 * @brief Constructor
	 * @details Creates and initializes a Cocoa window
	 */
	CocoaWindow() = default;

	/**
	 * @brief Destructor
	 * @details Cleans up Metal and Cocoa resources
	 */
	~CocoaWindow() override;

public:
	void OnUpdate() const override {}

	/**
	 * @brief Prepares the window for rendering
	 * @param VSync True to enable VSync, false to disable
	 * @details This method can be used to perform any necessary setup before the rendering loop starts.
	 *			For example, it can be used to ensure that the Metal layer is properly configured and ready for rendering.
	 */
	void GetReady(bool VSync) override;

public:
	/**
	 * @brief Gets the current size of the window
	 * @return std::pair<float, float> Pair of width and height in pixels
	 */
	[[nodiscard]] std::pair<float, float> GetWindowSize() const override;

	/**
	 * @brief Gets the current size of the frame
	 * @return std::pair<float, float> Pair of frame width and height in pixels
	 */
	[[nodiscard]] std::pair<float, float> GetFrameSize() const override;

	/**
	 * @brief Checks if VSync is enabled
	 * @return bool True if VSync is enabled, false otherwise
	 */
	[[nodiscard]] bool IsVSync() const override;

	/**
	 * @brief Gets the native macOS window
	 * @return void* Pointer to the native macOS window (NS::Window*)
	 * @details Provides access to the underlying macOS window for platform-specific operations. The returned pointer can be cast to NS::Window* for use with Cocoa APIs.
	 */
	[[nodiscard]] void* GetNativeWindow() const override { return _window.get(); }

public:
	/**
	 * @brief Sets the window size
	 * @param width New width in pixels
	 * @param height New height in pixels
	 * @details Convenience method to set both width and height at once. Updates the window's dimensions and resizes the Metal layer accordingly to ensure proper rendering.
	 */
	void SetSize(unsigned int width, unsigned int height) override;

	/**
	 * @brief Enables or disables VSync
	 * @param enabled True to enable VSync, false to disable
	 * @details VSync synchronizes rendering with the monitor's refresh rate to prevent screen tearing. This method updates the Metal layer's display sync setting accordingly.
	 */
	void SetVSync(bool enabled) override;

public:
	/**
	 * @brief Sets the event callback function
	 * @param callback Function to be called when events occur
	 * @details The callback will be invoked for all window and input events, allowing the application to respond to user interactions and window changes.
	 */
	void SetEventCallback(const EventCallbackFn& callback) override;

	/**
	 * @brief Sets the resize event callback function
	 * @param callback Function to be called when the window is resized
	 * @details The callback will be invoked with the new content scale when the window is resized, allowing the application to adjust rendering or UI layout based on the new size.
	 */
	void SetContentScaleCallback(const ContentSizeCallbackFn& callback) override;

	/**
	 * @brief Sets the VSync state change callback function
	 * @param callback Function to be called when the VSync state changes
	 * @details The callback will be invoked with the new VSync state when it is changed, allowing the application to respond to VSync changes.
	 */
	void SetVSyncCallback(const VSyncCallbackFn& callback) override;

public:
	/**
	 * @brief Gets the native macOS window
	 * @return NS::Window* Pointer to the AppKit window
	 * @details Provides access to the underlying macOS window for platform-specific operations. This allows for direct interaction with the Cocoa window when necessary.
	 */
	[[nodiscard]] NS::Window* GetWindow() const { return _window.get(); }

	/**
	 * @brief Gets the content view controller of the window
	 * @return NS::RenderViewController* Pointer to the view controller managing the content view
	 * @details Provides access to the view controller that manages the content view of the window.
	 *			This allows for direct interaction with the view controller when necessary, such as for handling input events or managing the view hierarchy.
	 */
	[[nodiscard]] NS::RenderViewController* GetViewController() const { return _viewController.get(); }

public:
	/**
	 * @brief Initializes the window with the provided Metal device
	 * @param device Pointer to the Metal device to be used for view controller initialization
	 * @details Initializes the window by first initializing the view controller with the provided Metal device and then setting up the Cocoa window and Metal layer.
	 *			This method is called by the CocoaApplication during its initialization process to prepare the window for rendering with Metal.
	 *			It ensures that the view controller is properly set up with the Metal device before initializing the window and its associated resources.
	 */
	void Init(const MTL::Device* device);

protected:
	/**
	 * @brief Sets internal callbacks for I/O events
	 * @details Registers the necessary callbacks for handling keyboard input, mouse input, and other I/O events using the Cocoa event system.
	 *			These callbacks will translate Cocoa events into the engine's event system and invoke the appropriate event callbacks stored in the _data structure.
	 */
	void _SetIOEventCallbacks() override;

	/**
	 * @brief Sets internal callbacks for window events
	 * @details Registers the necessary callbacks for handling window events such as resizing, closing, minimizing, etc.
	 *			These callbacks will translate Cocoa window events into the engine's event system and invoke the appropriate event callbacks stored in the _data structure.
	 */
	void _SetWindowEventCallbacks() override;

	void _SetViewControllerEventCallbacks();

	/**
	 * @brief Sets internal callbacks for window events
	 * @details Stores the provided callbacks in the _data structure, which will be
	 *			invoked by the GLFW event callbacks registered in _SetWindowCallbacks()
	 */
	void _SetInternalCallbacks() override {};

protected:
	void _Init() override {}

	/** @brief Initializes the Cocoa window and Metal layer
	 * @details Creates the native macOS window using Cocoa APIs and sets up the Core Animation Metal layer for rendering.
	 *			This includes configuring the layer's properties and attaching it to the window's content view.
	 */
	void _InitWindow() override;

	/**
	 * @brief Initializes the view controller with the Metal device
	 * @param device Pointer to the Metal device to be used for rendering
	 * @details Initializes the view controller by creating a new instance of NS::RenderViewController and setting it up with the provided Metal device. This prepares the view controller for managing the content view and handling rendering operations using Metal.
	 */
	void _InitViewController(const MTL::Device* device);

	/** @brief Shuts down the window and cleans up resources
	 * @details This method is called by the destructor to clean up Metal and Cocoa resources, including closing the window and releasing any allocated resources.
	 */
	void _Shutdown() override;

public:
	WINDOW_API_TYPE(Cocoa)

private:
	NS::SharedPtr<NS::RenderViewController> _viewController; ///< View controller for managing the content view
	NS::SharedPtr<NS::Window> _window; ///< Native macOS window
	NS::SharedPtr<NS::WindowDelegate> _windowDelegate; ///< Delegate for handling window events

	EventHandlers _handlers; ///< Struct containing event handlers for view and window events
};

}

#endif //CE_WINDOW_MAC_COCOAWINDOW_HPP
