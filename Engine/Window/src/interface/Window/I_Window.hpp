//
// Module: Window
// File: I_Window.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_WINDOW_I_WINDOW_HPP
#define CE_WINDOW_I_WINDOW_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"

#include <memory>

struct GLFWwindow;


namespace CE::Events {
class I_Event;
}

/**
 * @namespace CE::Window
 * @brief Window management and window interfaces
 * @details Contains the I_Window interface and related type definitions for window management.
 *          This namespace defines the common interface for all window implementations,
 *          allowing for platform-specific implementations while maintaining a consistent API.
 */
namespace CE::Window {

/**
 * @typedef EventWindowData
 * @brief Type alias for window data with event handling
 * @details Specialization of WindowData for engine events
 */
using EventWindowData = Types::Window::WindowData<Events::I_Event>;

/**
 * @typedef EventCallbackFn
 * @brief Type alias for event callback functions
 * @details Function type for handling engine events
 */
using EventCallbackFn = Types::Window::CallbackFn<Events::I_Event>;

/**
 * @class I_Window
 * @brief Abstract interface for platform-specific window implementations
 * @details Defines the common interface that all window/window implementations must provide.
 *          Implementations include OpenGLWindow for cross-platform OpenGL windows and
 *          MetalWindow for macOS Metal-based windows.
 */
class CE_API I_Window {
public:
	/**
	 * @brief Virtual destructor
	 * @details Ensures proper cleanup of derived window implementations
	 */
	virtual ~I_Window() = default;

public:
	/**
	 * @brief Updates the window state each frame
	 * @details Pure virtual method that handles per-frame updates, event polling,
	 *          and buffer swapping. Must be implemented by derived classes.
	 */
	virtual void OnUpdate() = 0;

public:
	/**
	 * @brief Gets the current width of the window
	 * @return unsigned int Width in pixels
	 */
	[[nodiscard]] virtual unsigned int GetWidth() const = 0;

	/**
	 * @brief Gets the current height of the window
	 * @return unsigned int Height in pixels
	 */
	[[nodiscard]] virtual unsigned int GetHeight() const = 0;

	/**
	 * @brief Checks if VSync is enabled
	 * @return bool True if VSync is enabled, false otherwise
	 */
	[[nodiscard]] virtual bool IsVSync() const = 0;

	/**
	 * @brief Gets the underlying GLFW window pointer
	 * @return GLFWwindow* Pointer to the GLFW window
	 * @details Provides access to the GLFW window for platform-specific operations
	 */
	[[nodiscard]] virtual GLFWwindow* GetGLFWwindow() const = 0;

	/**
	 * @brief Sets the event callback function
	 * @param callback Function to be called when events occur
	 * @details The callback will be invoked for all window and input events
	 */
	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

	/**
	 * @brief Configures all window event callbacks
	 * @details Sets up GLFW callbacks for resize, close, keyboard, mouse, etc.
	 */
	virtual void SetWindowCallbacks() = 0;

	/**
	 * @brief Sets the window width
	 * @param width New width in pixels
	 */
	virtual void SetWidth(unsigned int width) = 0;

	/**
	 * @brief Sets the window height
	 * @param height New height in pixels
	 */
	virtual void SetHeight(unsigned int height) = 0;

	/**
	 * @brief Enables or disables VSync
	 * @param enabled True to enable VSync, false to disable
	 * @details VSync synchronizes rendering with the monitor's refresh rate
	 */
	virtual void SetVSync(bool enabled) = 0;

	/**
	 * @brief Factory method to create a window of a specific type
	 * @tparam T Concrete window type (must derive from I_Window)
	 * @param windowProps Window configuration properties
	 * @return I_Window* Pointer to the created window
	 * @details Template factory method that creates and returns a window
	 *          of the specified type with the given properties
	 */
	template<std::derived_from<I_Window> T>
	static I_Window* CreateWindow(const Types::Window::WindowProps& windowProps) {
		auto window = std::make_unique<T>(windowProps);
		return window.release();
	}
};

}

#endif // CE_WINDOW_I_WINDOW_HPP
