//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#pragma once

#ifndef CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP
#define CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP

#include "Core/Application/I_Application.hpp"
#include "Define/DynamicLinker.hpp"

#include <Foundation/Foundation.hpp>

#include <chrono>
#include <memory>


namespace CA {
class DisplayLink;
}

namespace NS {
class Application;
}

namespace CE {
namespace Core::Application {
class CocoaApplicationDelegate;
}

namespace Layers {
class ImGuiMetalLayer;
}

namespace Render::Context {
class I_Context;
class MetalContext;
}

namespace Window {
class I_Window;
class CocoaWindow;
}

}

namespace CE::Core::Application {

/**
 * @class CocoaApplication
 * @brief macOS-specific application implementation using Cocoa
 * @details Provides a macOS-specific implementation of the I_Application interface using Cocoa for application management.
 *			Manages the application lifecycle and event handling for a native macOS application.
 */
class CE_API CocoaApplication: public I_Application {
public:
	/**
	 * @brief Default constructor
	 */
	CocoaApplication();

	/**
	 * @brief Copy constructor (deleted)
	 */
	CocoaApplication(const CocoaApplication& other) = delete;

	/**
	 * @brief Move constructor (deleted)
	 */
	CocoaApplication(CocoaApplication&& other) noexcept = delete;

	/**
	 * @brief Destructor
	 */
	~CocoaApplication() override;

public:
	/**
	 * @brief Copy assignment operator (deleted)
	 */
	CocoaApplication& operator=(const CocoaApplication& other) = delete;

	/**
	 * @brief Move assignment operator (deleted)
	 */
	CocoaApplication& operator=(CocoaApplication&& other) = delete;

public:
	/**
	 * @brief Runs the application
	 * @details Starts the AppKit Cocoa application loop
	 */
	void Run() override;

	/**
	 * @brief Quits the application
	 * @details Terminates the AppKit Cocoa application loop
	 */
	void Quit() override;

	/**
	 * @brief Updates the application state
	 * @param deltaTime Time elapsed since the last update in seconds
	 * @details Called every frame to update the application. Updates all layers in the layer stack.
	 */
	void Tick(float deltaTime) override;

	/**
	 * @brief Handles events
	 * @param event Reference to the event to be processed
	 * @details Dispatches events to the appropriate layers in the layer stack
	 */
	void OnEvent(Events::I_Event& event) override;

	/**
     * @brief Initializes the application with window properties
     * @param windowProps Window configuration properties
     * @details Initializes the application by creating the window and setting up the rendering context based on the provided window properties. This method should be called before running the application to ensure that all necessary components are properly initialized.
     */
	void Init(const Types::Window::WindowProps& windowProps) override;

	/**
	 * @brief Initializes the ImGui layer
	 * @details Initializes the ImGui layer for rendering UI based on the specified graphics API. For macOS with Metal, this will involve creating an ImGuiMetalLayer instance and pushing it as an overlay.
	 */
	void InitImGuiLayer(Types::Render::GraphicsApi) override;

	/**
	 * @brief Starts the display link for rendering
	 * @details Initializes and starts the display link, which synchronizes the rendering loop with the display's refresh rate. This ensures smooth rendering and optimal performance on macOS.
	 */
	void StartDisplayLink();

	/**
	 * @brief Stops the display link for rendering
	 * @details Stops and cleans up the display link, which is used to synchronize the rendering loop with the display's refresh rate. This should be called when the application is quitting or when rendering is no longer needed to free up system resources.
	 */
	void StopDisplayLink();

public:
	/**
	 * @brief Sets the ImGui layer for the application
	 * @param imguiLayer Pointer to the ImGui layer to set
	 * @details Sets the ImGui layer for rendering UI. This method allows you to specify the ImGui layer that will be used for rendering the user interface.
	 *			It must be an ImGuiMetalLayer or derived class that is compatible with the Metal rendering context used by the application.
	 */
	void SetImGuiLayer(Layers::I_Layer* imguiLayer) override;

	/**
	 * @brief Removes the ImGui layer from the application
	 * @details Removes the currently set ImGui layer from the application and pops it from the layer stack. This will stop rendering the ImGui UI and free up resources associated with the ImGui layer.
	 */
	void RemoveImGuiLayer() override;

protected:
	/**
     * @brief Initializes the application window
     * @param windowProps Window configuration properties
     * @details Creates the application window and sets up event callbacks based on the provided window properties.
     *			Initializes the appropriate input system based on the window API.
     */
	void _InitWindow(const Types::Window::WindowProps& windowProps) override;

	/**
     * @brief Initializes the renderer
     * @details Initializes the rendering context based on the specified graphics API. For macOS, this will typically involve setting up a Metal rendering context.
     */
	void _InitRenderer(Types::Render::GraphicsApi) override;

public:
	/**
     * @brief Gets the application's window
     * @return Window::I_Window& Reference to the window
     * @details Provides access to the window for rendering operations. This method returns a reference to the window instance managed by the CocoaApplication, allowing other components to interact with the window as needed.
     */
	[[nodiscard]] Window::I_Window& GetWindow() const override;

	/**
     * @brief Gets the rendering context
     * @return Render::Context::I_Context& Reference to the rendering context
     * @details Provides access to the rendering context for rendering operations. This method returns a reference to the MetalContext instance managed by the CocoaApplication, allowing other components to perform rendering using the Metal API.
     */
	[[nodiscard]] Render::Context::I_Context& GetRenderContext() const override;

private:
	static void _StDisplayLinkCallback(void* userData);
	void _SetWindowCallbacks() const;

private:
	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;

	NS::SharedPtr<NS::Application> _appCocoa; ///< Pointer to the Cocoa application instance
	NS::SharedPtr<CA::DisplayLink> _displayLink; ///< Pointer to the display link for synchronizing rendering
	std::unique_ptr<CocoaApplicationDelegate> _appDelegate; ///< Delegate for handling Cocoa application events

	std::unique_ptr<Render::Context::MetalContext> _context; ///< Pointer to the Metal rendering context
	std::unique_ptr<Window::CocoaWindow> _window; ///< Pointer to the application window

	Layers::ImGuiMetalLayer* _imguiLayer; ///< Pointer to the ImGui layer for rendering UI

	TimePoint _lastFrameTime; ///< Timestamp of the last frame for delta time calculation
};

}

#endif //CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP
