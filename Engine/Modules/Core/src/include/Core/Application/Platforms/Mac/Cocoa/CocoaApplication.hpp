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
#include "Core/MainHub/Events/Platforms/Mac/Cocoa/CocoaEventHubDispatcher.hpp"
#include "Core/Render/Context/Platforms/Mac/Metal/MetalContext.hpp"
#include "Core/Window/Platforms/Mac/Cocoa/CocoaWindow.hpp"
#include "Define/DynamicLinker.hpp"

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>

#include <array>
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
 * @class CocoaApplicationEventHandler
 * @brief Owns the unicast dispatchers the Cocoa application fires for its own lifecycle events
 * @details AppTick/AppUpdate/AppRender are raised every frame from Tick(); AppError is raised when the application reports a
 *			failure. The application binds these to the event hub's ReceiveApp* methods (see
 *			CocoaApplication::SetEventHubDispatcher), so firing one flows into the hub and out to every subscriber.
 */
class CocoaApplicationEventHandler {
public:
	struct CocoaApplicationEvents {
		UnicastDispatcher<int, const char*> onErrorDispatcher;
		UnicastDispatcher<> onTickDispatcher;
		UnicastDispatcher<> onUpdateDispatcher;
		UnicastDispatcher<> onRenderDispatcher;
	};

public:
	void DispatchErrorEvent(int errorCode, const char* description) const;
	void DispatchTickEvent() const;
	void DispatchUpdateEvent() const;
	void DispatchRenderEvent() const;

public:
	CocoaApplicationEvents applicationEvents;
};

/**
 * @class CocoaApplication
 * @brief macOS-specific application implementation using Cocoa
 * @details Provides a macOS-specific implementation of the I_Application interface using Cocoa for application management.
 *			Manages the application lifecycle and event handling for a native macOS application.
 */
class CE_API CocoaApplication: public I_Application {
	friend class CocoaApplicationDelegate;

	enum EventHubSubscription: std::size_t {
		AppError = 0,
		WindowClose = 1,
		WindowError = 2,
		_Count
	};

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
	void SubscribeToHubDispatcher() override;
	void UnsubscribeFromDispatcher() override;

public:
	CocoaEventHubDispatcher eventHubDispatcher; ///< Multicast event hub fed by the native view/window unicast dispatchers
	CocoaApplicationEventHandler applicationEventHandler; ///< Fires the application's own AppTick/Update/Render/Error events into the hub

private:
	void _BindContextDelegates();

	/**
	 * @brief Handles the application's did-finish-launching lifecycle event
	 * @details Bound to NsApplicationDelegate and invoked once the AppKit run loop has started. Performs the app-level setup
	 *			that must happen after launch completes: promotes the process to a regular UI app, builds the main menu,
	 *			activates the app and reveals the window.
	 */
	void _OnDidFinishLaunching() const;

	/**
	 * @brief Builds and installs the application's main menu
	 * @details Creates a minimal main menu with an application menu holding a standard "Quit" item wired to Cmd+Q, so the
	 *			native menu bar and its keyboard shortcut work.
	 */
	void _CreateMenuBar() const;

	/**
	 * @brief Application-level reaction to a window close event dispatched by the event hub
	 * @details Subscribed to CocoaEventHubDispatcher::cocoaApplicationEventHub.onCloseMulticastDispatcher. Routes the close
	 *			through the engine's clean shutdown path.
	 */
	void _OnWindowClose(Events::WindowCloseEvent& event);

	void _OnDraw(MTK::View*);

private:
	NS::SharedPtr<NS::Application> _appCocoa = nullptr; ///< Pointer to the Cocoa application instance

	std::unique_ptr<MetalContext> _context = nullptr; ///< Pointer to the Metal rendering context
	std::unique_ptr<CocoaWindow> _window = nullptr; ///< Pointer to the application window

	ImGuiMetalLayer* _imguiLayer = nullptr; ///< Pointer to the ImGui layer for rendering UI

	Native::NsApplicationDelegate _appDelegate; ///< Delegate for handling Cocoa application events

	std::thread _loopThread; ///< Thread for running the application loop
	std::atomic<bool> _tickPending; ///< Flag to indicate if a tick is pending for the next frame

	std::array<uint32_t, _Count> _eventHubHandlers{};

public:
	MTL::RenderPipelineState* defaultRenderPipelineState = nullptr;
};

}

#endif //CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP
