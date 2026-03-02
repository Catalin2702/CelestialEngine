//
// Module: Core
// File: Application.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-15
// Updated: 2026-02-28
//

#include "Core/Application.hpp"
#include "Define/Bind.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/I_Event.hpp"
#include "Layers/I_Layer.hpp"
#include "Tools/Log/Log.hpp"
#include "Window/I_Window.hpp"

#ifdef CE_PLATFORM_MACOS
#include "Window/Platforms/Mac/MetalWindow.hpp"
#include "Window/Platforms/Universal/OpenGlWindow.hpp"
#else
#include "Window/Platforms/Universal/OpenGlWindow.hpp"
#endif

#include <GLFW/glfw3.h>

#include <stdexcept>

// Platform-specific window type selection
#ifdef CE_PLATFORM_MACOS
using MetalWindow = CE::Window::MetalWindow;		///< Use Metal window on macOS
using OpenGlWindow = CE::Window::OpenGlWindow;		///< Use OpenGL window on macOS as well (fallback)
#else
using OpenGlWindow = CE::Window::OpenGlWindow;		///< Use OpenGL window on other platforms
#endif


namespace CE::Core {

/**
 * @brief Static singleton instance pointer
 */
Application* Application::_instance = nullptr;

/**
 * @brief Default constructor implementation
 * @details Asserts that no other Application instance exists (singleton pattern),
 *			then initializes with default window properties: "CelestialEngine" title,
 *			1280x720 resolution, VSync enabled
 */
Application::Application() {
	assert(_instance == nullptr && "Application already exists!");
	_Init({"CelestialEngine", 1280, 720, true, Types::Window::GraphicsApi::OpenGL});
}

/**
 * @brief Constructor with window properties implementation
 * @param windowProps Window configuration (title, width, height, VSync)
 * @details Initializes the application with the specified window properties
 */
Application::Application(const TypeWindow::WindowProps& windowProps) {
	assert(_instance == nullptr && "Application already exists!");
	_Init(windowProps);
}

/**
 * @brief Constructor with individual window parameters implementation
 * @param title Window title string
 * @param width Window width in pixels
 * @param height Window height in pixels
 * @param VSync Enable or disable vertical synchronization
 * @param graphicsApi Graphics API to use for rendering
 * @details Initializes the application with individual window parameters by
 *			constructing a WindowProps object and passing it to _Init
 */
Application::Application(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync, const Types::Window::GraphicsApi graphicsApi) {
	assert(_instance == nullptr && "Application already exists!");
	_Init({title, width, height, VSync, graphicsApi});
}

/**
 * @brief Destructor implementation
 * @details Cleans up application resources in the correct order:
 *			1. Clear layer stack (detaches all layers, which may use GLFW)
 *			2. Destroy window (calls glfwDestroyWindow)
 *			3. Terminate GLFW library
 *			This order is critical to avoid GLFW errors when layers (e.g., ImGui)
 *			try to clean up GLFW resources during their OnDetach.
 */
Application::~Application() {
	// First, detach all layers (they may use GLFW in their cleanup)
	_layerStack.Clear();

	// Then destroy the window (calls glfwDestroyWindow)
	_window.reset();

	// Reset the singleton instance pointer
	_instance = nullptr;
}

void Application::Update() {
	for (const auto layer: _layerStack)
		layer->OnUpdate();
	_window->OnUpdate();
}

/**
 * @brief Main application loop implementation
 * @details Runs continuously while _running is true. Each iteration:
 *			1. Updates all layers in the layer stack (OnUpdate)
 *			2. Updates the window (polls events and swaps buffers)
 *			The loop exits when _running is set to false (typically by window close event)
 */
void Application::Run() {
	while (_running) {
		Update();
	}
}

/**
 * @brief Event handling implementation
 * @param event Reference to the event to be processed
 * @details Handles events in the following order:
 *			1. Dispatches window close events to OnWindowClose handler
 *			2. Propagates event through layer stack in reverse order (overlays first)
 *			3. Stops propagation if any layer handles the event
 *			This allows overlays (like ImGui) to intercept events before game layers
 */
void Application::OnEvent(Events::I_Event& event) {
	Events::EventDispatcher eventDispatcher(event);
	eventDispatcher.Dispatch<Events::WindowCloseEvent>(BIND_FN_ONE_PARAM(Application::OnWindowClose));

	for (auto it = _layerStack.end(); it != _layerStack.begin(); ) {
		(*--it)->OnEvent(event);
		if (event.IsHandled())
			break;
	}
}

/**
 * @brief Window close event handler implementation
 * @return bool Always returns true to indicate the event was handled
 * @details Sets _running to false, which causes the main loop to exit,
 *			and logs the window close event. The event parameter is unnamed
 *			as its content is not needed for this handler.
 */
bool Application::OnWindowClose(const Events::WindowCloseEvent&) {
	_running = false;
	CE_CORE_INFO("Application::OnWindowClose: Window closed");
	return true;
}

/**
 * @brief Adds a layer to the layer stack
 * @param layer Pointer to the layer to add
 * @details Delegates to LayerStack::PushLayer, which inserts the layer
 *			before overlays and calls its OnAttach method
 */
void Application::PushLayer(Layers::I_Layer *layer) {
	_layerStack.PushLayer(layer);
}

/**
 * @brief Adds an overlay to the layer stack
 * @param overlay Pointer to the overlay to add
 * @details Delegates to LayerStack::PushOverlay, which inserts the overlay
 *			at the end of the stack (after all layers) and calls its OnAttach method
 */
void Application::PushOverlay(Layers::I_Layer *overlay) {
	_layerStack.PushOverlay(overlay);
}

void Application::PopLayer(Layers::I_Layer* layer) {
	_layerStack.PopLayer(layer);
}

void Application::PopOverlay(Layers::I_Layer* overlay) {
	_layerStack.PopOverlay(overlay);
}

/**
 * @brief Private initialization method implementation
 * @param windowProps Window configuration properties
 * @details Performs the following initialization steps:
 *			1. Sets the singleton instance pointer
 *			2. Creates the appropriate window (Metal on macOS, OpenGL elsewhere)
 *			3. Sets up the event callback to route events to OnEvent
 *			4. Sets _running to true to start the main loop
 *			Throws std::runtime_error if window creation fails or API is unsupported.
 */
void Application::_Init(const TypeWindow::WindowProps& windowProps) {
	_instance = this;
	switch (windowProps.graphicsApi) {
		case Types::Window::GraphicsApi::OpenGL:
			_window = std::unique_ptr<Window::I_Window>(
				Window::I_Window::CreateWindow<OpenGlWindow>(windowProps)
			);
			break;
#ifdef CE_PLATFORM_MACOS
		case Types::Window::GraphicsApi::Metal:
			_window = std::unique_ptr<Window::I_Window>(
				Window::I_Window::CreateWindow<MetalWindow>(windowProps)
			);
			break;
#endif
		default:
			CE_CORE_ERROR("Application::_Init: Unsupported graphics API specified in window properties. Graphics API: {0}", windowProps.graphicsApi);
			throw std::runtime_error("Unsupported graphics API specified in window properties");
	}

	if (not _window) {
		CE_CORE_ERROR("Application::_Init: Can't initialize the window");
		throw std::runtime_error("Can't initialize the window");
	}

	_window->SetEventCallback(BIND_FN_ONE_PARAM(Application::OnEvent));
	_running = true;
}

}
