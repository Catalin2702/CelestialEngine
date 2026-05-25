//
// Module: CelestialEngine/Engine/Modules/Window/Platforms/Common/Glfw
// File: GlfwWindow.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-05-25
//

#pragma once

#ifndef CE_WINDOW_COMMON_GLFWWINDOW_HPP
#define CE_WINDOW_COMMON_GLFWWINDOW_HPP

#include "Core/Window/I_Window.hpp"

#include "Define/DynamicLinker.hpp"
#include "Define/Window.hpp"
#include "Types/Window/WindowDestructor.hpp"
#include "Types/Window/WindowProps.hpp"

#include <utility>

struct GLFWwindow;

namespace TypeWindow = CE::Types::Window;

/**
 * @namespace CE::Core::Window
 * @brief Window management and window implementations
 * @details Contains the I_Window interface and concrete implementations for different platforms.
 *			The OpenGLWindow class provides a cross-platform implementation using OpenGL and GLFW,
 *			while the MetalWindow class provides a macOS-specific implementation using Metal.
 */
namespace CE::Core::Window {

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

	/**
	 * @brief Prepares the window for rendering
	 * @param VSync True to enable VSync, false to disable
	 * @details This method can be used to perform any necessary setup before the rendering loop starts.
	 *			For example, it can be used to ensure that the OpenGL context is properly configured and ready for rendering.
	 *			In this implementation, it sets the VSync state according to the configuration in _data.
	 */
	void GetReady(bool VSync) override;

public:
	/**
	 * @brief Gets the current size of the window
	 * @return std::pair<unsigned int, unsigned int> Pair of width and height in pixels
	 */
	[[nodiscard]] std::pair<float, float> GetWindowSize() const override;

	/**
	 * @brief Gets the current size of the frame
	 * @return std::pair<float, float> Pair of frame width and height in pixels
	 * @details The frame size is the actual size of the window's drawable area, which may differ from the window size due to scaling or platform-specific behavior. This method returns the current frame width and height in pixels, which is important for rendering at the correct resolution.
	 */
	[[nodiscard]] std::pair<float, float> GetFrameSize() const override;

	/**
	 * @brief Checks if VSync is enabled
	 * @return bool True if VSync is enabled, false otherwise
	 * @details Inline getter that returns the VSync state from _data
	 */
	[[nodiscard]] bool IsVSync() const override;

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

public:
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

protected:
	/**
	 * @brief Initializes the OpenGL window
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
};

}

#endif //CE_WINDOW_COMMON_GLFWWINDOW_HPP
