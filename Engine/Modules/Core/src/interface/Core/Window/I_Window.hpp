//
// Module: CelestialEngine/Engine/Core/Modules/Window
// File: I_Window.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-09-09
//

#pragma once

#ifndef CE_CORE_WINDOW_I_WINDOW_HPP
#define CE_CORE_WINDOW_I_WINDOW_HPP

#include "Define/DynamicLinker.hpp"
#include "Types/Var/Vars.hpp"
#include "Types/Window/WindowProps.hpp"

#include <memory>
#include <string_view>
#include <utility>


namespace CE::Events {
class I_Event;
class WindowResizeEvent;
}

namespace CE::Core {

class I_EventHubDispatcher;
class I_Platform;

/**
 * @class I_Window
 * @brief Abstract interface for platform-specific window implementations
 * @details What a window can be asked and told, not how it is built: creating one is the concrete class's business,
 *			and a two-phase Init here would force every backend into a shape some of them do not have (a Wayland
 *			surface comes up through a sequence of asynchronous round-trips with the compositor, not a single call).
 *			A constructed window is a usable window.
 *
 *			Draining the event queue is likewise absent, because on no windowing API is it per-window: it is
 *			process-wide, per-thread or per-connection depending on the system, but never per-window. That call
 *			belongs to the platform and the run loop.
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
	 * @brief Creates a fully initialized window on the given windowing backend
	 * @param platform The live windowing library, which must outlive the returned window
	 * @return std::unique_ptr<I_Window> The window, ready to be shown
	 * @details Taking the platform by reference rather than looking one up says the dependency out loud: a window
	 *			cannot exist before the library that hosts it, and cannot outlive it either.
	 */
	[[nodiscard]] static std::unique_ptr<I_Window> MakeWindow(I_Platform& platform);

public:
	/**
	 * @brief Makes the window visible and gives it input focus
	 * @details Separate from creation because the moment to reveal a window is the application's decision, and some
	 *			backends cannot show one until their run loop is up.
	 */
	/**
	 * @brief Brings the native window up
	 * @details Empty by default because most backends have nothing to do here: they build a usable window in their
	 *			constructor. The exception is a backend that hands out no usable window until its platform has finished
	 *			launching, so the step is named, and called from the platform's ready handler, rather than hidden inside
	 *			a constructor that would run too early there.
	 */
	virtual void Init() {}

	virtual void Show() = 0;

	/**
	 * @brief Routes this window's raw callbacks into the event hub
	 * @details See I_Platform::ConnectToEventHub: each backend does its own wiring, because it is the only one that
	 *			knows what shape its native callbacks have.
	 */
	virtual void ConnectToEventHub(I_EventHubDispatcher& eventHub) = 0;

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
	 * @return f32 1.0 on a standard display, 2.0 on a high-DPI one
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
	 * @return void* Pointer to the backend's own native window object
	 * @details The escape hatch for platform-specific operations this interface does not cover. Note that it is one
	 *			handle, and some windowing systems need two - a display connection plus a surface - so a backend there
	 *			will have to expose them through a surface interface instead.
	 */
	[[nodiscard]] virtual void* GetNativeWindow() const = 0;

	/**
	 * @brief Gets the window API type
	 * @return Types::WindowApi Enum value representing the window API
	 * @details Returns the specific window API this implementation is built on, for the rare caller that has to handle
	 *			one of them specially.
	 */
	[[nodiscard]] virtual Types::WindowApi GetWindowApi() const = 0;
};

template<Types::WindowApi Api>
class I_WindowBase: public I_Window {
public:
	[[nodiscard]] Types::WindowApi GetWindowApi() const override { return Api; }
};

}

#endif //CE_CORE_WINDOW_I_WINDOW_HPP
