//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Common/Glfw
// File: GlfwWindow.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-07-14
//

#pragma once

#ifndef CE_CORE_WINDOW_COMMON_GLFWWINDOW_HPP
#define CE_CORE_WINDOW_COMMON_GLFWWINDOW_HPP

#include "Core/Window/I_Window.hpp"

#include "Define/DynamicLinker.hpp"
#include "Define/Window.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Types/Window/Platforms/Common/Glfw/GlfwWindowDestructor.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <array>
#include <utility>

struct GLFWwindow;

namespace CE::Core {

class GlfwWindowEventHandler {
public:
	struct GlfwWindowStateEvents {
		UnicastEventDispatcher<int, int> onResizeDispatcher;
		UnicastEventDispatcher<> onCloseDispatcher;
		UnicastEventDispatcher<int, const char*> onErrorDispatcher;
	};

	struct GlfwKeyboardEvents {
		UnicastEventDispatcher<int, int, int, int> onKeyDispatcher;
		UnicastEventDispatcher<unsigned int> onCharDispatcher;
	};

	struct GlfwMouseEvents {
		UnicastEventDispatcher<int, int, int> onMouseButtonDispatcher;
		UnicastEventDispatcher<double, double> onMousePositionDispatcher;
		UnicastEventDispatcher<int, int, int, double, double> onMouseDraggedDispatcher;
		UnicastEventDispatcher<double, double> onMouseWheelScrollDispatcher;
	};

public:
	void DispatchResizeEvent(int width, int height) const;
	void DispatchCloseEvent() const;
	void DispatchErrorEvent(int errorCode, const char* description) const;

	void DispatchKeyEvent(int key, int action, int scancode, int mods) const;
	void DispatchCharEvent(unsigned int codepoint) const;

	void DispatchMouseButtonEvent(int button, int action, int mods) const;
	void DispatchMousePositionEvent(double xPos, double yPos) const;
	void DispatchMouseDraggedEvent(int button, int action, int mods, double xPos, double yPos) const;
	void DispatchMouseWheelScrollEvent(double xOffset, double yOffset) const;

public:
	GlfwWindowStateEvents windowStateEvents;
	GlfwKeyboardEvents keyboardEvents;
	GlfwMouseEvents mouseEvents;
};

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
	 * @details Creates and initializes a GLFW window using the properties specified in Config::Config
	 */
	explicit GlfwWindow();

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
	 * @brief Initializes the OpenGL window
	 */
	void Init() override;

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

protected:
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

public:
	GlfwWindowEventHandler windowEventHandler;

private:
	Types::GlfwWindowPtr _glfwWindow = nullptr;	///< Smart pointer managing the GLFW window lifetime

	/// @brief Tracks the pressed/released state of each mouse button (indexed by GLFW button code).
	/// @details GLFW has no native drag event, so a drag is synthesized when the cursor moves while a button is held.
	std::array<bool, 8> _pressedMouseButtons{};

	/// @brief Latest keyboard modifier flags reported by the mouse button callback, forwarded with synthesized drag events.
	int _lastMouseMods = 0;
};

}

#endif //CE_WINDOW_COMMON_GLFWWINDOW_HPP
