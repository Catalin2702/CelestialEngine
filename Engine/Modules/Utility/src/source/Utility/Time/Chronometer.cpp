//
// Module: CelestialEngine/Engine/Modules/Utility/Time
// File: Chronometer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-08-25
//

#include "Utility/Time/Chronometer.hpp"

#include "Tools/Log/Log.hpp"


namespace CE::Utility {

Chronometer::Chronometer() {
	Start();
}

Chronometer::~Chronometer() {
	Stop();
	PrintResult();
}

void Chronometer::Start() {
	_start = Clock::now();
}

void Chronometer::Stop() {
	if (_isStopped) [[unlikely]]
		return;

	_end = Clock::now();
	_isStopped = true;
}

std::chrono::nanoseconds Chronometer::GetElapsed() const {
	const auto end = _isStopped ? _end : Clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(end - _start);
}

double Chronometer::GetElapsedMilliseconds() const {
	return std::chrono::duration<double, std::milli>(GetElapsed()).count();
}

void Chronometer::PrintResult() const {
	if constexpr (CE_DEBUG) {
		CE_CORE_TRACE("Chronometer duration: {0:.3f}s", GetElapsedMilliseconds() * .001);
	}
}

}
