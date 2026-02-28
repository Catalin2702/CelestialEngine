//
// Module: Window/Platforms/Universal
// File: OpenGLViewport.hpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#pragma once

#ifndef CE_WINDOW_WINDOWS_WINDOWSVIEWPORT_HPP
#define CE_WINDOW_WINDOWS_WINDOWSVIEWPORT_HPP

#include "Window/I_Viewport.hpp"

#include "Types/Window/WindowDestructor.hpp"
#include "Types/Window/WindowProps.hpp"


struct GLFWwindow;

namespace TypeWindow = CE::Types::Window;

/**
 * @namespace CE::Window
 * @brief Window management and viewport implementations
 * @details Contains the I_Viewport interface and concrete implementations for different platforms.
 *          The OpenGLViewport class provides a cross-platform implementation using OpenGL and GLFW,
 *          while the MetalViewport class provides a macOS-specific implementation using Metal.
 */
namespace CE::Window {

/**
 * @class OpenGLViewport
 * @brief Cross-platform viewport implementation using OpenGL rendering
 * @details Concrete implementation of I_Viewport that uses GLFW for window management
 *          and OpenGL for rendering. This is the primary cross-platform implementation
 *          that works on Windows, Linux, and macOS. It provides a complete windowing
 *          system with event handling, VSync control, and OpenGL context management.
 */
class OpenGLViewport final: public I_Viewport {
public:
	/**
	 * @brief Constructor
	 * @param windowProps Window configuration properties (title, width, height, VSync)
	 * @details Creates and initializes an OpenGL-based viewport with the specified properties.
	 *          Calls _Init() to set up GLFW, create the window, and initialize OpenGL context.
	 */
	OpenGLViewport(const TypeWindow::WindowProps& windowProps);

	/**
	 * @brief Destructor
	 * @details Cleans up GLFW window resources by calling _Shutdown()
	 */
	~OpenGLViewport() override;

public:
	/**
	 * @brief Updates the viewport state each frame
	 * @details Polls all pending events via glfwPollEvents() and swaps the front/back
	 *          rendering buffers via glfwSwapBuffers() to display the rendered content
	 */
	void OnUpdate() override;

public:
	/**
	 * @brief Gets the current width of the viewport
	 * @return unsigned int Width in pixels
	 * @details Inline getter that returns the cached width value from _data
	 */
	[[nodiscard]] unsigned int GetWidth() const override { return _data.width; }

	/**
	 * @brief Gets the current height of the viewport
	 * @return unsigned int Height in pixels
	 * @details Inline getter that returns the cached height value from _data
	 */
	[[nodiscard]] unsigned int GetHeight() const override { return _data.height; }

	/**
	 * @brief Checks if VSync is enabled
	 * @return bool True if VSync is enabled, false otherwise
	 * @details Inline getter that returns the VSync state from _data
	 */
	[[nodiscard]] bool IsVSync() const override { return _data.VSync; }

	/**
	 * @brief Gets the underlying GLFW window pointer
	 * @return GLFWwindow* Raw pointer to the GLFW window
	 * @details Provides access to the GLFW window for platform-specific operations.
	 *          Returns the raw pointer from the smart pointer wrapper.
	 */
	[[nodiscard]] GLFWwindow* GetGLFWwindow() const override { return _glfwWindow.get(); }
	
	/**
	 * @brief Sets the event callback function
	 * @param callback Function to be called when events occur
	 * @details Stores the callback function that will be invoked for all window and
	 *          input events (resize, close, keyboard, mouse, etc.)
	 */
	void SetEventCallback(const EventCallbackFn& callback) override;

	/**
	 * @brief Configures all GLFW window event callbacks
	 * @details Sets up GLFW callbacks for:
	 *          - Window resize events
	 *          - Window close events
	 *          - Keyboard input (press, release, repeat, typed characters)
	 *          - Mouse button events (press, release)
	 *          - Mouse movement events
	 *          - Mouse scroll events
	 *          All callbacks generate appropriate engine events and invoke the event callback
	 */
	void SetWindowCallbacks() override;

	/**
	 * @brief Sets the viewport width
	 * @param width New width in pixels
	 * @details Updates the cached width value in _data. Note: this only updates
	 *          the stored value, the actual window resize is handled by GLFW events
	 */
	void SetWidth(unsigned int width) override;

	/**
	 * @brief Sets the viewport height
	 * @param height New height in pixels
	 * @details Updates the cached height value in _data. Note: this only updates
	 *          the stored value, the actual window resize is handled by GLFW events
	 */
	void SetHeight(unsigned int height) override;

	/**
	 * @brief Enables or disables vertical synchronization (VSync)
	 * @param enabled True to enable VSync, false to disable
	 * @details When enabled, limits the frame rate to the monitor's refresh rate to
	 *          prevent screen tearing. Uses glfwSwapInterval(1) to enable and
	 *          glfwSwapInterval(0) to disable. If GLFW is not initialized, prints
	 *          a warning and returns without making changes.
	 */
	void SetVSync(bool enabled) override;

private:
	/**
	 * @brief Initializes the OpenGL viewport
	 * @details Performs complete viewport initialization:
	 *          1. Logs window creation information
	 *          2. Initializes GLFW if not already initialized (with error callback)
	 *          3. Configures OpenGL context (version 4.1 Core Profile, forward compatible)
	 *          4. Creates the GLFW window with specified dimensions
	 *          5. Makes the OpenGL context current
	 *          6. Initializes GLAD to load OpenGL function pointers
	 *          7. Associates window data with GLFW user pointer
	 *          8. Sets VSync according to configuration
	 *          9. Registers all event callbacks
	 *          Exits with EXIT_FAILURE if GLAD initialization fails
	 */
	void _Init();

	/**
	 * @brief Cleans up and releases viewport resources
	 * @details Resets the GLFW window smart pointer, which automatically destroys
	 *          the GLFW window when there are no more references to it
	 */
	void _Shutdown();

private:
	TypeWindow::GLFWwindowPtr _glfwWindow = nullptr;	///< Smart pointer managing the GLFW window lifetime
	EventWindowData _data;								///< Window data including dimensions, VSync state, and event callback
};

}

#endif //CE_WINDOW_WINDOWS_WINDOWSVIEWPORT_HPP
