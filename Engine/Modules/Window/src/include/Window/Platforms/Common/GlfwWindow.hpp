//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Common
// File: GlfwWindow.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-20
//

#pragma once

#ifndef CE_WINDOW_COMMON_GLFWWINDOW_HPP
#define CE_WINDOW_COMMON_GLFWWINDOW_HPP

#include "Window/I_Window.hpp"

#include "Define/Window.hpp"
#include "Types/Window/WindowDestructor.hpp"
#include "Types/Window/WindowProps.hpp"

#include <utility>

struct GLFWwindow;

namespace TypeWindow = CE::Types::Window;

/**
 * @namespace CE::Window
 * @brief Window management and window implementations
 * @details Contains the I_Window interface and concrete implementations for different platforms.
 *			The OpenGLWindow class provides a cross-platform implementation using OpenGL and GLFW,
 *			while the MetalWindow class provides a macOS-specific implementation using Metal.
 */
namespace CE::Window {

/**
 * @class GlfwWindow
 * @brief Cross-platform window implementation using GLFW
 * @details Concrete implementation of I_Window that uses GLFW for window management
 *			and OpenGL for rendering. This is the primary cross-platform implementation
 *			that works on Windows, Linux, and macOS. It provides a complete windowing
 *			system with event handling, VSync control, and OpenGL context management.
 */
class GlfwWindow final: public I_Window {
public:
	/**
	 * @brief Constructor
	 * @param windowProps Window configuration properties (title, width, height, VSync)
	 * @details Creates and initializes an OpenGL-based window with the specified properties.
	 *			Calls _Init() to set up GLFW, create the window, and initialize OpenGL context.
	 */
	GlfwWindow(const TypeWindow::WindowProps& windowProps);

	/**
	 * @brief Destructor
	 * @details Cleans up GLFW window resources by calling _Shutdown()
	 */
	~GlfwWindow() override;

public:
	/**
	 * @brief Updates the window state each frame
	 * @details Polls all pending events via glfwPollEvents() and swaps the front/back
	 *			rendering buffers via glfwSwapBuffers() to display the rendered content
	 */
	void OnUpdate() const override;

public:
	/**
	 * @brief Gets the current width of the window
	 * @return unsigned int Width in pixels
	 * @details Inline getter that returns the cached width value from _data
	 */
	[[nodiscard]] unsigned int GetWidth() const override { return _data.width; }

	/**
	 * @brief Gets the current height of the window
	 * @return unsigned int Height in pixels
	 * @details Inline getter that returns the cached height value from _data
	 */
	[[nodiscard]] unsigned int GetHeight() const override { return _data.height; }

	/**
	 * @brief Gets the current size of the window
	 * @return std::pair<unsigned int, unsigned int> Pair of width and height in pixels
	 */
	[[nodiscard]] std::pair<unsigned int, unsigned int> GetSize() const override { return {_data.width, _data.height}; }

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
	 *			Returns the raw pointer from the smart pointer wrapper.
	 */
	[[nodiscard]] void* GetNativeWindow() const override { return _glfwWindow.get(); }

	[[nodiscard]] std::pair<float, float> GetContentScale() const override;
	
	/**
	 * @brief Sets the event callback function
	 * @param callback Function to be called when events occur
	 * @details Stores the callback function that will be invoked for all window and
	 *			input events (resize, close, keyboard, mouse, etc.)
	 */
	void SetEventCallback(const EventCallbackFn& callback) override;

	/**
	 * @brief Configures all GLFW window event callbacks
	 * @details Sets up GLFW callbacks for:
	 *			- Window resize events
	 *			- Window close events
	 *			- Keyboard input (press, release, repeat, typed characters)
	 *			- Mouse button events (press, release)
	 *			- Mouse movement events
	 *			- Mouse scroll events
	 *			All callbacks generate appropriate engine events and invoke the event callback
	 */
	void SetWindowCallbacks() override;

	/**
	 * @brief Sets the window width
	 * @param width New width in pixels
	 * @details Updates the cached width value in _data. Note: this only updates
	 *			the stored value, the actual window resize is handled by GLFW events
	 */
	void SetWidth(unsigned int width) override;

	/**
	 * @brief Sets the window height
	 * @param height New height in pixels
	 * @details Updates the cached height value in _data. Note: this only updates
	 *			the stored value, the actual window resize is handled by GLFW events
	 */
	void SetHeight(unsigned int height) override;

	/**
	 * @brief Enables or disables vertical synchronization (VSync)
	 * @param enabled True to enable VSync, false to disable
	 * @details When enabled, limits the frame rate to the monitor's refresh rate to
	 *			prevent screen tearing. Uses glfwSwapInterval(1) to enable and
	 *			glfwSwapInterval(0) to disable. If GLFW is not initialized, prints
	 *			a warning and returns without making changes.
	 */
	void SetVSync(bool enabled) override;

	WINDOW_API_TYPE(GLFW)

private:
	/**
	 * @brief Initializes the OpenGL window
	 * @details Performs complete window initialization:
	 *			1. Logs window creation information
	 *			2. Initializes GLFW if not already initialized (with error callback)
	 *			3. Configures OpenGL context (version 4.1 Core Profile, forward compatible)
	 *			4. Creates the GLFW window with specified dimensions
	 *			5. Makes the OpenGL context current
	 *			6. Initializes GLAD to load OpenGL function pointers
	 *			7. Associates window data with GLFW user pointer
	 *			8. Sets VSync according to configuration
	 *			9. Registers all event callbacks
	 *			Exits with EXIT_FAILURE if GLAD initialization fails
	 */
	void _Init();

	/**
	 * @brief Cleans up and releases window resources
	 * @details Resets the GLFW window smart pointer, which automatically destroys
	 *			the GLFW window when there are no more references to it
	 */
	void _Shutdown();

private:
	TypeWindow::GLFWwindowPtr _glfwWindow = nullptr;	///< Smart pointer managing the GLFW window lifetime
	EventWindowData _data;								///< Window data including dimensions, VSync state, and event callback
};

}

#endif //CE_WINDOW_COMMON_GLFWWINDOW_HPP
