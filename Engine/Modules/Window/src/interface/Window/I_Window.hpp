//
// Module: CelestialEngine/Engine/Modules/Window
// File: I_Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-03-29
//

#pragma once

#ifndef CE_WINDOW_I_WINDOW_HPP
#define CE_WINDOW_I_WINDOW_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Window/WindowProps.hpp"
#include "Utility/Callback/Callback.hpp"

#include <memory>
#include <utility>

namespace CE::Events {
class I_Event;
class WindowResizeEvent;
}

/**
 * @namespace CE::Window
 * @brief Window management and window interfaces
 * @details Contains the I_Window interface and related type definitions for window management.
 *			This namespace defines the common interface for all window implementations,
 *			allowing for platform-specific implementations while maintaining a consistent API.
 */
namespace CE::Window {

using WindowResizeEventCallbackFn = Utility::CallbackFn<Events::WindowResizeEvent>;

struct WindowInternalCallbacks {
	WindowResizeEventCallbackFn ResizeEventCallback;	///< Callback function for handling window resize events
};

/**
 * @typedef EventCallbackFn
 * @brief Type alias for event callback functions
 * @details Function type for handling engine events
 */
using EventCallbackFn = Utility::CallbackFn<Events::I_Event>;

using ContentSizeCallbackFn = Utility::CallbackFn<const std::pair<float, float>>;
using VSyncCallbackFn = Utility::CallbackFn<const bool>;

struct WindowCallbacks {
	EventCallbackFn EventCallback;				///< Callback function for handling engine events
	ContentSizeCallbackFn ContentSizeCallback;	///< Callback function for handling window resize events
	VSyncCallbackFn VSyncCallback;				///< Callback function for handling VSync state changes

	WindowInternalCallbacks _internalCallbacks;
};

/**
 * @class I_Window
 * @brief Abstract interface for platform-specific window implementations
 * @details Defines the common interface that all window/window implementations must provide.
 *			Implementations include OpenGLWindow for cross-platform OpenGL windows and
 *			MetalWindow for macOS Metal-based windows.
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
	 *			and buffer swapping. Must be implemented by derived classes.
	 */
	virtual void OnUpdate() const = 0;

public:
	/**
	 * @brief Gets the current width of the window
	 * @return unsigned int Width in pixels
	 */
	[[nodiscard]] virtual float GetWindowWidth() const = 0;

	/**
	 * @brief Gets the current height of the window
	 * @return unsigned int Height in pixels
	 */
	[[nodiscard]] virtual float GetWindowHeight() const = 0;

	/**
	 * @brief Gets the current size of the window
	 * @return std::pair<float, float> Pair of width and height in pixels
	 * @details Returns the current width and height of the window in pixels.
	 *			This may differ from the initial properties due to resizing or platform-specific behavior.
	 *			The returned size represents the actual dimensions of the window.
	 */
	[[nodiscard]] virtual std::pair<float, float> GetWindowSize() const = 0;

	/**
	 * @brief Gets the current width of the view (content area) of the window
	 * @return float Current width in pixels
	 * @details Returns the width of the view or content area of the window, which may differ from the overall window dimensions due to platform-specific behavior. This is the actual drawable area for rendering.
	 */
	[[nodiscard]] virtual float GetViewWidth() const = 0;

	/**
	 * @brief Gets the current height of the view (content area) of the window
	 * @return float Current height in pixels
	 * @details Returns the height of the view or content area of the window, which may differ from the overall window dimensions due to platform-specific behavior. This is the actual drawable area for rendering.
	 */
	[[nodiscard]] virtual float GetViewHeight() const = 0;

	/**
	 * @brief Gets the current size of the view (content area) of the window
	 * @return std::pair<float, float> Pair of width and height in pixels
	 * @details Returns the current width and height of the view or content area of the window, which may differ from the overall window dimensions due to platform-specific behavior. This is the actual drawable area for rendering.
	 */
	[[nodiscard]] virtual std::pair<float, float> GetViewSize() const = 0;

	/**
	 * @brief Gets the current width of the monitor
	 * @return float Current monitor width in pixels
	 * @details Returns the width of the monitor that the window is currently on. This can be used for fullscreen mode or for scaling calculations.
	 */
	[[nodiscard]] virtual float GetMonitorWidth() const = 0;

	/**
	 * @brief Gets the current height of the monitor
	 * @return float Current monitor height in pixels
	 * @details Returns the height of the monitor that the window is currently on. This can be used for fullscreen mode or for scaling calculations.
	 */
	[[nodiscard]] virtual float GetMonitorHeight() const = 0;

