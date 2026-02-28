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
#include "Window/I_Viewport.hpp"

#ifdef CE_PLATFORM_MACOS
#include "Window/Platforms/Mac/MetalViewport.hpp"
#include "Window/Platforms/Universal/OpenGLViewport.hpp"
#else
#include "Window/Platforms/Universal/OpenGLViewport.hpp"
#endif

// Platform-specific viewport type selection
#ifdef CE_PLATFORM_MACOS
using Viewport = CE::Window::MetalViewport;  ///< Use Metal viewport on macOS
#else
using Viewport = CE::Window::OpenGLViewport; ///< Use OpenGL viewport on other platforms
#endif


namespace CE::Core {

/**
 * @brief Static singleton instance pointer
 */
Application* Application::_instance = nullptr;

/**
 * @brief Default constructor implementation
 * @details Asserts that no other Application instance exists (singleton pattern),
 *          then initializes with default window properties: "CelestialEngine" title,
 *          1280x720 resolution, VSync enabled
 */
Application::Application() {
	assert(_instance == nullptr && "Application already exists!");
	_Init({"CelestialEngine", 1280, 720, true});
}

/**
 * @brief Constructor with window properties implementation
 * @param windowProps Window configuration (title, width, height, VSync)
 * @details Initializes the application with the specified window properties
 */
Application::Application(const TypeWindow::WindowProps& windowProps) {
	_Init(windowProps);
}

/**
 * @brief Constructor with individual window parameters implementation
 * @param title Window title string
 * @param width Window width in pixels
 * @param height Window height in pixels
 * @param VSync Enable or disable vertical synchronization
 * @details Initializes the application with individual window parameters by
 *          constructing a WindowProps object and passing it to _Init
 */
Application::Application(const std::string& title, const unsigned int width, const unsigned int height, const bool VSync) {
	_Init({title, width, height, VSync});
}

/**
 * @brief Destructor implementation
 * @details Default destructor - cleanup is handled automatically by smart pointers
 *          and layer stack destructor
 */
Application::~Application() = default;

/**
 * @brief Main application loop implementation
 * @details Runs continuously while _running is true. Each iteration:
 *          1. Updates all layers in the layer stack (OnUpdate)
 *          2. Updates the viewport (polls events and swaps buffers)
 *          The loop exits when _running is set to false (typically by window close event)
 */
void Application::Run() {
	while (_running) {
		for (const auto layer: _layerStack) {
			layer->OnUpdate();
		}
		_viewport->OnUpdate();
	}
}

/**
 * @brief Event handling implementation
 * @param event Reference to the event to be processed
 * @details Handles events in the following order:
 *          1. Dispatches window close events to OnWindowClose handler
 *          2. Propagates event through layer stack in reverse order (overlays first)
 *          3. Stops propagation if any layer handles the event
 *          This allows overlays (like ImGui) to intercept events before game layers
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
 *          and logs the window close event. The event parameter is unnamed
 *          as its content is not needed for this handler.
 */
bool Application::OnWindowClose(const Events::WindowCloseEvent&) {
	_running = false;
	CE_CORE_INFO("Window closed");
	return true;
}

/**
 * @brief Adds a layer to the layer stack
 * @param layer Pointer to the layer to add
 * @details Delegates to LayerStack::PushLayer, which inserts the layer
 *          before overlays and calls its OnAttach method
 */
void Application::PushLayer(Layers::I_Layer *layer) {
	_layerStack.PushLayer(layer);
}

/**
 * @brief Adds an overlay to the layer stack
 * @param overlay Pointer to the overlay to add
 * @details Delegates to LayerStack::PushOverlay, which inserts the overlay
 *          at the end of the stack (after all layers) and calls its OnAttach method
 */
void Application::PushOverlay(Layers::I_Layer *overlay) {
	_layerStack.PushOverlay(overlay);
}

/**
 * @brief Private initialization method implementation
 * @param windowProps Window configuration properties
 * @details Performs the following initialization steps:
 *          1. Sets the singleton instance pointer
 *          2. Creates the appropriate viewport (Metal on macOS, OpenGL elsewhere)
 *          3. Sets up the event callback to route events to OnEvent
 *          4. Sets _running to true to start the main loop
 *          Exits with error if viewport creation fails
 */
void Application::_Init(const TypeWindow::WindowProps& windowProps) {
	_instance = this;
	_viewport = std::unique_ptr<Window::I_Viewport>(
		Window::I_Viewport::CreateWindow<Viewport>(windowProps)
	);
	if (not _viewport) {
		CE_CORE_ERROR("Can't initialize the window");
		exit(EXIT_FAILURE);
	}
	_viewport->SetEventCallback(BIND_FN_ONE_PARAM(Application::OnEvent));
	_running = true;
}

}
