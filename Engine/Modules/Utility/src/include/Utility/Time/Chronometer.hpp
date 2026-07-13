//
// Module: CelestialEngine/Engine/Modules/Utility/Time
// File: Chronometer.hpp
// Created by: Catalin Chirosca
// Created: 2026-02-16
// Updated by: Catalin Chirosca
// Updated: 2026-07-13
//

#pragma once

#ifndef CE_UTILITY_TIME_TIMER_HPP
#define CE_UTILITY_TIME_TIMER_HPP

#include "Define/DynamicLinker.hpp"

#include <chrono>

namespace CE::Utility {

/**
 * @class Chronometer
 * @brief RAII-style timing utility for measuring code execution time
 * @details Automatically starts timing on construction and stops on destruction,
 *			printing the elapsed time. Only active in debug builds for performance
 *			profiling without impacting release builds.
 */
class CE_API Chronometer {
public:
	/**
	 * @brief Constructor - starts the timer
	 * @details Captures the current high-resolution clock time as the start point
	 */
	Chronometer();

	/**
	 * @brief Destructor - stops the timer and prints results
	 * @details Captures the end time and prints the elapsed duration
	 */
	~Chronometer();

private:
	/**
	 * @brief Starts the chronometer
	 * @details Records the current time as the start point
	 */
	void Start();

	/**
	 * @brief Stops the chronometer
	 * @details Records the current time as the end point
	 */
	void Stop();

	/**
	 * @brief Prints the measured time duration
	 * @details Calculates and outputs the elapsed time between start and stop
	 */
	void PrintResult() const;

private:
	std::chrono::time_point<std::chrono::high_resolution_clock> _start;	///< Start time point
	std::chrono::time_point<std::chrono::high_resolution_clock> _end;	///< End time point
};

}

#endif //CE_UTILITY_TIME_TIMER_HPP
