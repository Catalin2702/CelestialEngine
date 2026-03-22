//
// Module: CelestialEngine/Engine/Modules/Core
// File: Application.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-15
// Updated by: Catalin Chirosca
// Updated: 2026-03-22
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
class I_RenderLayer;
}

namespace CE::Render::Context {
class I_Context;
}

namespace CE::Window {
class I_Window;
}

namespace TypeWindow = CE::Types::Window;

/**
 * @namespace CE::Core
 * @brief Core application framework components
 * @details Contains the Application class which serves as the main entry point for client applications.
 *			Manages the application lifecycle, event handling, and layer management. Client applications
 *			should inherit from Application and implement the CreateApplication factory functions.
 */
namespace CE::Core {

/**
 * @class Application
 * @brief Core application class that manages the main loop and application lifecycle
 * @details Central class that initializes the window, manages the layer stack, handles events,
 *			and runs the main application loop. Client applications should inherit from this
 *			class and implement the CreateApplication factory functions.
 */
class CE_API Application {
public:
	/**
	 * @brief Default constructor
	 * @details Creates an application with default window properties
	 */
	Application();

	/**
	 * @brief Virtual destructor
	 * @details Cleans up application resources
	 */
	virtual ~Application();

public:
	/**
	 * @brief Updates the application state
	 * @details Called every frame to update the application. Updates all layers in the layer stack.
	 */
	virtual void Update();

	/**
	 * @brief Runs the main application loop
	 * @details Starts the main loop, in which it calls Update every frame
	 */
	virtual void Run();

	/**
	 * @brief Handles application events
	 * @param event Reference to the event to be processed
	 * @details Dispatches events to appropriate handlers and propagates them
	 *			through the layer stack
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
	 *			events before regular layers
	 */
	void PushOverlay(Layers::I_Layer* overlay);

	/**
	 * @brief Removes a layer from the layer stack
	 * @param layer Pointer to the layer to remove
	 * @details Delegates to LayerStack::PopLayer, which removes the layer
	 *			from the stack and calls its OnDetach method
	 */
	void PopLayer(Layers::I_Layer* layer);

	/**
	 * @brief Removes an overlay from the layer stack
	 * @param overlay Pointer to the overlay layer to remove
	 * @details Delegates to LayerStack::PopOverlay, which removes the overlay
	 *			from the stack and calls its OnDetach method
	 */
	void PopOverlay(Layers::I_Layer* overlay);

	/**
	 * @brief Sets the render layer (e.g. ImGui layer) for the application
	 * @param renderLayer Pointer to the render layer to set
	 * @details The render layer is used for rendering UI and is typically an ImGui layer.
	 *			This method allows setting or changing the render layer at runtime.
	 */
	void SetRenderLayer(Layers::I_RenderLayer* renderLayer) { _renderLayer = renderLayer; }

	/**
	 * @brief Checks if the application has any layers
	 * @return bool True if there are layers in the stack, false otherwise
	 * @details Utility method to check if the layer stack is empty
	 */
	[[nodiscard]] bool HasLayers() const { return not _layerStack.Empty(); }

	/**
	 * @brief Gets the number of layers in the stack
	 * @return size_t Number of layers in the stack
	 * @details Utility method to get the current size of the layer stack
	 */
	[[nodiscard]] size_t LayersSize() const { return _layerStack.Size(); }

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

	[[nodiscard]] Render::Context::I_Context* GetContext() const { return _context.get(); }

public:
	/**
	 * @brief Initializes window property
	 * @param windowProps Window configuration properties
	 * @details Public window initializer method. Creates the window
	 *			and sets up event callbacks based on the provided window properties.
	 *			Initialize the appropriate Input system based on the window API.
	 */
	void InitWindow(const TypeWindow::WindowProps& windowProps);

	void InitRenderer(const TypeWindow::WindowProps& windowProps);

	/**
	 * @brief Initializes the ImGui layer based on window properties
	 * @param windowProps Window configuration properties
	 * @details Public ImGui layer initializer method. Creates the appropriate
	 *			ImGui layer based on the graphics API and window API specified
	 *			in the window properties, and pushes it as an overlay.
	 */
	void InitImGuiLayer(const TypeWindow::WindowProps& windowProps);

	void InitAll(const TypeWindow::WindowProps& windowProps);

private:
	void _Init();

	void _SetWindowCallbacks() const;

private:
	std::unique_ptr<Window::I_Window> _window;		///< Application window
	std::unique_ptr<Render::Context::I_Context> _context;	///< Graphics context for rendering
	Layers::I_RenderLayer* _renderLayer = nullptr;	///< ImGui layer for rendering UI
	bool _running = false;							///< Flag indicating if application is running
	Layers::LayerStack _layerStack;					///< Stack of layers and overlays

private:
	static Application* _instance;					///< Singleton application instance
};

/**
 * @brief Factory function to create the application with default properties
 * @return Application* Pointer to the created application
 * @details Must be implemented by the client application
 */
std::unique_ptr<Application> CreateApplication();

}

#endif //CE_CORE_APPLICATION_HPP
