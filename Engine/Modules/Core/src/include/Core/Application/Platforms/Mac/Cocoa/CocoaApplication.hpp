//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP
#define CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP

#include "Core/Application/I_Application.hpp"

#include "Apple/MetalCpp/AppKit/NsApplicationDelegate.hpp"
#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Define/DynamicLinker.hpp"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#include <atomic>
#include <memory>
#include <thread>


namespace CA {
	class DisplayLink;
}

namespace NS {
	class Application;
	class Event;
	class Notification;
}

namespace CE::Core {
	class I_Layer;
	class ImGuiMetalLayer;
}

namespace CE::Core {

/**
 * @class CocoaApplication
 * @brief macOS-specific application implementation using Cocoa
 * @details Provides a macOS-specific implementation of the I_Application interface using Cocoa for application management.
 *			Manages the application lifecycle and event handling for a native macOS application.
 */
class CE_API CocoaApplication: public I_Application {
	friend class CocoaApplicationDelegate;

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
	void OnEvent(Events::I_Event& event) override {}

	void DispatchEventToLayers(Events::I_Event& event);

	/**
     * @brief Initializes the application with window properties
     * @details Initializes the application by creating the window and setting up the rendering context based on the provided window properties. This method should be called before running the application to ensure that all necessary components are properly initialized.
     */
	void Init() override;

	/**
	 * @brief Initializes the ImGui layer
	 * @details Initializes the ImGui layer for rendering UI based on the specified graphics API. For macOS with Metal, this will involve creating an ImGuiMetalLayer instance and pushing it as an overlay.
	 */
	void InitImGuiLayer() override;

public:
	/**
	 * @brief Sets the ImGui layer for the application
	 * @param imguiLayer Pointer to the ImGui layer to set
	 * @details Sets the ImGui layer for rendering UI. This method allows you to specify the ImGui layer that will be used for rendering the user interface.
	 *			It must be an ImGuiMetalLayer or derived class that is compatible with the Metal rendering context used by the application.
	 */
	void SetImGuiLayer(I_Layer* imguiLayer) override;

	/**
	 * @brief Removes the ImGui layer from the application
	 * @details Removes the currently set ImGui layer from the application and pops it from the layer stack. This will stop rendering the ImGui UI and free up resources associated with the ImGui layer.
	 */
	void RemoveImGuiLayer() override;

	void SetRunning(bool running) override;

protected:
	void _InitWindow() override {}

	void _InitRenderer() override {}

public:
	/**
     * @brief Gets the application's window
     * @return Window::I_Window& Reference to the window
     * @details Provides access to the window for rendering operations. This method returns a reference to the window instance managed by the CocoaApplication, allowing other components to interact with the window as needed.
     */
	[[nodiscard]] I_Window& GetWindow() const override { return *_window; }

	/**
	 * @brief Gets the Cocoa-specific window
	 * @return Window::CocoaWindow& Reference to the CocoaWindow
	 * @details Provides access to the Cocoa-specific window for platform-specific operations. This method returns a reference to the CocoaWindow instance managed by the CocoaApplication, allowing other components to perform operations that are specific to the macOS platform.
	 */
	[[nodiscard]] CocoaWindow& GetCocoaWindow() const { return *_window; }

	/**
     * @brief Gets the rendering context
     * @return Render::Context::I_Context& Reference to the rendering context
     * @details Provides access to the rendering context for rendering operations. This method returns a reference to the MetalContext instance managed by the CocoaApplication, allowing other components to perform rendering using the Metal API.
     */
	[[nodiscard]] I_Context& GetRenderContext() const override { return *_context; }

	/**
	 * @brief Gets the Metal-specific rendering context
	 * @return Render::Context::MetalContext& Reference to the MetalContext
	 * @details Provides access to the Metal-specific rendering context for platform-specific rendering operations. This method returns a reference to the MetalContext instance managed by the CocoaApplication, allowing other components to perform rendering operations that are specific to the Metal API on macOS.
	 */
	[[nodiscard]] MetalContext& GetMetalContext() const { return *_context; }

	[[nodiscard]] static CocoaApplication& StGet() { return dynamic_cast<CocoaApplication&>(I_Application::StGet()); }

public:
	void SetEventHubDispatcher() override;

private:
	void _BindWindowCallbacks() const;
	void _SetWindowCallbacks();

	void _BindViewCallbacks();
	void _SetViewEventCallbacks();

	void _OnWindowWillClose(const NS::Notification* notification);

	void _OnMouseButtonDown(const NS::Event* event);
	void _OnMouseButtonUp(const NS::Event* event);
	void _OnMouseButtonDragged(const NS::Event* event);
	void _OnMouseMoved(const NS::Event* event);

	void _OnKeyDown(const NS::Event* event);
	void _OnKeyUp(const NS::Event* event);

private:
	NS::SharedPtr<NS::Application> _appCocoa = nullptr; ///< Pointer to the Cocoa application instance

	std::unique_ptr<MetalContext> _context = nullptr; ///< Pointer to the Metal rendering context
	std::unique_ptr<CocoaWindow> _window = nullptr; ///< Pointer to the application window

	ImGuiMetalLayer* _imguiLayer = nullptr; ///< Pointer to the ImGui layer for rendering UI

	Native::NsApplicationDelegate _appDelegate; ///< Delegate for handling Cocoa application events

	std::thread _loopThread; ///< Thread for running the application loop
	std::atomic<bool> _tickPending; ///< Flag to indicate if a tick is pending for the next frame

public:
	MTL::RenderPipelineState* defaultRenderPipelineState = nullptr;
};

}

#endif //CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP
