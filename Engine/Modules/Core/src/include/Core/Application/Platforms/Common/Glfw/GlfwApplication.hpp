//
// Module: CelestialEngine/Engine/Modules/Core/Application/Platforms/Common/Glfw
// File: GlfwApplication.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-08-24
//

#pragma once

#ifndef CE_CORE_APPLICATION_PLATFORMS_COMMON_GLFW_GLFWAPPLICATION_HPP
#define CE_CORE_APPLICATION_PLATFORMS_COMMON_GLFW_GLFWAPPLICATION_HPP

#include "Define/DynamicLinker.hpp"

#include "Core/Application/I_Application.hpp"
#include "Core/MainHub/Events/Platforms/Common/Glfw/GlfwEventHubDispatcher.hpp"
#include "Core/Render/Buffer/Platforms/Common/OpenGl/OpenGlBuffer.hpp"
#include "Core/Render/Shader/Platforms/Common/OpenGl/OpenGlShaderProgram.hpp"

#include <array>
#include <memory>

// On macOS GLFW still creates a real NSApplication, so the GLFW backend reuses the native menu bar. SEL and NS::Object are only
// needed for the menu callbacks, which exist on macOS alone; elsewhere GLFW has no native menu bar.
#ifdef CE_PLATFORM_MACOS
	#include <objc/objc.h>
#endif


namespace CE::Core {
	class OpenGlContext;
	class I_Layer;
	class ImGuiOpenGlLayer;
	class GlfwWindow;
}

#ifdef CE_PLATFORM_MACOS
namespace NS {
	class Object;
}
#endif

namespace CE::Core {

/**
 * @class GlfwApplicationEventHandler
 * @brief Owns the unicast dispatchers the GLFW application fires for its own lifecycle events
 * @details AppTick/AppUpdate/AppRender are raised every frame from Tick(); AppError is raised when the application reports a
 *			failure. The application binds these to the event hub's ReceiveApp* methods (see
 *			GlfwApplication::SetEventHubDispatcher), so firing one flows into the hub and out to every subscriber.
 */
class GlfwApplicationEventHandler {
public:
	struct GlfwApplicationEvents {
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
	GlfwApplicationEvents applicationEvents;
};

/**
 * @class GlfwApplication
 * @brief GLFW-specific application implementation
 * @details Provides a GLFW-specific implementation of the I_Application interface for platforms that support GLFW.
 *			Manages the application lifecycle and event handling using GLFW for window management and input handling.
 */
class CE_CORE_API GlfwApplication: public I_Application {
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
	GlfwApplication();

	/**
	 * @brief Copy constructor (deleted)
	 */
	GlfwApplication(const GlfwApplication&) = delete;

	/**
	 * @brief Move constructor (deleted)
	 */
	GlfwApplication(GlfwApplication&&) noexcept = delete;

	/**
	 * @brief Destructor
	 */
	~GlfwApplication() override;

public:
	/**
	 * @brief Copy assignment operator (deleted)
	 */
	GlfwApplication& operator=(const GlfwApplication&) = delete;

	/**
	 * @brief Move assignment operator (deleted)
	 */
	GlfwApplication& operator=(GlfwApplication&&) noexcept = delete;

public:
	/**
	 * @brief Starts the application
	 * @details Starts the GLFW application loop
	 */
	void Start() override;

	/**
	 * @brief Quits the application
	 * @details Terminates the GLFW application loop
	 */
	void Quit() override;

	/**
	 * @brief Updates the application state
	 * @param deltaTime Time elapsed since the last update in seconds
	 * @details Called every frame to update the application. Updates all layers in the layer stack.
	 */
	void Tick(float deltaTime) override;

	/**
	 * @brief Initializes the application window
	 * @details Creates the application window and sets up event callbacks based on the window properties set in Config::Config.
	 *			Initializes the appropriate input system based on the window API.
	 */
	void Init() override;

