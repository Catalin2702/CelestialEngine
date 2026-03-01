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

/**
 * @brief Chronometer constructor - starts timing
 * @details Captures the current high-resolution clock time and stores it as the start point
 */
Chronometer::Chronometer() {
	Start();
}

/**
 * @brief Chronometer destructor - stops timing and prints result
 * @details Captures the end time and prints the elapsed duration
 */
Chronometer::~Chronometer() {
	Stop();
	PrintResult();
}

/**
 * @brief Starts the chronometer
 * @details Records the current time as the start point using high_resolution_clock
 */
void Chronometer::Start() {
	_start = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Stops the chronometer
 * @details Records the current time as the end point using high_resolution_clock
 */
void Chronometer::Stop() {
	_end = std::chrono::high_resolution_clock::now();
}

/**
 * @brief Prints the measured time duration
 * @details Calculates the elapsed time in microseconds and milliseconds,
 *          then logs it using the core logger with rounded values for better readability
 */
void Chronometer::PrintResult() const {
	const auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(_start).time_since_epoch().count();
	const auto end = std::chrono::time_point_cast<std::chrono::milliseconds>(_end).time_since_epoch().count();
	const auto duration = end - start;

	CE_CORE_TRACE("Chronometer duration: {0:.3f}s", duration * .001);
}

}