	/**
	 * @brief Gets the current size of the monitor
	 * @return std::pair<float, float> Pair of monitor width and height in pixels
	 * @details Returns the width and height of the monitor that the window is currently on. This can be used for fullscreen mode or for scaling calculations.
	 */
	[[nodiscard]] virtual std::pair<float, float> GetMonitorSize() const = 0;

	/**
	 * @brief Gets the window API type
	 * @return Types::Window::WindowApi Enum value representing the window API
	 * @details Returns the specific window API type used by this window implementation (e.g., OpenGL, Metal, Cocoa). This can be used for platform-specific handling or optimizations.
	 */
	[[nodiscard]] virtual Types::Window::WindowApi GetWindowApi() const = 0;

	/**
	 * @brief Gets the content scale of the window
	 * @return std::pair<float, float> Pair of content scale factors for x and y axes
	 * @details The content scale represents the scaling factor applied to the window's content, which is important for rendering at the correct resolution on high-DPI displays. This method returns the current content scale factors for both axes.
	 */
	[[nodiscard]] virtual std::pair<float, float> GetContentScale() const = 0;

	/**
	 * @brief Gets the content size of the window
	 * @return std::pair<float, float> Pair of content width and height in pixels
	 * @details The content size represents the actual drawable area of the window, which may differ from the window's logical size due to scaling. This method returns the current content width and height in pixels, which is important for rendering at the correct resolution.
	 */
	[[nodiscard]] virtual std::pair<float, float> GetContentSize() const = 0;

	/**
	 * @brief Checks if VSync is enabled
	 * @return bool True if VSync is enabled, false otherwise
	 */
	[[nodiscard]] virtual bool IsVSync() const = 0;

	/**
	 * @brief Gets the underlying GLFW window pointer
	 * @return void* Pointer to the GLFW window
	 * @details Provides access to the GLFW window for platform-specific operations
	 */
	[[nodiscard]] virtual void* GetNativeWindow() const = 0;

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
	 * @brief Sets the window size
	 * @param width New width in pixels
	 * @param height New height in pixels
	 * @details Convenience method to set both width and height at once. Updates the internal values in the _data structure. Note: this only updates
	 *			the stored values, it does not actually resize the window
	 */
	virtual void SetSize(unsigned int width, unsigned int height) = 0;

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
	 *			of the specified type with the given properties
	 */
	template<std::derived_from<I_Window> T>
	static I_Window* NewWindow(const Types::Window::WindowProps& windowProps) {
		auto window = std::make_unique<T>(windowProps);
		return window.release();
	}

public:
	/**
	 * @brief Sets the event callback function
	 * @param callback Function to be called when events occur
	 * @details The callback will be invoked for all window and input events
	 */
	virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

	/**
	 * @brief Sets the resize event callback function
	 * @param callback Function to be called when the window is resized
	 * @details The callback will be invoked with the new content scale when the window is resized
	 */
	virtual void SetContentScaleCallback(const ContentSizeCallbackFn& callback) = 0;

	/**
	 * @brief Sets the VSync state change callback function
	 * @param callback Function to be called when the VSync state changes
	 * @details The callback will be invoked with the new VSync state when it is changed
	 */
	virtual void SetVSyncCallback(const VSyncCallbackFn& callback) = 0;

	/**
	 * @brief Prepares the window for rendering
	 * @details This method can be used to perform any necessary setup before the rendering loop starts.
	 *			For example, it can be used to set the callbacks to their initial states, ensure that the window is properly configured, or perform any other necessary initialization before rendering begins.
	 *			Derived classes can override this method to implement any specific setup required for their window implementation before the rendering loop starts.
	 */
	virtual void GetReady() = 0;

protected:
	/**
	 * @brief Sets internal callback functions for input/output events
	 * @details This method allows setting internal callbacks that are used by the window implementation
	 *			to handle specific events such as keyboard input, mouse movement, etc. The provided
	 *			callbacks will be stored internally and invoked when the corresponding events occur.
	 */
	virtual void _SetIOEventCallbacks() = 0;

	/**
	 * @brief Sets internal callback functions for window events
	 * @details This method allows setting internal callbacks that are used by the window implementation
	 *			to handle specific events such as window resizing. The provided callbacks will be stored
	 *			internally and invoked when the corresponding events occur.
	 */
	virtual void _SetWindowEventCallbacks() = 0;

	/**
	 * @brief Sets internal callback functions for window events
	 * @details This method allows setting internal callbacks that are used by the window implementation
	 *			to handle specific events such as window resizing. The provided callbacks will be stored
	 *			internally and invoked when the corresponding events occur.
	 */
	virtual void _SetInternalCallbacks() = 0;

protected:
	virtual void _Init() = 0;
	virtual void _InitWindow() = 0;
	virtual void _Shutdown() = 0;

	WindowCallbacks _callbacks;
};

}

#endif //CE_WINDOW_I_WINDOW_HPP
