//
// Module: Core
// File: Application.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-15
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_CORE_APPLICATION_HPP
#define CE_CORE_APPLICATION_HPP

#include "Define/DynamicLinker.hpp"
#include "Layers/LayerStack.hpp"
#include "Types/Window/WindowProps.hpp"

#include <memory>

namespace CE::Events {
class I_Event;
class WindowCloseEvent;
}
namespace CE::Layers {
class I_Layer;
}
namespace CE::Window {
class I_Window;
}

namespace TypeWindow = CE::Types::Window;

/**
 * @namespace CE::Core
 * @brief Core application framework components
 * @details Contains the Application class which serves as the main entry point for client applications.
 *          Manages the application lifecycle, event handling, and layer management. Client applications
 *          should inherit from Application and implement the CreateApplication factory functions.
 */
namespace CE::Core {

/**
 * @class Application
 * @brief Core application class that manages the main loop and application lifecycle
 * @details Central class that initializes the window, manages the layer stack, handles events,
 *          and runs the main application loop. Client applications should inherit from this
 *          class and implement the CreateApplication factory functions.
 */
class CE_API Application {
public:
	/**
	 * @brief Default constructor
	 * @details Creates an application with default window properties
	 */
	Application();

	/**
	 * @brief Constructor with window properties
	 * @param windowProps Window configuration (title, dimensions, VSync)
	 * @details Creates an application with specified window properties
	 */
	Application(const TypeWindow::WindowProps& windowProps);

	/**
	 * @brief Constructor with individual window parameters
	 * @param title Window title string
	 * @param width Window width in pixels
	 * @param height Window height in pixels
	 * @param VSync Enable or disable vertical synchronization
	 * @param graphicsApi Graphics API to use for rendering
	 * @details Creates an application with individual window parameters
	 */
	Application(const std::string& title, unsigned int width, unsigned int height, bool VSync, Types::Window::GraphicsApi graphicsApi);

	/**
	 * @brief Virtual destructor
	 * @details Cleans up application resources
	 */
	virtual ~Application();

public:
	/**
	 * @brief Runs the main application loop
	 * @details Continues running until _running is set to false. Each iteration
	 *          updates all layers and polls events from the window.
	 */
	virtual void Run();

	/**
	 * @brief Handles application events
	 * @param event Reference to the event to be processed
	 * @details Dispatches events to appropriate handlers and propagates them
	 *          through the layer stack
	 */
	virtual void OnEvent(Events::I_Event& event);

	/**
	 * @brief Handles window close events
	 * @param event Window close event
	 * @return bool True if the event was handled
	 * @details Sets the running flag to false, causing the application to exit
	 */
	virtual bool OnWindowClose(const Events::WindowCloseEvent& event);

	/**
	 * @brief Adds a layer to the layer stack
	 * @param layer Pointer to the layer to add
	 * @details Layers are updated and receive events in the order they are pushed
	 */
	void PushLayer(Layers::I_Layer* layer);

	/**
	 * @brief Adds an overlay to the layer stack
	 * @param overlay Pointer to the overlay layer to add
	 * @details Overlays are rendered on top of regular layers and receive
	 *          events before regular layers
	 */
	void PushOverlay(Layers::I_Layer* overlay);

public:
	/**
	 * @brief Gets the singleton application instance
	 * @return Application& Reference to the application instance
	 * @details Returns the current application instance (singleton pattern)
	 */
	[[nodiscard]] static Application& Get() { return *_instance; }

	/**
	 * @brief Gets the application's window
	 * @return Window::I_Window* Pointer to the window
	 * @details Provides access to the window for rendering operations
	 */
	[[nodiscard]] Window::I_Window* GetWindow() const { return _window.get(); }

protected:
	/**
	 * @brief Initializes the application with window properties
	 * @param windowProps Window configuration properties
	 * @details Protected initialization method called by constructors. Creates the window
	 *          and sets up event callbacks.
	 */
	void _Init(const TypeWindow::WindowProps& windowProps);

private:
	std::unique_ptr<Window::I_Window> _window;		///< Application window
	bool _running = false;							///< Flag indicating if application is running
	Layers::LayerStack _layerStack;					///< Stack of layers and overlays
	static Application* _instance;					///< Singleton application instance
};

/**
 * @brief Factory function to create the application with default properties
 * @return Application* Pointer to the created application
 * @details Must be implemented by the client application
 */
std::unique_ptr<Application> CreateApplication();

/**
 * @brief Factory function to create the application with window properties
 * @param windowProps Window configuration properties
 * @return Application* Pointer to the created application
 * @details Must be implemented by the client application
 */
std::unique_ptr<Application> CreateApplication(const TypeWindow::WindowProps& windowProps);

/**
 * @brief Factory function to create the application with individual window parameters
 * @param title Window title string
 * @param width Window width in pixels
 * @param height Window height in pixels
 * @param VSync Enable or disable vertical synchronization
 * @param graphicsApi Graphics API to use for rendering
 * @return Application* Pointer to the created application
 * @details Must be implemented by the client application
 */
std::unique_ptr<Application> CreateApplication(const std::string& title, unsigned int width, unsigned int height, bool VSync, Types::Window::GraphicsApi graphicsApi);

}

#endif //CE_CORE_APPLICATION_HPP
