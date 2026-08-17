//
// Module: CelestialEngine/Engine/Modules/Core/Application
// File: I_Application.hpp
// Created by: Catalin Chirosca
// Created: 2026-04-18
// Updated by: Catalin Chirosca
// Updated: 2026-08-18
//

#pragma once

#ifndef CE_CORE_APPLICATION_I_APPLICATION_HPP
#define CE_CORE_APPLICATION_I_APPLICATION_HPP

#include "Core/Layers/LayerStack.hpp"
#include "Define/DynamicLinker.hpp"

#include <atomic>
#include <chrono>
#include <memory>


namespace CE {
namespace Events {
	class I_Event;
}

namespace Core {
	class I_Layer;
	class I_Context;
	class I_Window;
}

namespace Types {
	struct WindowProps;
	enum class GraphicsApi: uint8_t;
}

}

namespace CE::Core {

/**
 * @class I_Application
 * @brief Interface for the core application class
 * @details Defines the main application lifecycle methods, event handling, and layer management functions.
 *			Client applications should implement this interface and provide a factory function to create an instance of their application.
 */
class CE_CORE_API I_Application {
public:
	using Clock = std::chrono::steady_clock;
	using TimePoint = std::chrono::time_point<Clock>;

public:
	/**
	 * @brief Virtual destructor
	 */
	virtual ~I_Application();

public:
	/**
	 * @brief Virtual method to start the application
	 */
	virtual void Start() = 0;

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
     * @brief Virtual method to initialize the application with window properties
     */
	virtual void Init() = 0;

	/**
	 * @brief Initializes the ImGui layer with the specified graphics API
	 */
	void virtual InitImGuiLayer() = 0;

	/**
	 * @brief Adds a layer to the layer stack
	 * @param layer Pointer to the layer to add
	 * @details Layers are updated and receive events in the order they are pushed
	 */
	void virtual PushLayer(I_Layer* layer);

	/**
	 * @brief Adds an overlay to the layer stack
	 * @param overlay Pointer to the overlay layer to add
	 * @details Overlays are rendered on top of regular layers and receive
	 *			events before regular layers
	 */
	void virtual PushOverlay(I_Layer* overlay);

	/**
	 * @brief Removes a layer from the layer stack
	 * @param layer Pointer to the layer to remove
	 * @details Delegates to LayerStack::PopLayer, which removes the layer
	 *			from the stack and calls its OnDetach method
	 */
	void virtual PopLayer(I_Layer* layer);

	/**
	 * @brief Removes an overlay from the layer stack
	 * @param overlay Pointer to the overlay layer to remove
	 * @details Delegates to LayerStack::PopOverlay, which removes the overlay
	 *			from the stack and calls its OnDetach method
	 */
	void virtual PopOverlay(I_Layer* overlay);

protected:
	/**
	 * @brief Initializes the application with the specified window properties
	 */
	void virtual _InitWindow() = 0;

	/**
	 * @brief Initializes the renderer with the specified graphics API
	 */
	void virtual _InitRenderer() = 0;

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
	[[nodiscard]] virtual I_Window& GetWindow() const = 0;

	/**
	 * @brief Gets the application's graphics context
	 * @return Render::Context::I_Context* Pointer to the graphics context
	 */
	[[nodiscard]] virtual I_Context& GetRenderContext() const = 0;

	/**
	 * @brief Checks whether the application loop is currently running
	 */
	[[nodiscard]] bool IsRunning() const { return _isRunning.load(); }

	/**
	 * @brief Removes the ImGui layer from the application
	 */
	virtual void RemoveImGuiLayer() = 0;

	/**
	 * @brief Gets the time elapsed since the previous frame, in seconds
	 * @details Also advances the internal frame timestamp: call once per frame.
	 */
	float GetDeltaTime() const;

	/**
	 * @brief Resets the frame timestamp so the next GetDeltaTime does not include a pause (e.g. startup)
	 */
	void ResetDeltaTime() const;

public:
	/**
	 * @brief Sets the running state of the application loop
	 */
	virtual void SetRunning(const bool running) { _isRunning.store(running); }

	/**
	 * @brief Sets the ImGui layer for the application
	 * @param imguiLayer Pointer to the ImGui layer to set
	 */
	virtual void SetImGuiLayer(I_Layer* imguiLayer) = 0;

	/**
	 * @brief Binds every raw platform source to the event hub's Receive* methods (backend-specific)
	 */
	virtual void SetEventHubDispatcher() = 0;
	/**
	 * @brief Subscribes the application-level handlers (input state first) to the event hub
	 */
	virtual void SubscribeToHubDispatcher() {}
	/**
	 * @brief Removes the application-level handlers from the event hub
	 */
	virtual void UnsubscribeFromDispatcher() {}

protected:
	static std::atomic<I_Application*> _stInstance; ///< Singleton application instance
	mutable std::atomic<TimePoint> _lastFrameTime = Clock::now(); ///< Timestamp of the last frame for delta time calculation

	std::atomic<bool> _isRunning = false; ///< Flag indicating if the application is running
	LayerStack _layerStack; ///< Stack of layers and overlays
};

std::unique_ptr<I_Application> CreateApplication(int argc, const char* argv[]);

}

#endif //CE_CORE_APPLICATION_I_APPLICATION_HPP
