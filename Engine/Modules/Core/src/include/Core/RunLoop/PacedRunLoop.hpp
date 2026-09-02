//
// Module: CelestialEngine/Engine/Modules/Core/RunLoop
// File: PacedRunLoop.hpp
// Created by: Catalin Chirosca
// Created: 2026-09-01
// Updated by: Catalin Chirosca
// Updated: 2026-09-01
//

#pragma once

#ifndef CE_CORE_RUNLOOP_PACEDRUNLOOP_HPP
#define CE_CORE_RUNLOOP_PACEDRUNLOOP_HPP

#include "Core/RunLoop/I_RunLoop.hpp"
#include "Define/DynamicLinker.hpp"
#include "Utility/Delegate/Dispatcher.hpp"

#include <chrono>


namespace CE::Core {

/**
 * @class PacedRunLoop
 * @brief A loop that owns its thread and sleeps between frames to hold a frame rate
 * @details Deliberately not under Platforms/: there is nothing platform-specific in it. The GLFW application's loop
 *			looked like a GLFW concern, but glfwPollEvents is called by the window inside the frame callback, so what
 *			was left is std::chrono and std::this_thread. The same loop drives Windows and Linux unchanged.
 *
 *			The pacing sleeps rather than spins, and resynchronises instead of catching up: after a long frame it
 *			starts counting again from now, so a hitch costs one late frame rather than a burst of rushed ones.
 */
class CE_CORE_API PacedRunLoop final: public I_RunLoop {
public:
	PacedRunLoop() = default;

	PacedRunLoop(const PacedRunLoop&) = delete;
	PacedRunLoop(PacedRunLoop&&) noexcept = delete;

	~PacedRunLoop() override = default;

public:
	PacedRunLoop& operator = (const PacedRunLoop&) = delete;
	PacedRunLoop& operator = (PacedRunLoop&&) noexcept = delete;

public:
	void Start() override;

	void Stop() override { _started = false; }

public:
	void Run() override { _paused = false; }
	void Pause() override { _paused = true; }

	void SetTargetFrameRate(const u32 framesPerSecond) override { _targetFrameRate = framesPerSecond; }

public:
	void SetFrameDelegate(const EventDelegate<>& onFrame) override { _onFrame.Bind(onFrame); }

	void SetDidStartDelegate(const EventDelegate<>& onDidStart) override { _onDidStart.Bind(onDidStart); }

public:
	[[nodiscard]] bool IsRunning() const override { return _started; }
	[[nodiscard]] bool IsPaused() const override { return _paused; }
	[[nodiscard]] u32 GetTargetFrameRate() const override { return _targetFrameRate; }

private:
	static constexpr std::chrono::milliseconds _pausedFromPolling{8};

private:
	UnicastDispatcher<> _onFrame;
	UnicastDispatcher<> _onDidStart;

	bool _started = false;
	bool _paused = false;

	u32 _targetFrameRate = 0;
};

}

#endif //CE_CORE_RUNLOOP_PACEDRUNLOOP_HPP
