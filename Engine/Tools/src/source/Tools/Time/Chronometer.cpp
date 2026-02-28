//
// Module: Tools/Time
// File: Chronometer.cpp
// Created by: Catalin Chirosca
// Updated by: Catalin Chirosca
// Created: 2026-02-16
// Updated: 2026-02-28
//

#include "Tools/Time/Chronometer.hpp"

#include "Tools/Log/Log.hpp"

namespace CE::Tools::Time {

#ifdef CE_DEBUG
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
	const auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(_start).time_since_epoch().count();
	const auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(_end).time_since_epoch().count();
	const auto duration = end - start;
	const double ms = static_cast<double>(duration) * 0.001;

	CE_CORE_TRACE("Chronometer duration: {0} - ({1} ms)", duration, ms);
}
#endif

}
