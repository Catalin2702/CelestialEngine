//
// Module: CelestialEngine/Engine/Modules/Core/Window/Platforms/Common/Glfw
// File: GlfwWindow.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-17
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_WINDOW_COMMON_GLFWWINDOW_HPP
#define CE_CORE_WINDOW_COMMON_GLFWWINDOW_HPP

#include "Core/Window/I_Window.hpp"
#include "Core/Window/Platforms/Common/Glfw/GlfwPlatform.hpp"
#include "Core/Render/Surface/I_OpenGlSurface.hpp"
#include "Define/DynamicLinker.hpp"
#include "Types/Types.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <source_location>
#include <string_view>
#include <utility>


struct GLFWwindow;

namespace CE::Core {

class GlfwWindowEventHandler {
public:
	struct GlfwWindowStateEvents {
		UnicastDispatcher<int, int> onResizeDispatcher;
		UnicastDispatcher<> onCloseDispatcher;
		UnicastDispatcher<int> onFocusDispatcher;	///< Fired by the GLFW focus callback with GLFW_TRUE/GLFW_FALSE
	};

	struct GlfwKeyboardEvents {
		UnicastDispatcher<int, int, int, int> onKeyDispatcher;
		UnicastDispatcher<unsigned int> onCharDispatcher;
	};

	struct GlfwMouseEvents {
		UnicastDispatcher<int, int, int> onMouseButtonDispatcher;
		UnicastDispatcher<f64, f64> onMousePositionDispatcher;
		UnicastDispatcher<int, int, int, f64, f64> onMouseDraggedDispatcher;
		UnicastDispatcher<f64, f64> onMouseWheelScrollDispatcher;
	};

public:
	/**
	 * @brief Forwards the resize callback to the bound listener
	 */
	void DispatchResizeEvent(int width, int height) const;
	/**
	 * @brief Forwards the close callback to the bound listener
	 */
	void DispatchCloseEvent() const;
	/**
	 * @brief Forwards a window focus change to the bound listener (the event hub)
	 * @param focused GLFW_TRUE when the window gained input focus, GLFW_FALSE when it lost it
	 */
	void DispatchFocusEvent(int focused) const;

	/**
	 * @brief Forwards the key callback to the bound listener
	 */
	void DispatchKeyEvent(int key, int action, int scancode, int mods) const;
	/**
	 * @brief Forwards the char callback to the bound listener
	 */
	void DispatchCharEvent(unsigned int codepoint) const;

	/**
	 * @brief Forwards the mouse button callback to the bound listener
	 */
	void DispatchMouseButtonEvent(int button, int action, int mods) const;
	/**
	 * @brief Forwards the mouse position callback to the bound listener
	 */
	void DispatchMousePositionEvent(f64 xPos, f64 yPos) const;
	/**
	 * @brief Forwards the mouse dragged callback to the bound listener
	 */
	void DispatchMouseDraggedEvent(int button, int action, int mods, f64 xPos, f64 yPos) const;
	/**
	 * @brief Forwards the mouse wheel scroll callback to the bound listener
	 */
	void DispatchMouseWheelScrollEvent(f64 xOffset, f64 yOffset) const;

public:
	GlfwWindowStateEvents windowStateEvents;
	GlfwKeyboardEvents keyboardEvents;
	GlfwMouseEvents mouseEvents;
};

/**
 * @class GlfwWindow
 * @brief Cross-platform window implementation using GLFW
 * @details Concrete implementation of I_Window that uses GLFW for window management, and of I_OpenGlSurface so an
 *			OpenGL swapchain can present into it. Works on Windows, Linux and macOS.
 *
 *			It owns one native window and nothing else: the library's own lifetime, its event queue and its window
 *			hints belong to GlfwPlatform, which must outlive every window built from it.
 */
class CE_CORE_API GlfwWindow final: public I_WindowBase<Types::WindowApi::GLFW>, public I_OpenGlSurface {
public:
	/**
	 * @brief Creates the native window, fully initialized
	 * @param platform The live GLFW library, which applies the window hints and must outlive this window
	 * @details Taking the platform by reference is what makes the ordering requirement a compile-time one: a window
	 *			cannot be built before the library that hosts it.
	 */
	explicit GlfwWindow(const GlfwPlatform& platform);

