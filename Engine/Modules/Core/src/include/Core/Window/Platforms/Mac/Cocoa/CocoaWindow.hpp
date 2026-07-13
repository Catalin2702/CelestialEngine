//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Mac/Cocoa
// File: CocoaWindow.hpp
// Created by: Catalin Chirosca
// Created: 2026-03-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_CORE_WINDOW_MAC_COCOAWINDOW_HPP
#define CE_CORE_WINDOW_MAC_COCOAWINDOW_HPP

#include "Define/DynamicLinker.hpp"
#include "Define/Window.hpp"

#include "Apple/MetalCpp/AppKit/WindowEventDispatcher.hpp"
#include "Core/Window/I_Window.hpp"
#include "Types/Window/WindowProps.hpp"

#include <AppKit/AppKit.hpp>
#include <Foundation/Foundation.hpp>

#include <memory>
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

class CocoaWindowEventHandler: public Native::WindowEventDispatcher {
	struct WindowStateEvents {
		VoidMulticastEventDispatcher cocoaWindowCreatedMulticastDispatcher;
		VoidMulticastEventDispatcher cocoaWindowInitializedMulticastDispatcher;
		VoidMulticastEventDispatcher cocoaWindowWillShutdownMulticastDispatcher;
	};

public:
	void DispatchCocoaWindowCreated();
	void DispatchCocoaWindowInitialized();
	void DispatchCocoaWindowWillShutdown();

public:
	WindowStateEvents windowStateEvents;
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
	CocoaWindow();

	/**
	 * @brief Destructor
	 * @details Cleans up Metal and Cocoa resources
	 */
	~CocoaWindow() override;

public:
	/**
	 * @brief Initializes the window with the provided Metal device
	 */
	void Init() override;

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

protected:
	/**
	 * @brief Initializes the Cocoa window and Metal layer
	 * @details Creates the native macOS window using Cocoa APIs and sets up the Core Animation Metal layer for rendering.
	 *			This includes configuring the layer's properties and attaching it to the window's content view.
	 */
	void _InitWindow() override;

	/**
	 * @brief Shuts down the window and cleans up resources
	 * @details This method is called by the destructor to clean up Metal and Cocoa resources, including closing the window and releasing any allocated resources.
	 */
	void _Shutdown() override;

public:
	WINDOW_API_TYPE(Cocoa)

public:
	std::unique_ptr<CocoaWindowEventHandler> cocoaWindowEventDispatcher; ///< Dispatch the NS::Window and CocoaWindow events

private:
	NS::SharedPtr<NS::Window> _window; ///< Native macOS window
};

}

#endif //CE_CORE_WINDOW_MAC_COCOAWINDOW_HPP
