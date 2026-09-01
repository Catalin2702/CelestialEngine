//
// Module: CelestialEngine/Engine/Core/Modules/Window
// File: I_Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_WINDOW_I_WINDOW_HPP
#define CE_CORE_WINDOW_I_WINDOW_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"
#include "Types/Window/WindowProps.hpp"

#include <string_view>
#include <utility>


namespace CE::Events {
class I_Event;
class WindowResizeEvent;
}

namespace CE::Core {

/**
 * @class I_Window
 * @brief Abstract interface for platform-specific window implementations
 * @details What a window can be asked and told, not how it is built: creating one is the concrete class's business,
 *			and a two-phase Init here would force every backend into a shape some of them do not have (a Wayland
 *			surface comes up through a sequence of asynchronous round-trips with the compositor, not a single call).
 *			A constructed window is a usable window.
 *
 *			Draining the event queue is likewise absent, because on no windowing API is it per-window: glfwPollEvents
 *			is process-wide, nextEventMatchingMask: belongs to the NSApplication, PeekMessage is per-thread and
 *			wl_display_dispatch_pending is per-connection. That call belongs to the platform and the run loop.
 */
class CE_CORE_API I_Window {
public:
	/**
	 * @brief Virtual destructor
	 * @details Ensures proper cleanup of derived window implementations
	 */
	virtual ~I_Window() = default;

public:
	/**
	 * @brief Makes the window visible and gives it input focus
	 * @details Separate from creation because the moment to reveal a window is the application's decision: the Cocoa
	 *			backend defers it until the run loop is up (applicationDidFinishLaunching), which is the canonical
	 *			lifecycle on that platform.
	 */
	virtual void Show() = 0;

	/**
	 * @brief Minimizes the window to the dock/taskbar
	 */
	virtual void Miniaturize() = 0;

	/**
	 * @brief Restores the window from the minimized state
	 * @details Not expressible on every backend: xdg-shell can minimize a surface but has no request to restore one,
	 *			so a future Wayland implementation will have to leave this a no-op.
	 */
	virtual void Deminiaturize() = 0;

	/**
	 * @brief Toggles the window between fullscreen and windowed mode
	 */
	virtual void ToggleFullScreen() = 0;

public:
	/**
	 * @brief Resizes the window
	 * @param width New width in screen coordinates
	 * @param height New height in screen coordinates
	 * @details Actually resizes it. The size reported back by GetWindowSize changes when the window system confirms
	 *			the change through a resize event, not here.
	 */
	virtual void SetWindowSize(u32 width, u32 height) = 0;

	/**
	 * @brief Sets the text shown in the window's title bar
	 */
	virtual void SetTitle(std::string_view title) = 0;

public:
	/**
	 * @brief Gets the current size of the window, in screen coordinates
	 * @return std::pair<u32, u32> Pair of width and height
	 * @details Screen coordinates, which are not pixels on a high-DPI display: what to render at is GetFrameSize.
	 *			Integer because no windowing API reports a fractional window size.
	 */
	[[nodiscard]] virtual std::pair<u32, u32> GetWindowSize() const = 0;

	/**
	 * @brief Gets the current size of the drawable, in backing pixels
	 * @return std::pair<u32, u32> Pair of width and height
	 * @details The real resolution to render at. Differs from GetWindowSize by exactly GetContentScale.
	 */
	[[nodiscard]] virtual std::pair<u32, u32> GetFrameSize() const = 0;

	/**
	 * @brief Gets the ratio between backing pixels and screen coordinates
	 * @return f32 1.0 on a standard display, 2.0 on a Retina one
	 * @details Needed wherever a size in points has to become a size in pixels - UI scaling, font sizes - and to tell
	 *			a genuine resize apart from the same window moving to a display with a different scale.
	 */
	[[nodiscard]] virtual f32 GetContentScale() const = 0;

	/**
	 * @brief Gets the refresh rate of the display the window is on
	 * @return u32 Refresh rate in Hz, or 0 when it cannot be determined
	 * @details Drives the frame limiter at the real display rate while VSync is on.
	 */
	[[nodiscard]] virtual u32 GetRefreshRate() const = 0;

	/**
	 * @brief Gets the underlying native window handle
	 * @return void* Pointer to the native window (GLFWwindow*, NS::Window*, ...)
	 * @details The escape hatch for platform-specific operations this interface does not cover. Note that it is one
	 *			handle: X11 and Wayland need two (Display* plus Window, wl_display* plus wl_surface*), so a backend
	 *			there will have to expose them through a surface interface instead.
	 */
	[[nodiscard]] virtual void* GetNativeWindow() const = 0;

	/**
	 * @brief Gets the window API type
	 * @return Types::WindowApi Enum value representing the window API
	 * @details Returns the specific window API type used by this window implementation (e.g., GLFW, Cocoa). This can be
	 *			used for platform-specific handling or optimizations.
	 */
	[[nodiscard]] virtual Types::WindowApi GetWindowApi() const = 0;
};

}

#endif //CE_CORE_WINDOW_I_WINDOW_HPP
