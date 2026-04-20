//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: I_Application.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-04-20
//

#pragma once

#ifndef CE_CORE_APPLICATION_I_APPLICATION_HPP
#define CE_CORE_APPLICATION_I_APPLICATION_HPP

#include "Define/DynamicLinker.hpp"
#include "Layers/LayerStack.hpp"

#include <atomic>
#include <memory>

namespace CE {
namespace Events {
class I_Event;
}

namespace Layers {
class I_Layer;
}

namespace Render::Context {
class I_Context;
}

namespace Types {
namespace Window {
struct WindowProps;
}
namespace Render {
enum class GraphicsApi: uint8_t;
}
}

namespace Window {
class I_Window;
}


/**
 * @namespace CE::Core::Application
 * @brief Core application interface and related types
 * @details Contains the I_Application interface which defines the core application API for client applications.
 *			Defines the main application lifecycle methods, event handling, and layer management functions.
 *			Client applications should implement this interface and provide a factory function to create an instance of their application.
 */
namespace Core::Application {

/**
 * @class I_Application
 * @brief Interface for the core application class
 * @details Defines the main application lifecycle methods, event handling, and layer management functions.
 *			Client applications should implement this interface and provide a factory function to create an instance of their application.
 */
class CE_API I_Application {
public:
	/**
	 * @brief Virtual destructor
	 */
	virtual ~I_Application();

public:
	/**
	 * @brief Virtual method to run the application
	 */
	virtual void Run() = 0;

	/**
	 * @brief Virtual method to quit the application
	 */
	virtual void Quit() = 0;

	/**
	 * @brief Virtual method to update the application state
	 * @param deltaTime Time elapsed since the last update in seconds
	 */
	virtual void Tick(float deltaTime) = 0;

	/**
	 * @brief Virtual method to handle events
	 * @param event Reference to the event to be processed
	 */
	virtual void OnEvent(Events::I_Event& event) = 0;

	/**
     * @brief Virtual method to initialize the application with window properties
     * @param windowProps Window configuration properties
     */
	virtual void Init(const Types::Window::WindowProps& windowProps) = 0;

	/**
	 * @brief Initializes the ImGui layer with the specified graphics API
	 * @param graphicsApi Graphics API to use for the ImGui layer
	 */
	void virtual InitImGuiLayer(Types::Render::GraphicsApi graphicsApi) = 0;

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

protected:
	/**
	 * @brief Initializes the application with the specified window properties
	 * @param windowProps Window configuration properties
	 */
	void virtual _InitWindow(const Types::Window::WindowProps& windowProps) = 0;

	/**
	 * @brief Initializes the renderer with the specified graphics API
	 * @param graphicsApi Graphics API to use for rendering
	 */
	void virtual _InitRenderer(Types::Render::GraphicsApi graphicsApi) = 0;

public:
	/**
	 * @brief Gets the singleton application instance
	 * @return I_Application& Reference to the application instance
	 * @details Returns the current application instance (singleton pattern)
	 */
	[[nodiscard]] static I_Application& StGet();

	/**
	 * @brief Checks if the application has any layers
	 * @return bool True if there are layers in the layer stack, false otherwise
	 */
	[[nodiscard]] bool HasLayers() const { return not _layerStack.Empty(); }

	/**
	 * @brief Gets the number of layers in the stack
	 * @return size_t Number of layers in the stack
	 */
	[[nodiscard]] size_t LayersSize() const { return _layerStack.Size(); }

	/**
	 * @brief Gets the application's window
	 * @return Window::I_Window* Pointer to the window
	 */
	[[nodiscard]] virtual Window::I_Window& GetWindow() const = 0;

	/**
	 * @brief Gets the application's graphics context
	 * @return Render::Context::I_Context* Pointer to the graphics context
	 */
	[[nodiscard]] virtual Render::Context::I_Context& GetRenderContext() const = 0;

	[[nodiscard]] bool IsRunning() const { return _isRunning.load(); }

	void SetRunning(const bool running) { _isRunning.store(running); }

	/**
	 * @brief Sets the ImGui layer for the application
	 * @param imguiLayer Pointer to the ImGui layer to set
	 */
	virtual void SetImGuiLayer(Layers::I_Layer* imguiLayer) = 0;

	/**
	 * @brief Removes the ImGui layer from the application
	 */
	virtual void RemoveImGuiLayer() = 0;

protected:
	static std::atomic<I_Application*> _stInstance; ///< Singleton application instance

	std::atomic<bool> _isRunning; ///< Flag indicating if the application is running
	Layers::LayerStack _layerStack; ///< Stack of layers and overlays
};

std::unique_ptr<I_Application> CreateApplication(int argc, const char* argv[]);

}

}

#endif //CE_CORE_APPLICATION_I_APPLICATION_HPP
