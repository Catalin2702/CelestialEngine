//
// Module: CelestialEngine/Engine/Modules/Core/RunLoop
// File: PacedRunLoop.cpp
// Created by: Catalin Chirosca
// Created: 2026-09-01
// Updated by: Catalin Chirosca
// Updated: 2026-09-01
//

#include "Core/RunLoop/PacedRunLoop.hpp"
#include "Tools/Tools.hpp"

#include <stdexcept>
#include <thread>


namespace CE::Core {

void PacedRunLoop::Start() {
	if (_started) [[unlikely]] {
		CE_CORE_WARN("PacedRunLoop::Run: The loop is already running!");
		return;
	}

	if (not _onFrame.IsBound()) [[unlikely]] {
		constexpr auto error = "PacedRunLoop::Run: No frame delegate is bound!";
		CE_CORE_ERROR(error);
		throw std::runtime_error(error);
	}

	_started = true;
	_paused = false;

	_onDidStart.Dispatch();

	auto nextFrame = std::chrono::steady_clock::now();

	while (_started) [[likely]] {
		if (_paused) [[unlikely]] {
			std::this_thread::sleep_for(_pausedFromPolling);
			nextFrame = std::chrono::steady_clock::now();
			continue;
		}

		_onFrame.Dispatch();

		if (_targetFrameRate == 0) [[unlikely]]
			continue;

		nextFrame += std::chrono::duration_cast<std::chrono::steady_clock::duration>(
			std::chrono::duration<f64>(1.0_f64 / _targetFrameRate)
		);

		if (const auto now = std::chrono::steady_clock::now(); nextFrame > now)
			std::this_thread::sleep_until(nextFrame);
		else
			nextFrame = now;
	}
}


}
