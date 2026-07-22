//
// Module: CelestialEngine/Engine/Modules/Utility/Time
// File: Chronometer.cpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-22
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
	_start = std::chrono::high_resolution_clock::now();
}

void Chronometer::Stop() {
	_end = std::chrono::high_resolution_clock::now();
}

void Chronometer::PrintResult() const {
#ifdef CE_DEBUG
	const auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(_start).time_since_epoch().count();
	const auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(_end).time_since_epoch().count();
	const auto duration = end - start;

	CE_CORE_TRACE("Chronometer duration: {0:.3f}s", duration * .001);
#endif
}

}
