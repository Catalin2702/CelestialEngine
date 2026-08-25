//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: CocoaApplication.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-08-25

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

#include <dispatch/dispatch.h>

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
	class Menu;
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
	/**
	 * @brief Fires the application's error event into the event hub
	 */
	void DispatchErrorEvent(int errorCode, const char* description) const;
	/**
	 * @brief Fires the application's tick event into the event hub
	 */
	void DispatchTickEvent() const;
	/**
	 * @brief Fires the application's update event into the event hub
	 */
	void DispatchUpdateEvent() const;
	/**
	 * @brief Fires the application's render event into the event hub
	 */
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
class CE_CORE_API CocoaApplication: public I_Application {
	friend class CocoaApplicationDelegate;

	enum EventHubSubscription: std::size_t {
		AppError = 0,
		VSyncChange = 1,
		WindowClose = 2,
		WindowError = 3,
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
	CocoaApplication(const CocoaApplication&) = delete;

	/**
	 * @brief Move constructor (deleted)
	 */
	CocoaApplication(CocoaApplication&&) noexcept = delete;

	/**
	 * @brief Destructor
	 */
	~CocoaApplication() override;

public:
	/**
	 * @brief Copy assignment operator (deleted)
	 */
	CocoaApplication& operator=(const CocoaApplication&) = delete;

	/**
	 * @brief Move assignment operator (deleted)
	 */
	CocoaApplication& operator=(CocoaApplication&&) = delete;

public:
	/**
	 * @brief Starts the application
	 * @details Starts the AppKit Cocoa application loop
	 */
	void Start() override;

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

	/**
	 * @brief Sets the running state and reconciles the frame pacing with it
	 * @details Running starts the pacing (_Run: tick loop or display link, depending on VSync); stopping pauses it (_Pause).
	 */
	void SetRunning(bool running) override;

protected:
	/**
	 * @brief No-op: window creation happens in Init(), interleaved with the Metal context setup
	 */
	void _InitWindow() override {}

	/**
	 * @brief No-op: renderer creation happens in Init(), interleaved with the window setup
	 */
	void _InitRenderer() override {}

public:
	/**
     * @brief Gets the application's window
     * @return Window::I_Window& Reference to the window
     * @details Provides access to the window for rendering operations. This method returns a reference to the window instance managed by the CocoaApplication, allowing other components to interact with the window as needed.
     */
	[[nodiscard]] const I_Window& GetWindow() const override { return _window; }

	/**
	 * @brief Gets the Cocoa-specific window
	 * @return Window::CocoaWindow& Reference to the CocoaWindow
	 * @details Provides access to the Cocoa-specific window for platform-specific operations. This method returns a reference to the CocoaWindow instance managed by the CocoaApplication, allowing other components to perform operations that are specific to the macOS platform.
	 */
	[[nodiscard]] const CocoaWindow& GetCocoaWindow() const { return _window; }

	/**
	 * @brief Gets the Cocoa-specific window for operations that mutate it
	 */
	[[nodiscard]] CocoaWindow& GetCocoaWindow() { return _window; }

	/**
     * @brief Gets the rendering context
     * @return Render::Context::I_Context& Reference to the rendering context
     * @details Provides access to the rendering context for rendering operations. This method returns a reference to the MetalContext instance managed by the CocoaApplication, allowing other components to perform rendering using the Metal API.
     */
	[[nodiscard]] const I_Context& GetRenderContext() const override { return _context; }

	/**
	 * @brief Gets the Metal-specific rendering context
	 * @return Render::Context::MetalContext& Reference to the MetalContext
	 * @details Provides access to the Metal-specific rendering context for platform-specific rendering operations. This method returns a reference to the MetalContext instance managed by the CocoaApplication, allowing other components to perform rendering operations that are specific to the Metal API on macOS.
	 */
	[[nodiscard]] const MetalContext& GetMetalContext() const { return _context; }

	/**
	 * @brief Gets the Metal-specific rendering context for operations that mutate it (VSync, pipeline state, ...)
	 */
	[[nodiscard]] MetalContext& GetMetalContext() { return _context; }