	GlfwWindow(const GlfwWindow&) = delete;

	/**
	 * @brief Move constructor
	 * @details GLFW keeps the address of the owning GlfwWindow in the native window's user pointer (see _InitWindow), and
	 *			every C callback recovers `this` from it. Moving the object changes that address, so the move re-registers
	 *			the user pointer on the new location: a defaulted move would leave every GLFW callback pointing at the
	 *			moved-from husk. The moved-from window keeps a null native handle and becomes inert.
	 */
	GlfwWindow(GlfwWindow&& other) noexcept;

	/**
	 * @brief Destructor
	 * @details Destroys the native window. Terminating GLFW is GlfwPlatform's job, not this one's.
	 */
	~GlfwWindow() override;

public:
	GlfwWindow& operator = (const GlfwWindow&) = delete;

	/**
	 * @brief Move assignment
	 * @details Releases the window currently held (if any), then adopts other's native handle and re-registers the GLFW
	 *			user pointer on this object. See the move constructor for why that re-registration is mandatory.
	 */
	GlfwWindow& operator = (GlfwWindow&& other) noexcept;

public:
	/**
	 * @brief Makes the window visible and gives it input focus
	 */
	void Show() override;

	/**
	 * @brief Minimizes the window via glfwIconifyWindow
	 */
	void Miniaturize() override;

	/**
	 * @brief Restores the window via glfwRestoreWindow
	 */
	void Deminiaturize() override;

	/**
	 * @brief Toggles fullscreen by moving the window on/off the monitor video mode
	 */
	void ToggleFullScreen() override;

	/**
	 * @brief Makes this window's OpenGL context current on the calling thread
	 */
	void MakeContextCurrent() override;

	/**
	 * @brief Presents the back buffer
	 */
	void SwapBuffers() override;

public:
	/**
	 * @brief Resizes the native window
	 */
	void SetWindowSize(u32 width, u32 height) override;

	/**
	 * @brief Sets the text shown in the window's title bar
	 */
	void SetTitle(std::string_view title) override;

	/**
	 * @brief Sets the swap interval on this window's context
	 * @details Makes the context current first: glfwSwapInterval takes no window and applies to whichever context is
	 *			current on the calling thread, so without that the setting could land on another window entirely.
	 */
	void SetSwapInterval(i32 interval) override;

public:
	/**
	 * @brief Gets the size of the window, in screen coordinates
	 * @details Served from the value the size callback cached, not by asking GLFW: the query is a round-trip into the
	 *			window system (a synchronous request to the X server, on X11) and this is read every frame.
	 */
	[[nodiscard]] std::pair<u32, u32> GetWindowSize() const override { return {_width, _height}; }

	/**
	 * @brief Gets the size of the drawable, in backing pixels
	 * @details Differs from GetWindowSize by the content scale on a high-DPI display. Cached like it, from the
	 *			framebuffer size callback.
	 */
	[[nodiscard]] std::pair<u32, u32> GetFrameSize() const override { return {_frameWidth, _frameHeight}; }

	/**
	 * @brief Gets the ratio between backing pixels and screen coordinates
	 */
	[[nodiscard]] f32 GetContentScale() const override;

	/**
	 * @brief Gets the refresh rate of the monitor the window is on
	 * @return u32 Refresh rate in Hz, or 0 if it cannot be determined
	 * @details Returns the video mode refresh rate of the window's monitor when fullscreen, otherwise the primary monitor.
	 *			Used to drive the frame limiter at the real display rate while VSync is on.
	 */
	[[nodiscard]] u32 GetRefreshRate() const override;

	/**
	 * @brief Gets the underlying GLFW window pointer
	 * @return void* Raw pointer to the GLFW window
	 * @details The escape hatch for platform-specific operations this interface does not cover.
	 */
	[[nodiscard]] void* GetNativeWindow() const override { return _glfwWindow.get(); }

