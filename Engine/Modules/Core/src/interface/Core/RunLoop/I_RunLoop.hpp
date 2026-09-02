//
// Module: CelestialEngine/Engine/Modules/Core/RunLoop
// File: I_RunLoop.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-01
// Updated by: Catalin Chirosca
// Updated: 2026-09-02
//

#pragma once

#ifndef CE_CORE_RUNLOOP_I_RUNLOOP_HPP
#define CE_CORE_RUNLOOP_I_RUNLOOP_HPP

#include "Types/Var/Vars.hpp"
#include "Utility/Delegate/Delegate.hpp"

#include <memory>


namespace CE::Core {

class I_Platform;

/**
 * @class I_RunLoop
 * @brief Owns the thread of control and calls back once per frame
 * @details The one thing that genuinely differs between the platforms once the window, the device and the swapchain
 *			are behind interfaces. A paced loop keeps the thread of control itself; NSApplication takes it away and
 *			hands frames back through a display link. Both are "Run() blocks until Stop()", which is the only shape
 *			the two have in common and therefore the whole of the interface.
 *
 *			It knows nothing about time, rendering or windows: it decides *when* a frame happens, never what a frame
 *			is. Delta time stays with the application, which is what owns the clock.
 */
class I_RunLoop {
public:
	virtual ~I_RunLoop() = default;

public:
	/**
	 * @brief Creates the run loop that suits who owns the thread of control on this backend
	 * @param platform The live windowing library, which is what decides who owns the thread
	 * @return std::unique_ptr<I_RunLoop> The loop, with no delegates bound yet
	 * @details Chosen from the platform rather than from the graphics API because the question it answers is who owns
	 *			the thread: GLFW, Win32, X11 and Wayland all hand it to us, and only AppKit keeps it. Asking the live
	 *			platform instead of taking a WindowApi means the loop can never be built for a backend other than the
	 *			one actually running.
	 *
	 *			This is the one factory whose default branch is an answer rather than an error - the paced loop is
	 *			correct everywhere the thread is ours, so a new backend needs a case here only if it takes it away.
	 *
	 *			How frames are *paced* is a second question, and it stays inside the implementation: a loop driven by
	 *			the display swaps its pacing source when VSync changes, which is what SetPaused and SetTargetFrameRate
	 *			exist for.
	 */
	[[nodiscard]] static std::unique_ptr<I_RunLoop> MakeRunLoop(const I_Platform& platform);

public:
	/**
	 * @brief Takes the thread of control and drives frames until Stop()
	 * @details Blocking: it returns only once the loop has ended, so it is the last thing an application's Start()
	 *			does. Throws when no frame delegate is bound, since a loop with nothing to call is a spin.
	 */
	virtual void Start() = 0;

	/**
	 * @brief Ends the loop, so that Run() returns
	 * @details Safe to call from inside a frame callback: the current frame finishes first.
	 */
	virtual void Stop() = 0;

public:
	/**
	 * @brief Resume frame delivery
	 */
	virtual void Run() = 0;

	/**
	 * @brief Suspends frame delivery without ending the loop
	 */
	virtual void Pause() = 0;

	/**
	 * @brief Sets the frame rate to aim for
	 * @param framesPerSecond Frames per second; 0 means uncapped
	 * @details Only meaningful where the loop paces itself. A loop driven by the display ignores it, because the
	 *			display decides.
	 */
	virtual void SetTargetFrameRate(u32 framesPerSecond) = 0;

public:
	/**
	 * @brief Binds what to call once per frame
	 */
	virtual void SetFrameDelegate(const EventDelegate<>& onFrame) = 0;

	/**
	 * @brief Binds what to call after the loop is live but before the first frame
	 * @details The moment that exists only because Cocoa has it: the menu bar and the activation policy have to be set
	 *			from inside applicationWillFinishLaunching, which is reachable only once NSApplication is running.
	 */
	virtual void SetDidStartDelegate(const EventDelegate<>& onDidStart) = 0;

public:
	[[nodiscard]] virtual bool IsRunning() const = 0;
	[[nodiscard]] virtual bool IsPaused() const = 0;
	[[nodiscard]] virtual u32 GetTargetFrameRate() const = 0;
};

}

#endif //CE_CORE_RUNLOOP_I_RUNLOOP_HPP