	/**
	 * @brief Gets the application singleton downcast to CocoaApplication
	 */
	[[nodiscard]] static CocoaApplication& StGet() { return dynamic_cast<CocoaApplication&>(I_Application::StGet()); }

public:
	/**
	 * @brief Binds every raw source (view/window dispatchers, application events, render context) to the event hub's Receive* methods
	 */
	void SetEventHubDispatcher() override;
	/**
	 * @brief Subscribes the application-level handlers to the hub
	 * @details The input state subscribes FIRST (before any other subscriber), then error logging, VSync and window close handlers.
	 */
	void SubscribeToHubDispatcher() override;
	/**
	 * @brief Removes the input state and the application-level handlers from the hub
	 */
	void UnsubscribeFromDispatcher() override;

public:
	/**
	 * @brief Menu callback: quits the application
	 */
	static void StOnQuitMenuCallback(void*, SEL selector, const NS::Object* sender);
	/**
	 * @brief Menu callback: toggles VSync in the config and applies it to the render context
	 */
	static void StOnToggleVSyncCallback(void*, SEL selector, const NS::Object* sender);
	/**
	 * @brief Menu callback: minimizes the window
	 */
	static void StOnMiniaturizeCallback(void*, SEL selector, const NS::Object* sender);
	/**
	 * @brief Menu callback: restores the window from the dock
	 */
	static void StOnDeminiaturizeCallback(void*, SEL selector, const NS::Object* sender);
	/**
	 * @brief Menu callback: toggles fullscreen mode
	 */
	static void StOnToggleFullscreenCallback(void*, SEL selector, const NS::Object* sender);

private:
	/**
	 * @brief Starts the frame pacing that matches the current VSync state
	 * @details VSync on: resumes the CAMetalDisplayLink; VSync off: starts the dedicated tick loop thread.
	 */
	void _Run();

	/**
	 * @brief Stops the active frame pacing (display link or tick loop)
	 */
	void _Pause();

	/**
	 * @brief Binds the Metal context's view delegates (frame draw, display link) to the application
	 */
	void _BindContextDelegates();

	/**
	 * @brief Starts the tick loop thread that paces frames while VSync is off
	 * @details No-op if the thread is already running. The thread dispatches Tick onto the main queue; each dispatched block
	 *			re-checks IsRunning() and the loop flag before ticking, so stale blocks left over from a mode switch are dropped.
	 */
	void _StartTickLoop();

	/**
	 * @brief Signals the tick loop thread to stop and joins it
	 * @details Safe to call from the main thread: the thread only dispatches asynchronously, so the join never deadlocks.
	 *			No-op if the thread is not running.
	 */
	void _StopTickLoop();

	/**
	 * @brief Handles the application's did-finish-launching lifecycle event
	 * @details Bound to NsApplicationDelegate and invoked once the AppKit run loop has started. Performs the app-level setup
	 *			that must happen after launch completes: promotes the process to a regular UI app, builds the main menu,
	 *			activates the app and reveals the window.
	 */
	void _OnDidFinishLaunching(NS::Notification* notification) const;

	/**
	 * @brief AppKit will-finish-launching hook (pre-run-loop setup)
	 */
	void _OnWillFinishLaunching(NS::Notification* notification) const;

	/**
	 * @brief Application-level reaction to a window close event dispatched by the event hub
	 * @details Subscribed to CocoaEventHubDispatcher::cocoaApplicationEventHub.onCloseMulticastDispatcher. Routes the close
	 *			through the engine's clean shutdown path.
	 */
	void _OnWindowClose(Events::WindowCloseEvent& event);

	/**
	 * @brief Application-level reaction to a VSync change dispatched by the event hub
	 * @details Subscribed to CocoaEventHubDispatcher::metalRenderContextEventHub.onChangeVSyncDispatcher. Reconciles the
	 *			frame pacing with the new state via a stop/start of the run loop (tick loop vs CAMetalDisplayLink).
	 */
	void _OnVSyncChange(Events::VSyncEvent& event);

	/**
	 * @brief Per-frame draw callback from the MetalKit view: runs one Tick
	 */
	void _OnDraw(MTK::View*);

public:
	CocoaEventHubDispatcher eventHubDispatcher; ///< Multicast event hub fed by the native view/window unicast dispatchers
	CocoaApplicationEventHandler applicationEventHandler; ///< Fires the application's own AppTick/Update/Render/Error events into the hub

private:
	NS::SharedPtr<NS::Application> _appCocoa = nullptr; ///< Pointer to the Cocoa application instance

	// Declaration order matters twice over. Construction: the context comes up first, since the window is handed the
	// context-owned MetalKit view as its content view. Destruction (reverse order): the window goes down first, because it
	// retains that view and the view references the context-owned event dispatcher - so the window has to let go of it
	// before the context tears it down.
	MetalContext _context;	///< The Metal rendering context, owned by value
	CocoaWindow _window;	///< The application window, owned by value

	ImGuiMetalLayer* _imguiLayer = nullptr; ///< Pointer to the ImGui layer for rendering UI

	Native::NsApplicationDelegate _appDelegate; ///< Delegate for handling Cocoa application events

	std::thread _loopThread; ///< Thread for running the application loop (only while VSync is off)
	std::atomic<bool> _loopThreadRunning{false}; ///< Controls the tick loop thread lifetime independently of the app-running state
	dispatch_semaphore_t _tickSemaphore = nullptr; ///< Paces the tick loop against main-thread frame completions (avoids busy-spinning)

	std::array<uint32_t, _Count> _eventHubHandlers{};

public:
	MTL::RenderPipelineState* defaultRenderPipelineState = nullptr;
};

}

#endif //CE_CORE_APPLICATION_PLATFORMS_MAC_COCOA_COCOAAPPLICATION_HPP