	/**
	 * @brief Gets the underlying GLFW window pointer, typed
	 */
	[[nodiscard]] GLFWwindow* GetGlfwWindow() const { return _glfwWindow.get(); }

public:
	/**
	 * @brief Gets the size of the drawable, in backing pixels
	 * @details The same measurement I_Window already exposes as GetFrameSize; named differently on I_OpenGlSurface
	 *			because that is what the swapchain calls it.
	 */
	[[nodiscard]] std::pair<u32, u32> GetDrawableSize() const override { return GetFrameSize(); }

	/**
	 * @brief Gets the colour format of the default framebuffer
	 * @details GLFW creates it as 8 bits per channel in RGBA order and offers no way to ask for anything else, so this
	 *			is fixed rather than queried.
	 */
	[[nodiscard]] Types::PixelFormat GetColorFormat() const override { return Types::PixelFormat::RGBA8Unorm; }

private:
	/**
	 * @brief Creates the native window and registers every callback on it
	 */
	void _InitWindow(const GlfwPlatform& platform);

	/**
	 * @brief Destroys the native window, if this object still owns one
	 */
	void _Shutdown();

	/**
	 * @brief Points the native window's GLFW user pointer at this object
	 * @details Called on creation and again after every move: the GLFW C callbacks resolve their GlfwWindow through
	 *			glfwGetWindowUserPointer, so the pointer must always name the object that currently owns the handle.
	 */
	void _AdoptNativeWindow();

	/**
	 * @brief Gets the native handle, or null with a warning when the window is not initialized
	 * @details Every call into GLFW has to survive being made on a moved-from window, which holds no handle. Taking the
	 *			source location as a default argument - evaluated at the call site, not here - means the warning names
	 *			the caller, so no call site has to carry a message of its own and none of them can drift out of date.
	 */
	[[nodiscard]] GLFWwindow* _Native(const std::source_location& location = std::source_location::current()) const;

	/**
	 * @brief Reads the modifier keys currently held down, in GLFW's mods encoding
	 * @details GLFW reports modifiers only with key and button events, never with cursor motion, so a synthesized drag
	 *			has to ask for them: caching the ones from the last button press would report Shift as up for the whole
	 *			drag if it was pressed after the button went down.
	 */
	[[nodiscard]] int _CurrentModifiers() const;

private:
	/**
	 * @brief Registers the GLFW callbacks for keyboard, mouse button, cursor and scroll input
	 */
	void _SetIOEventCallbacks() const;

	/**
	 * @brief Registers the GLFW callbacks for resize, framebuffer resize, close and focus
	 */
	void _SetWindowEventCallbacks() const;

	/**
	 * @brief Seeds the cached sizes from the freshly created window
	 * @details The size callbacks only fire on a change, so the initial values have to be read once by hand.
	 */
	void _CacheSizes();

public:
	GlfwWindowEventHandler windowEventHandler;

private:
	Types::GlfwWindowPtr _glfwWindow = nullptr;	///< Smart pointer managing the GLFW window lifetime

	u32 _width = 0;								///< Cached window width, in screen coordinates
	u32 _height = 0;							///< Cached window height, in screen coordinates

	u32 _frameWidth = 0;						///< Cached framebuffer width, in backing pixels
	u32 _frameHeight = 0;						///< Cached framebuffer height, in backing pixels

	/// @brief Bitmask of the mouse buttons currently held, indexed by GLFW button code.
	/// @details GLFW has no native drag event, so a drag is synthesized when the cursor moves while a button is held.
	///			 A mask rather than an array of bool so the common case - nothing held - costs one comparison.
	u8 _pressedMouseButtons = 0;

#if not CE_PLATFORM_MACOS
	/// @brief Windowed-mode placement saved when entering fullscreen, so ToggleFullScreen can restore it on exit.
	/// @details Only used by the glfwSetWindowMonitor fullscreen path; macOS defers to the native NSWindow fullscreen instead.
	int _windowedX = 0;
	int _windowedY = 0;
	int _windowedWidth = 0;
	int _windowedHeight = 0;
#endif
};

}

#endif //CE_CORE_WINDOW_COMMON_GLFWWINDOW_HPP
