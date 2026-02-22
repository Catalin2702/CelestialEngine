//
// Created by Catalin Chirosca on 2026-02-16.
//

#include "Tools/Time/Timer.hpp"
#include "Tools/Log/Log.hpp"

namespace CE::Tools::Time {

#ifdef CE_DEBUG
Timer::Timer() {
	Start();
}

Timer::~Timer() {
	Stop();
	PrintResult();
}

void Timer::Start() {
	_start = std::chrono::high_resolution_clock::now();
}

void Timer::Stop() {
	_end = std::chrono::high_resolution_clock::now();
}

void Timer::PrintResult() const {
	const auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(_start).time_since_epoch().count();
	const auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(_end).time_since_epoch().count();
	const auto duration = end - start;
	const double ms = static_cast<double>(duration) * 0.001;

	CE_CORE_TRACE("Timer duration: {0} - ({1} ms)", duration, ms);
}
#endif

}
