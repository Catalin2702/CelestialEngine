//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Mac
// File: CocoaWindow.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
//

#pragma once

#ifndef CE_WINDOW_MAC_COCOAWINDOW_HPP
#define CE_WINDOW_MAC_COCOAWINDOW_HPP

#include "Window/I_Window.hpp"

#include "Define/Window.hpp"
#include "Types/Window/WindowProps.hpp"

#include <utility>
#include <AppKit/AppKit.hpp>
#include <Foundation/Foundation.hpp>

namespace CA {
class MetalLayer;
}
namespace MTL {
class CommandQueue;
class Device;
}
namespace NS {
class RenderView;
class View;
class Window;
}


namespace TypeWindow = CE::Types::Window;

/**
 * @namespace CE::Window
 * @brief Window management and window interfaces
 * @details Contains the I_Window interface and related type definitions for window management.
 *			This namespace defines the common interface for all window implementations,
 *			allowing for platform-specific implementations while maintaining a consistent API.
 *			The CocoaWindow class provides a macOS-specific implementation of the I_Window interface using Metal for rendering and Cocoa for window management,
 *			offering a native experience for applications targeting macOS with Metal.
 */
namespace CE::Window {

/**
 * @class CocoaWindow
 * @brief macOS-specific window implementation using Metal API and Cocoa
 * @details Provides a window implementation for macOS that uses Metal for rendering and Cocoa for window management.
 *			Manages a native macOS window and Metal resources for graphics.
 *			This class is designed for applications that want to use Metal on macOS without relying on GLFW, providing a more native experience.
 */
class CocoaWindow final: public I_Window {
public:
	/**
	 * @brief Constructor
	 * @param windowProps Window configuration properties (title, dimensions, VSync)
	 * @details Creates and initializes a Metal-based window with the specified properties using Cocoa APIs
	 */
	CocoaWindow(const TypeWindow::WindowProps& windowProps);

	/**
	 * @brief Destructor
	 * @details Cleans up Metal and Cocoa resources
	 */
	~CocoaWindow() override;

public:
	void OnUpdate() const override {}

public:
	/**
	 * @brief Gets the current width of the window
	 * @return unsigned int Current width in pixels
	 */
	[[nodiscard]] unsigned int GetWidth() const override { return _data.width; }

	/**
	 * @brief Gets the current height of the window
	 * @return unsigned int Current height in pixels
	 */
	[[nodiscard]] unsigned int GetHeight() const override { return _data.height; }

	/**
	 * @brief Gets the current size of the window
	 * @return std::pair<unsigned int, unsigned int> Pair of width and height in pixels
	 */
	[[nodiscard]] std::pair<unsigned int, unsigned int> GetSize() const override { return {_data.width, _data.height}; }

	/**
	 * @brief Checks if VSync is enabled
	 * @return bool True if VSync is enabled, false otherwise
	 */
	[[nodiscard]] bool IsVSync() const override { return _data.VSync; }

	/**
	 * @brief Gets the native macOS window
	 * @return void* Pointer to the native macOS window (NS::Window*)
	 * @details Provides access to the underlying macOS window for platform-specific operations. The returned pointer can be cast to NS::Window* for use with Cocoa APIs.
	 */
	[[nodiscard]] void* GetNativeWindow() const override { return GetCocoaWindow(); }

	[[nodiscard]] std::pair<float, float> GetContentScale() const override;

	/**
	 * @brief Gets the native macOS window
	 * @return NS::Window* Pointer to the AppKit window
	 * @details Provides access to the underlying macOS window for platform-specific operations. This allows for direct interaction with the Cocoa window when necessary.
	 */
	[[nodiscard]] NS::Window* GetCocoaWindow() const { return _window.get(); }

	/**
	 * @brief Gets the content view of the macOS window
	 * @return NS::View* Pointer to the NSView (content view)
	 * @details Provides access to the content view of the window, which is needed for ImGui OSX backend initialization. The content view is where the Metal layer is attached for rendering.
	 */
	[[nodiscard]] NS::RenderView* GetCocoaView() const { return _view.get(); }

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
	void SetContentScaleCallback(const ContentScaleCallbackFn& callback) override;

	/**
	 * @brief Sets the window width
	 * @param width New width in pixels
	 * @details Updates the window's width and resizes the Metal layer accordingly to ensure proper rendering.
	 */
	void SetWidth(unsigned int width) override;

	/**
	 * @brief Sets the window height
	 * @param height New height in pixels
	 * @details Updates the window's height and resizes the Metal layer accordingly to ensure proper rendering.
	 */
	void SetHeight(unsigned int height) override;

	/**
	 * @brief Enables or disables VSync
	 * @param enabled True to enable VSync, false to disable
	 * @details VSync synchronizes rendering with the monitor's refresh rate to prevent screen tearing. This method updates the Metal layer's display sync setting accordingly.
	 */
	void SetVSync(bool enabled) override;

	WINDOW_API_TYPE(Cocoa)

protected:
	/**
	 * @brief Configures all window event callbacks
	 * @details Sets up Cocoa callbacks for window events such as resize, close, focus, minimize, etc. This allows the application to handle these events appropriately.
	 */
	void _SetWindowCallbacks() override;

	/**
	 * @brief Sets internal callbacks for window events
	 * @details Stores the provided callbacks in the _data structure, which will be
	 *			invoked by the GLFW event callbacks registered in _SetWindowCallbacks()
	 */
	void _SetInternalCallbacks() override;

private:
	/** @brief Initializes the window and Metal resources
	 * @details This method is called by the constructor to set up the Metal device, create the window, and configure the Metal layer for rendering.
	 */
	void _Init();

	/** @brief Initializes the Cocoa window and Metal layer
	 * @details Creates the native macOS window using Cocoa APIs and sets up the Core Animation Metal layer for rendering. This includes configuring the layer's properties and attaching it to the window's content view.
	 */
	void _InitWindow();

	/** @brief Shuts down the window and cleans up resources
	 * @details This method is called by the destructor to clean up Metal and Cocoa resources, including closing the window and releasing any allocated resources.
	 */
	void _Shutdown();

	/** @brief Updates the size of the Metal layer to match the window dimensions
	 * @details This method is called whenever the window is resized to ensure that the Metal layer's drawable area matches the new window size, allowing for correct rendering.
	 */
	void _UpdateLayerSize() const;

private:
	Types::Window::WindowProps _data;							///< Window data including event callback

	NS::SharedPtr<NS::RenderView> _view;					///< Content view of the window
	NS::SharedPtr<NS::Window> _window;				///< Native macOS window
};

}

#endif //CE_WINDOW_MAC_COCOAWINDOW_HPP
