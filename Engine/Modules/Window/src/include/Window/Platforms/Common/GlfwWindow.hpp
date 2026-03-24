//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Common
// File: GlfwWindow.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-24
//

#pragma once

#ifndef CE_WINDOW_COMMON_GLFWWINDOW_HPP
#define CE_WINDOW_COMMON_GLFWWINDOW_HPP

#include "Window/I_Window.hpp"

#include "Define/DynamicLinker.hpp"
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
class CE_API GlfwWindow final: public I_Window {
public:
	/**
	 * @brief Constructor
	 * @param windowProps Window configuration properties (title, width, height, VSync)
	 * @details Creates and initializes an OpenGL-based window with the specified properties.
	 *			Calls _Init() to set up GLFW, create the window, and initialize OpenGL context.
	 */
	explicit GlfwWindow(TypeWindow::WindowProps windowProps);

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
	[[nodiscard]] float GetWidth() const override;

	/**
	 * @brief Gets the current height of the window
	 * @return unsigned int Height in pixels
	 * @details Inline getter that returns the cached height value from _data
	 */
	[[nodiscard]] float GetHeight() const override;

	/**
	 * @brief Gets the current size of the window
	 * @return std::pair<unsigned int, unsigned int> Pair of width and height in pixels
	 */
	[[nodiscard]] std::pair<float, float> GetSize() const override;

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

	/**
	 * @brief Gets the underlying GLFW window pointer
	 * @return GLFWwindow* Raw pointer to the GLFW window
	 * @details Provides access to the GLFW window for platform-specific operations.
	 *			Returns the raw pointer from the smart pointer wrapper.
	 */
	[[nodiscard]] GLFWwindow* GetGlfwWindow() const { return _glfwWindow.get(); }

	/**
	 * @brief Gets the content scale factors for the window
	 * @return std::pair<float, float> Content scale factors for X and Y axes
	 */
	[[nodiscard]] std::pair<float, float> GetContentScale() const override;

	/**
	 * @brief Gets the content size of the window
	 * @return std::pair<float, float> Content size in pixels for width and height
	 * @details This method retrieves the current content scale factors and multiplies them by the window's width and height to calculate the actual content size in pixels. This is useful for rendering and UI layout calculations that need to account for high-DPI displays.
	 */
	[[nodiscard]] std::pair<float, float> GetContentSize() const override;

	/**
	 * @brief Gets the framebuffer size of the window
	 * @return std::pair<int, int> Framebuffer width and height in pixels
	 * @details Retrieves the actual size of the framebuffer, which may differ from the window size due to scaling. This is important for rendering at the correct resolution, especially on high-DPI displays.
	 */
	[[nodiscard]] std::pair<int, int> GetFrameBufferSize() const;

	/**
	 * @brief Gets the current time since GLFW was initialized
	 * @return double Time
	 */
	[[nodiscard]] static double GetTime() ;

	/**
	 * @brief Sets the event callback function
	 * @param callback Function to be called when events occur
	 * @details Stores the callback function that will be invoked for all window and
	 *			input events (resize, close, keyboard, mouse, etc.)
	 */
	void SetEventCallback(const EventCallbackFn& callback) override;

	/**
	 * @brief Sets the resize event callback function
	 * @details The callback will be invoked with the new content scale when the window is resized, allowing the application to adjust rendering or UI layout based on the new size.
	 *			Note: GLFW handles window resizing through its own callbacks, so this function may not be used in this implementation.
	 */
	void SetContentScaleCallback(const ContentSizeCallbackFn&) override {};

	/**
	 * @brief Sets the VSync state change callback function
	 * @details The callback will be invoked with the new VSync state when it is changed, allowing the application to respond to VSync changes.
	 *			Note: VSync changes are typically handled directly in the SetVSync method, so this function may not be used in this implementation.
	 */
	void SetVSyncCallback(const VSyncCallbackFn&) override {};

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
	 * @brief Sets the window size
	 * @param width New width in pixels
	 * @param height New height in pixels
	 * @details Convenience method to set both width and height at once. Updates the cached width and height values in _data. Note: this only updates
	 *			the stored values, the actual window resize is handled by GLFW events
	 */
	void SetSize(unsigned int width, unsigned int height) override;

	/**
	 * @brief Enables or disables vertical synchronization (VSync)
	 * @param enabled True to enable VSync, false to disable
	 * @details When enabled, limits the frame rate to the monitor's refresh rate to
	 *			prevent screen tearing. Uses glfwSwapInterval(1) to enable and
	 *			glfwSwapInterval(0) to disable. If GLFW is not initialized, prints
	 *			a warning and returns without making changes.
	 */
	void SetVSync(bool enabled) override;

	/**
	 * @brief Prepares the window for rendering
	 * @details This method can be used to perform any necessary setup before the rendering loop starts.
	 *			For example, it can be used to ensure that the OpenGL context is properly configured and ready for rendering.
	 *			In this implementation, it sets the VSync state according to the configuration in _data.
	 */
	void SetCurrentContext(GLFWwindow* window = nullptr) const;

	void GetReady() override;

protected:
	/**
	 * @brief Sets internal callbacks for I/O events
	 * @details Registers GLFW callbacks for keyboard input, mouse movement, mouse button presses/releases, and scroll events.
	 *			Each callback retrieves the user data pointer (which points to the _callbacks structure) and invokes the appropriate event callback function with the corresponding event data.
	 */
	void _SetIOEventCallbacks() override;

	/**
	 * @brief Sets internal callbacks for window events
	 * @details Registers GLFW callbacks for window resizing and closing events. Each callback retrieves the user data pointer (which points to the _callbacks structure) and invokes the appropriate event callback function with the corresponding event data.
	 */
	void _SetWindowEventCallbacks() override;

	/**
	 * @brief Sets internal callbacks for window events
	 * @details Stores the provided callbacks in the _data structure, which will be
	 *			invoked by the GLFW event callbacks registered in _SetWindowCallbacks()
	 */
	void _SetInternalCallbacks() override;

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
	void _Init() override;

	/**
	 * @brief Initializes the GLFW window and OpenGL context
	 * @details This method is called by _Init() to perform the actual creation of the GLFW window and setup of the OpenGL context. It handles GLFW initialization, window creation, OpenGL context configuration, and error handling. If any step fails (e.g., GLFW initialization, window creation, GLAD initialization), it logs an error message and throws a runtime exception.
	 */
	void _InitWindow() override;

	/**
	 * @brief Cleans up and releases window resources
	 * @details Resets the GLFW window smart pointer, which automatically destroys
	 *			the GLFW window when there are no more references to it
	 */
	void _Shutdown() override;

public:
	WINDOW_API_TYPE(GLFW)

private:
	TypeWindow::GLFWwindowPtr _glfwWindow = nullptr;	///< Smart pointer managing the GLFW window lifetime
	Types::Window::WindowProps _data;								///< Window data including dimensions, VSync state, and event callback
};

}

#endif //CE_WINDOW_COMMON_GLFWWINDOW_HPP