	/**
	 * @brief Initializes the ImGui layer
	 * @details Initializes the ImGui layer for rendering UI based on the specified graphics API set in Config::Config.
	 *			For GLFW with OpenGL, this will involve creating an ImGuiOpenGLLayer instance and pushing it as an overlay.
	 */
	void InitImGuiLayer() override;

public:
	/**
	 * @brief Sets the ImGui layer for the application
	 * @param imguiLayer Pointer to the ImGui layer to set
	 * @details Sets the ImGui layer for rendering UI. This method allows you to specify the ImGui layer that will be used for rendering the user interface.
	 *			It must be an ImGuiOpenGlLayer or derived class that is compatible with the OpenGL rendering context used by the application.
	 */
	void SetImGuiLayer(I_Layer* imguiLayer) override;

	/**
	 * @brief Removes the ImGui layer from the application
	 * @details Removes the currently set ImGui layer from the application and pops it from the layer stack. This will stop rendering the ImGui UI and free up resources associated with the ImGui layer.
	 */
	void RemoveImGuiLayer() override;

	/**
	 * @brief Binds every raw source (window callbacks, application events, render context) to the event hub's Receive* methods
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
	 * @brief Gets the application singleton downcast to GlfwApplication
	 */
	[[nodiscard]] static GlfwApplication& StGet();

#ifdef CE_PLATFORM_MACOS
	// Native macOS menu-bar callbacks. GLFW creates the NSApplication, so the same menu (built by CreateMenuBar) drives these.
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
#endif

private:
	/**
	 * @brief Application-level reaction to a window close event dispatched by the event hub
	 * @details Subscribed to GlfwEventHubDispatcher::glfwWindowEventHub.onCloseMulticastDispatcher; quits the application.
	 */
	void _OnWindowClose(Events::WindowCloseEvent& event);

	/**
	 * @brief Application-level reaction to a VSync change dispatched by the event hub
	 * @details Subscribed to GlfwEventHubDispatcher::openGlRenderContextEventHub.onChangeVSyncDispatcher. Updates the
	 *			frame limiter target: monitor refresh rate with VSync on, the configured refresh rate with VSync off.
	 */
	void _OnVSyncChange(const Events::VSyncEvent& event) const;

protected:
	/**
	 * @brief Initializes the application window
	 * @details Creates the application window and sets up event callbacks.
	 *			Initializes the appropriate input system based on the window API.
	 */
	void _InitWindow() override;

	/**
	 * @brief Initializes the renderer
	 * @details Initializes the rendering context based on the specified graphics API.
	 *			For GLFW, this will typically involve setting up an OpenGL or Vulkan rendering context.
	 */
	void _InitRenderer() override;

public:
	/**
	 * @brief Gets the application window
	 * @return Reference to the application window
	 * @details Returns a reference to the application window, which can be used for various operations such as event handling and rendering.
	 */
	[[nodiscard]] I_Window& GetWindow() const override;

	/**
	 * @brief Gets the GLFW-specific window for platform-specific operations
	 */
	[[nodiscard]] GlfwWindow& GetGlfwWindow() const;

	/**
	* @brief Gets the rendering context
	* @return Render::Context::I_Context& Reference to the rendering context
	 * @details Returns a reference to the application renderer context, which can be used for rendering operations and managing graphics resources.
	 */
	I_Context& GetRenderContext() const override;

	/**
	 * @brief Gets the OpenGL-specific rendering context for platform-specific operations
	 */
	OpenGlContext& GetOpenGlContext() const;

public:
	GlfwEventHubDispatcher eventHubDispatcher;
	GlfwApplicationEventHandler applicationEventHandler; ///< Fires the application's own AppTick/Update/Render/Error events into the hub

private:
	std::unique_ptr<OpenGlContext> _context; ///< Pointer to the OpenGL rendering context
	std::unique_ptr<GlfwWindow> _window; ///< Pointer to the GLFW window

	OpenGlVertexBuffer _vertexBuffer;
	OpenGlIndexBuffer _indexBuffer;
	OpenGlShaderProgram _shaderProgram; ///< Pointer to the OpenGL shader program used for rendering

	ImGuiOpenGlLayer* _imguiLayer; ///< Pointer to the ImGui layer for rendering UI

	uint32_t _vertexArrayId = 0; ///< OpenGL buffer IDs for vertex array, vertex buffer, and index buffer

	std::array<uint32_t, _Count> _eventHubHandlers{}; ///< Handles of the app-level hub subscriptions, indexed by EventHubSubscription
};

}

#endif //CE_CORE_APPLICATION_PLATFORMS_COMMON_GLFW_GLFWAPPLICATION_HPP
